#include <set>
#include "include/mir/mir.hpp"
#include "include/utils/pattern_match.hpp"
#include "include/requirement/type.hpp"
#include "include/algorithm/union_find/union_find.hpp"

namespace clawn::mir

{
class LifeTimeAnalyzer : public mir::ValueVisitor<LifeTimeAnalyzer>
{
    mutable algorithm::UnionFindTree<MIRID> lifetime_tree;

    public:
    LifeTimeAnalyzer(const LifeTimeAnalyzer &) = default;
    LifeTimeAnalyzer() {}
    void register_id(MIRID id) const
    {
        if (!lifetime_tree.has_parent(id))
        {
            lifetime_tree.add_root(id);
        }
    }

    void visit(const mir::Constant &mir) const { register_id(mir.get_id()); }
    void visit(const mir::ComparePrimitive &mir) const
    {
        register_id(mir.get_id());
    }
    void visit(const mir::Allocate &mir) const { register_id(mir.get_id()); }
    void visit(const mir::Load &mir) const
    {
        register_id(mir.get_id());
        lifetime_tree.unite(mir.get_id(), mir.get_address()->get_id());
        mir.get_address()->accept(*this);
        // std::cout << mir.get_name() << "<->" <<
        // mir.get_address()->to_string() << std::endl;
    }
    void visit(const mir::Store &mir) const
    {
        register_id(mir.get_id());
        auto address = mir.get_address();
        auto value = mir.get_value();
        address->accept(*this);
        value->accept(*this);
        lifetime_tree.unite(address->get_id(), value->get_id());
        // std::cout << address->get_name() << " <->" << value->get_name() <<
        // ":" << lifetime_tree.get_root(address->get_id()) << std::endl;
    }

    void visit(const mir::Cast &mir) const
    {
        register_id(mir.get_id());
        mir.get_value()->accept(*this);
        // register_id(mir.get_id());
        lifetime_tree.unite(mir.get_id(), mir.get_value()->get_id());
    }

    void visit(const mir::SizeOf mir) const { register_id(mir.get_id()); }

    void visit(const mir::GetElementPointer &mir) const
    {
        register_id(mir.get_id());
        mir.get_from()->accept(*this);
        // register_id(mir.get_id());
        lifetime_tree.unite(mir.get_id(), mir.get_from()->get_id());
    }
    void visit(const mir::FunctionCall &mir) const
    {
        register_id(mir.get_id());
        // return
        // ir_builder.CreateLoad(ir_builder.CreateAlloca(mir.get_type()->accept(type_generator),0,"aloc:call"),"calling");
        // return llvm::ConstantInt::get(context, llvm::APInt(64, 0, true));
        // register_id(mir.get_id());
        auto function_name = mir.get_function()
                                 ->get_type()
                                 ->as<requirement::FunctionType>()
                                 .get_name();
        if (function_name.find("[GLOBAL]") == std::string::npos && function_name != "append" && function_name != "get_appended_list")
        {
            return;
        }
        for (auto argument : mir.get_arguments())
        {
            argument->accept(*this);
            lifetime_tree.unite(mir.get_id(), argument->get_id());
        }
    }
    void visit(const mir::Function &function) const
    {
        for (auto block : function.get_body())
        {
            for (auto mir : block->get_mirs())
            {
                // register_id(mir->get_id());
                if (mir->is_type<mir::Value>())
                {
                    mir->as<mir::Value>().accept(*this);
                }
                if (mir->is_type<mir::Return>())
                {
                    auto expression = mir->as<mir::Return>().get_expression();
                    lifetime_tree.unite(mir->get_id(), expression->get_id());
                    expression->accept(*this);
                }
            }
        }
    }

    void visit(const mir::Argument &mir) const { register_id(mir.get_id()); }
    template <typename T>
    void operator()(const T &mir) const
    {
        return visit(mir);
    }

    mir::Function::Analysis analyze(const mir::Function &function)
    {
        visit(function);
        auto analyzer = LifeTimeAnalyzer();
        auto blocks = function.get_body();
        std::set<mir::MIRID> allocations_may_be_used_after_function;
        std::set<mir::MIRID> callings_to_free_associated_memories;
        for (auto block : blocks)
        {
            for (auto mir : block->get_mirs())
            {
                if (mir->is_type<mir::Return>())
                {
                    allocations_may_be_used_after_function.insert(
                        mir->get_id());
                    if (mir->as<mir::Return>()
                            .get_expression()
                            ->is_type<mir::FunctionCall>())
                    {
                        callings_to_free_associated_memories.insert(
                            mir->as<mir::Return>().get_expression()->get_id());
                    }
                }
                if (mir->is_type<mir::Value>() &&
                    mir->as<mir::Value>().is_type<mir::FunctionCall>())
                {
                    callings_to_free_associated_memories.insert(mir->get_id());
                }
            }
        }

        for (auto block : blocks)
        {
            for (auto mir : block->get_mirs())
            {
                for (auto return_expression :
                     allocations_may_be_used_after_function)
                {
                    // std::cout << lifetime_tree.get_root(mir->get_id()) << ","
                    // <<  lifetime_tree.get_root(return_expression) <<
                    // std::endl;
                    if (lifetime_tree.get_root(mir->get_id()) ==
                            lifetime_tree.get_root(return_expression) &&
                        mir->is_type<mir::Value>())
                    {
                        // std::cout << mir->get_name() << " may be used after
                        // function." << std::endl;
                        if (mir->as<mir::Value>().is_type<mir::Allocate>() &&
                            !mir->as<mir::Value>()
                                 .as<mir::Allocate>()
                                 .get_type_to_allocate()
                                 ->is_type<requirement::FunctionType>()
                            //&&
                            // mir->as<mir::Value>().as<mir::Allocate>().get_type_to_allocate()->is_type<requirement::ReferenceType>()
                        )
                        {
                            // std::cout << mir->to_string() << " NEEDS MALLOC!"
                            //<< lifetime_tree.get_root(mir->get_id()) <<
                            // std::endl;
                            allocations_may_be_used_after_function.insert(
                                mir->get_id());
                        }
                        if (mir->as<mir::Value>().is_type<mir::FunctionCall>())
                        {
                            callings_to_free_associated_memories.erase(
                                mir->get_id());
                        }
                    }
                }
            }
        }

        return mir::Function::Analysis(allocations_may_be_used_after_function,
                                       callings_to_free_associated_memories);
    }
};

mir::Function::Analysis Function::analyze() const
{
    LifeTimeAnalyzer analyzer;
    return analyzer.analyze(*this);
}

}  // namespace clawn::mir