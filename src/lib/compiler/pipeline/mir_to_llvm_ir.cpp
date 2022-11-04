#include <optional>
#include <algorithm>
#include <unordered_map>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Verifier.h>
#include "include/mir/mir.hpp"
#include "include/compiler/pipeline.hpp"
#include "include/compiler/error.hpp"
#include "include/compiler/module/module.hpp"
#include "include/resource/resource.hpp"
#include "include/utils/pattern_match.hpp"
#include "include/utils/transform.hpp"
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>

std::string to_string(llvm::Type *type)
{
    std::string str;
    llvm::raw_string_ostream rso(str);
    type->print(rso);
    return rso.str();
}

std::string to_string(llvm::Value *value)
{
    std::string str;
    llvm::raw_string_ostream rso(str);
    value->print(rso);
    return rso.str();
}

namespace clawn::compiler
{
class MIRToLLVMIRImpl
{
    private:
    std::shared_ptr<Module> clawn_module;
    const std::shared_ptr<requirement::TypeEnvironment> type_environment;
    const std::shared_ptr<requirement::TypeSolver> type_solver;
    llvm::LLVMContext &context;
    llvm::Module &llvm_module;
    llvm::IRBuilder<> &ir_builder;
    mutable std::unordered_map<std::string, llvm::AllocaInst *> variables;

    public:
    MIRToLLVMIRImpl(std::shared_ptr<Module> clawn_module,
                    llvm::LLVMContext &context, llvm::Module &llvm_module,
                    llvm::IRBuilder<> &ir_builder)
        : clawn_module(clawn_module),
          type_environment(clawn_module->get_type_environment()),
          type_solver(type_environment->get_solver()),
          context(context),
          llvm_module(llvm_module),
          ir_builder(ir_builder)
    {
    }
};

class LLVMTypeGenerator
    : public utils::VisitorWrapper<LLVMTypeGenerator, requirement::Type>
{
    mutable std::unordered_map<requirement::TypeID, llvm::Type *> generated;

    public:
    llvm::Module &llvm_module;
    llvm::LLVMContext &context;
    llvm::IRBuilder<> &ir_builder;
    LLVMTypeGenerator(llvm::Module &llvm_module, llvm::LLVMContext &context,
                      llvm::IRBuilder<> &ir_builder)
        : llvm_module(llvm_module), context(context), ir_builder(ir_builder)
    {
    }
    llvm::Type *visit(const requirement::VoidType &original) const
    {
        return llvm::Type::getVoidTy(context);
    }
    llvm::Type *visit(const requirement::CType &original) const
    {
        auto kind = original.get_info().kind;
        auto size_in_bits = original.get_info().size_in_bytes * 8;
        if (kind == requirement::CPrimitiveType::BOOL)
        {
            return ir_builder.getInt1Ty();
        }
        if (kind == requirement::CPrimitiveType::CHAR)
        {
            return ir_builder.getIntNTy(size_in_bits);
        }
        if (kind == requirement::CPrimitiveType::DOUBLE)
        {
            return ir_builder.getDoubleTy();
        }
        if (kind == requirement::CPrimitiveType::FLOAT)
        {
            return ir_builder.getFloatTy();
        }
        if (kind == requirement::CPrimitiveType::INT)
        {
            return ir_builder.getIntNTy(size_in_bits);
        }
        if (kind == requirement::CPrimitiveType::LONG)
        {
            return ir_builder.getIntNTy(size_in_bits);
        }
        if (kind == requirement::CPrimitiveType::LONGDOUBLE)
        {
            /*THIS MAY BE DANGEROUS*/
            // std::cerr << "WARNING: LONG DOUBLE IS NOT WELL-SUPPORTED" <<
            // std::endl;
            if (size_in_bits == 128)
            {
                return llvm::Type::getFP128Ty(llvm_module.getContext());
            }
            else
            {
                throw std::runtime_error(
                    "LONG DOUBLE IS NOT SUPPORTED ON THIS ENVIRONMENT.");
            }
        }
        if (kind == requirement::CPrimitiveType::LONGLONG)
        {
            return ir_builder.getIntNTy(size_in_bits);
        }
        if (kind == requirement::CPrimitiveType::SCHAR)
        {
            return ir_builder.getIntNTy(size_in_bits);
        }
        if (kind == requirement::CPrimitiveType::SHORT)
        {
            return ir_builder.getIntNTy(size_in_bits);
        }
        if (kind == requirement::CPrimitiveType::UCHAR)
        {
            return ir_builder.getIntNTy(size_in_bits);
        }
        if (kind == requirement::CPrimitiveType::UINT)
        {
            return ir_builder.getIntNTy(size_in_bits);
        }
        if (kind == requirement::CPrimitiveType::ULONG)
        {
            return ir_builder.getIntNTy(size_in_bits);
        }
        if (kind == requirement::CPrimitiveType::ULONGLONG)
        {
            return ir_builder.getIntNTy(size_in_bits);
        }
        if (kind == requirement::CPrimitiveType::USHORT)
        {
            return ir_builder.getIntNTy(size_in_bits);
        }
        if (kind == requirement::CPrimitiveType::WCHAR)
        {
            return ir_builder.getIntNTy(size_in_bits);
        }
        if (kind == requirement::CPrimitiveType::UNKNOWN)
        {
            return ir_builder.getIntNTy(size_in_bits);
        }
        throw std::runtime_error("not implemented or obsoluted.");
    }
    llvm::Type *visit(const requirement::BooleanType &original) const
    {
        return ir_builder.getInt1Ty();
    }
    llvm::Type *visit(const requirement::IntegerType &original) const
    {
        return ir_builder.getInt64Ty();
    }

    llvm::Type *visit(const requirement::RealNumberType &original) const
    {
        return ir_builder.getDoubleTy();
    }
    llvm::Type *visit(const requirement::ReferenceType &original) const
    {
        if (original.get_reference_to()->is_type<requirement::VoidType>())
        {
            return ir_builder.getInt8PtrTy();
        }
        return original.get_reference_to()->accept(*this)->getPointerTo();
    }
    llvm::Type *visit(const requirement::ListType &original) const
    {
        return original.get_element_type()->get_pointer_to()->accept(*this);
        // throw std::runtime_error("not implemented or obsoluted.");
    }

    llvm::Type *visit(const requirement::UnionType &original) const
    {
        std::vector<uint64_t> tag_type_sizes;
        auto tags = original.get_solved_tags();
        if (tags.size() <= 1)
        {
            return llvm::StructType::get(context, {});
        }
        for (auto &tag_type : tags)
        {
            auto tag_llvm_type = tag_type.second->accept(*this);
            if (tag_llvm_type->isFunctionTy())
            {
                tag_type_sizes.push_back(
                    llvm_module.getDataLayout().getTypeAllocSizeInBits(
                        ir_builder.getInt8PtrTy()));
            }
            else
            {
                tag_type_sizes.push_back(
                    llvm_module.getDataLayout().getTypeAllocSizeInBits(
                        tag_llvm_type));
            }
        }
        if (!tags.count(""))
        {
            std::vector<llvm::Type *> types{ir_builder.getInt64Ty()};
            return llvm::StructType::get(context,
                                         types);  //,
                                                  // original.get_name());
        }
        auto tag_info_type =
            tags.at("")->accept(*this);  // tacit knowledge. not good.
        auto max_size =
            *std::max_element(tag_type_sizes.begin(), tag_type_sizes.end());
        auto llvm_union_type = llvm::StructType::get(
            context, {tag_info_type, ir_builder.getIntNTy(max_size)});  //,
        // original.get_name());
        // std::cout <<
        // original.get_tag_info_type()->get_type_solver()->get_type_by_id(original.get_id())->to_string()
        // << "->" << to_string(llvm_union_type) << std::endl;
        return llvm_union_type;
    }
    llvm::Type *visit(const requirement::StructureType &original) const
    {
        std::vector<llvm::Type *> field;
        for (auto &type : original.get_member_types())
        {
            field.push_back(type.second->accept(*this));
        }
        auto type =
            llvm::StructType::get(context, field);  //, original.get_name());
        return type;
    }
    llvm::Type *visit(const requirement::FunctionType &original) const
    {
        return llvm::FunctionType::get(
                   original.get_return_type()->accept(*this),
                   utils::get_transformed(original.get_argument_types(),
                                          [this](const auto &type) {
                                              return type->accept(*this);
                                          }),
                   false)
            ->getPointerTo();
    }

    llvm::Type *visit(const requirement::UnsolvedType &original) const
    {
        return llvm::StructType::get(
            context, std::vector<llvm::Type *>());  //, "!unsolved");
    }

    template <typename T>
    llvm::Type *operator()(const T &type) const
    {
        auto id = type.get_id();
        if (generated.count(id))
        {
            return generated.at(id);
        }
        else
        {
            auto llvm_type = visit(type);
            generated.insert(std::make_pair(id, llvm_type));
            return llvm_type;
        }
    }
};

class MirToLLVMIR;

class ValueToLLVMIR : public mir::ValueVisitor<ValueToLLVMIR>
{
    llvm::LLVMContext &context;
    llvm::Module &llvm_module;
    llvm::IRBuilder<> &ir_builder;
    std::function<llvm::Value *(mir::MIR &)> mir_converter;
    std::unordered_map<std::shared_ptr<mir::BasicBlock>, llvm::BasicBlock *>
        &basic_block_map;
    LLVMTypeGenerator &type_generator;
    mutable std::unordered_map<mir::MIRID, llvm::Value *> generated;
    mutable std::set<clawn::mir::MIRID> allocations_may_be_used_after_function;
    mutable std::set<clawn::mir::MIRID> callings_to_free_associated_memories;
    mutable std::unordered_map<mir::MIRID, std::vector<llvm::Value *>>
        calling_ids_to_free;
    mutable llvm::Value *current_function_session_id;

    public:
    ValueToLLVMIR(const ValueToLLVMIR &) = default;
    ValueToLLVMIR(llvm::LLVMContext &context, llvm::Module &llvm_module,
                  llvm::IRBuilder<> &ir_builder,
                  std::function<llvm::Value *(mir::MIR &)> mir_converter,
                  std::unordered_map<std::shared_ptr<mir::BasicBlock>,
                                     llvm::BasicBlock *> &basic_block_map,
                  LLVMTypeGenerator &type_generator)
        : context(context),
          llvm_module(llvm_module),
          ir_builder(ir_builder),
          mir_converter(mir_converter),
          basic_block_map(basic_block_map),
          type_generator(type_generator)
    {
    }

    llvm::Value *get_builtin_use_heap() const
    {
        auto use_heap_function = llvm_module.getFunction("use_heap");
        if (!use_heap_function)
        {
            std::vector<llvm::Type *> use_heap_types(1,
                                                     ir_builder.getInt64Ty());
            auto use_heap_declaration_type = llvm::FunctionType::get(
                ir_builder.getInt64Ty()->getPointerTo(), use_heap_types, false);
            use_heap_function = llvm::Function::Create(
                use_heap_declaration_type, llvm::Function::ExternalLinkage,
                "use_heap", &llvm_module);
        }
        return use_heap_function;
    }

    llvm::Value *get_builtin_get_unique_number() const
    {
        auto function = llvm_module.getFunction("get_unique_number");
        if (!function)
        {
            auto declaration_type =
                llvm::FunctionType::get(ir_builder.getInt64Ty(), {}, false);
            function = llvm::Function::Create(
                declaration_type, llvm::Function::ExternalLinkage,
                "get_unique_number", &llvm_module);
        }
        return function;
    }

    llvm::Value *get_builtin_register_calling_id() const
    {
        auto function = llvm_module.getFunction("register_calling_id");
        if (!function)
        {
            std::vector<llvm::Type *> argument_types{};
            auto declaration_type = llvm::FunctionType::get(
                ir_builder.getVoidTy(), argument_types, false);
            function = llvm::Function::Create(
                declaration_type, llvm::Function::ExternalLinkage,
                "register_calling_id", &llvm_module);
        }
        return function;
    }

    llvm::Value *get_builtin_register_calling_id_to_free() const
    {
        auto function = llvm_module.getFunction("register_calling_id_to_free");
        if (!function)
        {
            std::vector<llvm::Type *> argument_types(1,
                                                     ir_builder.getInt64Ty());
            auto declaration_type = llvm::FunctionType::get(
                ir_builder.getVoidTy(), argument_types, false);
            function = llvm::Function::Create(
                declaration_type, llvm::Function::ExternalLinkage,
                "register_calling_id_to_free", &llvm_module);
        }
        return function;
    }

    llvm::Value *get_builtin_free_heaps_associated_calling_id() const
    {
        auto function =
            llvm_module.getFunction("free_heaps_associated_calling_id");
        if (!function)
        {
            std::vector<llvm::Type *> argument_types(1,
                                                     ir_builder.getInt64Ty());
            auto declaration_type = llvm::FunctionType::get(
                ir_builder.getVoidTy(), argument_types, false);
            function = llvm::Function::Create(
                declaration_type, llvm::Function::ExternalLinkage,
                "free_heaps_associated_calling_id", &llvm_module);
        }
        return function;
    }

    llvm::Value *visit(const mir::Constant &mir) const
    {
        return utils::Match{mir.get_variant()}(
            utils::Type<mir::Integer>(),
            [this](const mir::Integer &constant) -> llvm::Value * {
                return llvm::ConstantInt::get(
                    context, llvm::APInt(64, constant.get_value(), true));
            },
            utils::Type<mir::RealNumber>(),
            [this](const mir::RealNumber &constant) -> llvm::Value * {
                return llvm::ConstantFP::get(
                    context, llvm::APFloat((double)constant.get_value()));
            },
            utils::Type<mir::StringPointer>(),
            [this](const mir::StringPointer &constant) -> llvm::Value * {
                return ir_builder.CreateGlobalStringPtr(constant.get_value());
                // throw std::runtime_error("not implemented or obsoluted.");
                // return nullptr;
            },
            utils::Default(),
            [](const auto &) -> llvm::Value * { return nullptr; });
    }
    llvm::Value *visit(const mir::ComparePrimitive &mir) const
    {
        llvm::Value *first = mir.get_values().first->accept(*this);
        llvm::Value *second = mir.get_values().second->accept(*this);
        if (first->getType() != second->getType())
        {
            // throw std::runtime_error("different types are not comparable");
        }
        if (first->getType()->isIntegerTy() && second->getType()->isIntegerTy())
        {
            return ir_builder.CreateICmpEQ(first, second, mir.get_name());
        }
        first->getType()->print(llvm::outs());
        llvm::outs().flush();
        second->getType()->print(llvm::outs());
        llvm::outs().flush();
        throw std::runtime_error("types are not comparable");
    }
    llvm::Value *visit(const mir::Allocate &mir) const
    {
        auto type_to_allocate =
            mir.get_type_to_allocate()->accept(type_generator);

        if (mir.is_global())
        {
            return new llvm::GlobalVariable(
                llvm_module, type_to_allocate, false,
                llvm::GlobalVariable::ExternalLinkage,
                llvm::Constant::getNullValue(type_to_allocate), mir.get_name());
        }
        else
        {
            if (allocations_may_be_used_after_function.count(mir.get_id()))
            {
                auto size = llvm::ConstantInt::get(
                    context,
                    llvm::APInt(
                        64,
                        llvm_module.getDataLayout().getTypeAllocSizeInBits(
                            type_to_allocate),
                        true));
                auto use_heap_function = get_builtin_use_heap();
                return ir_builder.CreateBitCast(
                    ir_builder.CreateCall(use_heap_function, {size}),
                    type_to_allocate->getPointerTo());
            }
            return ir_builder.CreateAlloca(type_to_allocate, 0, mir.get_name());
        }
    }
    llvm::Value *visit(const mir::Load &mir) const
    {
        return ir_builder.CreateLoad(mir.get_address()->accept(*this),
                                     mir.get_name());
    }
    llvm::Value *visit(const mir::Store &mir) const
    {
        auto value = mir.get_value()->accept(*this);
        auto address = mir.get_address()->accept(*this);
        ir_builder.CreateStore(value, address);
        return ir_builder.CreateLoad(address);
    }

    llvm::Value *visit(const mir::Cast &mir) const
    {
        auto value = mir.get_value()->accept(*this);
        auto cast_to = mir.get_cast_to()->accept(type_generator);
        return ir_builder.CreateBitCast(value, cast_to, mir.get_name());
    }

    llvm::Value *visit(const mir::SizeOf size_of) const
    {
        auto type = size_of.get_target_type()->accept(type_generator);
        auto size = llvm_module.getDataLayout().getTypeAllocSizeInBits(type);
        return llvm::ConstantInt::get(context, llvm::APInt(64, size, true));
    }

    llvm::Value *visit(const mir::GetElementPointer &mir) const
    {
        auto from = mir.get_from()
                        ->get_type()
                        ->as<requirement::ReferenceType>()
                        .get_reference_to();
        if (from->is_type<requirement::StructureType>())
        {
            auto t = mir.get_from()->accept(*this)->getType();
            return ir_builder.CreateStructGEP(
                // llvm::dyn_cast<llvm::StructType>(mir.get_from()->get_type()->accept(type_generator)),
                mir.get_from()->accept(*this), mir.get_index(), mir.get_name());
        }
        if (from->is_type<requirement::UnionType>())
        {
            auto &union_type = from->as<requirement::UnionType>();
            auto accessed_type = union_type.get_element_type(mir.get_index());

            llvm::Type *type_to_cast =
                llvm::StructType::get(
                    context,
                    {union_type.get_tag_info_type()->accept(type_generator),
                     accessed_type->accept(type_generator)})
                    ->getPointerTo();
            auto casted = ir_builder.CreateBitCast(
                mir.get_from()->accept(*this), type_to_cast,
                "casted_for_union_access");

            auto actuall_idnex =
                mir.get_index()
                    ? 1
                    : 0;  // if index is more or equal to 1, it's 1 because type
                          // is casted to {tag,accessed},if index is 0, actuall
                          // 0 is needed

            return ir_builder.CreateStructGEP(casted, actuall_idnex,
                                              mir.get_name());
        }
        throw std::runtime_error("invalid access." + from->to_string());
    }
    llvm::Value *visit(const mir::FunctionCall &mir) const
    {
        auto calling = ir_builder.CreateCall(
            mir.get_function()->accept(*this),
            utils::get_transformed(mir.get_arguments(),
                                   [this](const auto &argument) {
                                       return argument->accept(*this);
                                   }));
        // std::cout << mir.get_id() << std::endl;
        auto function_name = mir.get_function()
                                 ->get_type()
                                 ->as<requirement::FunctionType>()
                                 .get_name();
        if (callings_to_free_associated_memories.count(mir.get_id()))
        {
            if (function_name.find("[GLOBAL]") == std::string::npos)
            {  // ignore if function is written in C. Dirty but will be fixed.
                std::vector<llvm::Value *> arguments = {
                    current_function_session_id};
                ir_builder.CreateCall(get_builtin_register_calling_id_to_free(),
                                      arguments);
            }
        }
        else
        {
            ir_builder.CreateCall(get_builtin_register_calling_id(), {});
        }

        return calling;
        // return
        // ir_builder.CreateLoad(ir_builder.CreateAlloca(mir.get_type()->accept(type_generator),0,"aloc:call"),"calling");
        // return llvm::ConstantInt::get(context, llvm::APInt(64, 0, true));
    }
    llvm::Value *visit(const mir::Function &function) const
    {
        if (function.get_name() == "use_heap")
        {
            return get_builtin_use_heap();
        }
        if (function.get_name() == "get_unique_number")
        {
            return get_builtin_get_unique_number();
        }
        if (function.get_name() == "register_calling_id")
        {
            return get_builtin_register_calling_id();
        }
        if (function.get_name() == "register_calling_id_to_free")
        {
            return get_builtin_register_calling_id_to_free();
        }
        if (function.get_name() == "free_heaps_associated_calling_id")
        {
            return get_builtin_free_heaps_associated_calling_id();
        }

        if (function.get_name().find("[GLOBAL]") == std::string::npos)
        {
            auto got = llvm_module.getFunction(function.get_name());
            if (got)
            {
                return got;
            }
        }

        auto analysis = function.analyze();
        allocations_may_be_used_after_function =
            analysis.allocations_may_be_used_after_function;
        callings_to_free_associated_memories =
            analysis.callings_to_free_associated_memories;

        auto previous_block = ir_builder.GetInsertBlock();
        if (generated.count(function.get_id()))
        {
            return generated.at(function.get_id());
        }

        auto &function_type =
            function.get_type()->as<requirement::FunctionType>();
        auto llvm_function_type = llvm::FunctionType::get(
            function_type.get_return_type()->accept(type_generator),
            utils::get_transformed(function_type.get_argument_types(),
                                   [this](const auto &type) {
                                       return type->accept(type_generator);
                                   }),
            false);

        auto llvm_function = llvm::Function::Create(
            llvm_function_type, llvm::Function::ExternalLinkage,
            function.get_name(), &llvm_module);
        generated.insert(std::make_pair(
            function.get_id(),
            llvm_function));  // suppress recursion from Argument(argument mir
                              // references the function when it be converted to
                              // llvm ir).
        // return llvm_function;
        auto blocks = function.get_body();
        auto llvm_blocks = utils::get_transformed(
            blocks, [this, &llvm_function](const auto &block_) {
                if (basic_block_map.count(block_))
                {
                    return basic_block_map.at(block_);
                }
                auto llvm_block = llvm::BasicBlock::Create(
                    context, block_->get_name(), llvm_function);
                basic_block_map.insert(std::make_pair(block_, llvm_block));
                return llvm_block;
            });

        llvm::Value *session_id;
        if (!llvm_blocks.empty())
        {
            ir_builder.SetInsertPoint(llvm_blocks.at(0));
            session_id =
                ir_builder.CreateCall(get_builtin_get_unique_number());
            current_function_session_id = session_id;
        }

        for (size_t i = 0; i < blocks.size(); i++)
        {
            ir_builder.SetInsertPoint(llvm_blocks.at(i));
            callings_to_free_associated_memories =
                analysis.callings_to_free_associated_memories;
            current_function_session_id = session_id;
            for (auto &mir : blocks.at(i)->get_mirs())
            {
                std::vector<llvm::Value *> argument_for_free{
                    current_function_session_id};
                if (mir->is_type<mir::Return>())
                {
                    ir_builder.CreateCall(
                        get_builtin_free_heaps_associated_calling_id(),
                        argument_for_free);
                    mir_converter(*mir);
                    break;
                }
                auto got = mir_converter(*mir);
                if (mir->get_id() == blocks.at(i)->get_mirs().back()->get_id())
                {
                    if (i == blocks.size() - 1)
                    {
                        ir_builder.CreateCall(
                            get_builtin_free_heaps_associated_calling_id(),
                            argument_for_free);
                        ir_builder.CreateRet(got);
                    }
                }
                ir_builder.SetInsertPoint(llvm_blocks.at(i));
                allocations_may_be_used_after_function =
                    analysis.allocations_may_be_used_after_function;
                callings_to_free_associated_memories =
                    analysis.callings_to_free_associated_memories;
                current_function_session_id = session_id;
            }
        }
        ir_builder.SetInsertPoint(previous_block);
        return llvm_function;
    }

    llvm::Value *visit(const mir::Argument &mir) const
    {
        auto argument =
            (llvm::dyn_cast<llvm::Function>(visit(mir.get_function()))
                 ->args()
                 .begin() +
             mir.get_index());
        argument->setName("arg" + std::to_string(mir.get_index()));
        return argument;
    }
    template <typename T>
    llvm::Value *operator()(const T &mir) const
    {
        auto id = mir.get_id();
        if (generated.count(id))
        {
            return generated.at(id);
        }
        else
        {
            auto llvm_ir = visit(mir);
            generated.insert(std::make_pair(id, llvm_ir));
            return llvm_ir;
        }
    }
};

class MirToLLVMIR : public mir::MIRVisitor<MirToLLVMIR>
{
    llvm::LLVMContext &context;
    llvm::Module &llvm_module;
    llvm::IRBuilder<> &ir_builder;
    std::shared_ptr<ValueToLLVMIR> value_converter;
    std::unordered_map<std::shared_ptr<mir::BasicBlock>, llvm::BasicBlock *>
        basic_block_map;

    public:
    MirToLLVMIR(llvm::LLVMContext &context, llvm::Module &llvm_module,
                llvm::IRBuilder<> &ir_builder,
                LLVMTypeGenerator &type_generator)
        : context(context), llvm_module(llvm_module), ir_builder(ir_builder)
    {
        std::function<llvm::Value *(mir::MIR &)> converter =
            [this](mir::MIR &mir) { return mir.accept(*this); };
        value_converter = std::make_shared<ValueToLLVMIR>(
            context, llvm_module, ir_builder, converter, basic_block_map,
            type_generator);
    }
    llvm::Value *visit(const mir::Value &value) const
    {
        return value.accept(*value_converter);
    }
    llvm::Value *visit(const mir::Jump &jump) const
    {
        auto destination = basic_block_map.at(jump.get_destination());
        return ir_builder.CreateBr(destination);
    }
    llvm::Value *visit(const mir::ConditionalJump &conditional_jump) const
    {
        auto on_true =
            basic_block_map.at(conditional_jump.get_destination_on_true());
        auto on_false =
            basic_block_map.at(conditional_jump.get_destination_on_false());
        return ir_builder.CreateCondBr(
            conditional_jump.get_condition()->accept(*this), on_true, on_false);
    }
    llvm::Value *visit(const mir::Return &returning) const
    {
        return ir_builder.CreateRet(returning.get_expression()->accept(*this));
    }
};

void pipeline::mir_to_llvm_ir(mir::Context &mir_context,
                              llvm::LLVMContext &llvm_context,
                              llvm::Module &llvm_module,
                              llvm::IRBuilder<> &ir_builder)
{
    // auto function_type =
    // llvm::FunctionType::get(llvm::Type::getInt8Ty(llvm_context),
    //                                             {}, false);
    // auto function = llvm::Function::Create(
    //     function_type, llvm::Function::ExternalLinkage, "main",
    //     &llvm_module);
    // auto block = llvm::BasicBlock::Create(llvm_context, "entry", function);
    // ir_builder.SetInsertPoint(block);

    LLVMTypeGenerator type_generator(llvm_module, llvm_context, ir_builder);
    auto converter =
        MirToLLVMIR(llvm_context, llvm_module, ir_builder, type_generator);
    for (auto &function : mir_context.get_functions())
    {
        converter.visit(*function);
    }
    std::error_code error;
    llvm::raw_fd_ostream stream("llvmir", error);
    // llvm::sys::fs::OpenFlags::F_None);
    llvm_module.print(stream, nullptr);
    llvm::verifyModule(llvm_module, &llvm::outs());
}
}  // namespace clawn::compiler