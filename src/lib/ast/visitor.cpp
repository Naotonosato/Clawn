#include <vector>
#include "include/ast/ast.hpp"
#include "include/ast/visitor.hpp"
#include "include/utils/transform.hpp"

namespace clawn::ast
{
class FlattenerImpl
    : public Visitor<std::vector<std::reference_wrapper<const AST>>>
{
    void flatten(
        const AST& ast,
        std::vector<std::reference_wrapper<const AST>>& flattened) const
    {
        flattened.push_back(ast);
        auto flat_ast = ast.accept(*this);
        flattened.insert(flattened.end(),
                         std::make_move_iterator(flat_ast.begin()),
                         std::make_move_iterator(flat_ast.end()));
    }
    std::vector<std::reference_wrapper<const AST>> visit(const Root& node) const
    {
        std::vector<std::reference_wrapper<const AST>> flattened;
        for (auto& expression : node.get_children())
        {
            flatten(expression, flattened);
        }
        return flattened;
    }

    std::vector<std::reference_wrapper<const AST>> visit(
        const Integer& node) const
    {
        return {};
    }

    std::vector<std::reference_wrapper<const AST>> visit(
        const Float& node) const
    {
        return {};
    }

    std::vector<std::reference_wrapper<const AST>> visit(const List& node) const
    {
        auto initial_values = node.get_initial_values();

        std::vector<std::reference_wrapper<const AST>> flattened;
        for (auto& expression : node.get_initial_values())
        {
            flatten(expression, flattened);
        }
        return flattened;
    }

    std::vector<std::reference_wrapper<const AST>> visit(
        const String& node) const
    {
        return {};
    }

    std::vector<std::reference_wrapper<const AST>> visit(
        const SetResult& node) const
    {
        std::vector<std::reference_wrapper<const AST>> flattened;
        flatten(node.get_expression(), flattened);
        return flattened;
    }

    std::vector<std::reference_wrapper<const AST>> visit(
        const Variable& node) const
    {
        return {};
    }

    std::vector<std::reference_wrapper<const AST>> visit(
        const VariableDefinition& node) const
    {
        std::vector<std::reference_wrapper<const AST>> flattened;
        flatten(node.get_initial_value(), flattened);
        return flattened;
    }

    std::vector<std::reference_wrapper<const AST>> visit(
        const GlobalVariable& node) const
    {
        std::vector<std::reference_wrapper<const AST>> flattened;
        flatten(node.get_initial_value(), flattened);
        return flattened;
    }

    std::vector<std::reference_wrapper<const AST>> visit(
        const Assignment& node) const
    {
        std::vector<std::reference_wrapper<const AST>> flattened;
        flatten(node.get_left_hand_side(), flattened);
        flatten(node.get_right_hand_side(), flattened);
        return flattened;
    }

    std::vector<std::reference_wrapper<const AST>> visit(
        const Reference& node) const
    {
        std::vector<std::reference_wrapper<const AST>> flattened;
        flatten(node.get_refer_to(), flattened);
        return flattened;
    }

    std::vector<std::reference_wrapper<const AST>> visit(
        const Dereference& node) const
    {
        std::vector<std::reference_wrapper<const AST>> flattened;
        flatten(node.get_target(), flattened);
        return flattened;
    }

    std::vector<std::reference_wrapper<const AST>> visit(
        const BinaryExpression& node) const
    {
        std::vector<std::reference_wrapper<const AST>> flattened;
        flatten(node.get_left_hand_side(), flattened);
        flatten(node.get_right_hand_side(), flattened);
        return flattened;
    }

    std::vector<std::reference_wrapper<const AST>> visit(
        const FunctionCall& node) const
    {
        std::vector<std::reference_wrapper<const AST>> flattened;
        auto arguments = node.get_arguments();
        for (auto& argument : arguments)
        {
            flatten(argument, flattened);
        }
        flatten(node.get_function(), flattened);
        return flattened;
    }

    std::vector<std::reference_wrapper<const AST>> visit(
        const AccessElement& node) const
    {
        std::vector<std::reference_wrapper<const AST>> flattened;
        flatten(node.get_left_hand_side(), flattened);
        return flattened;
    }

    std::vector<std::reference_wrapper<const AST>> visit(
        const AccessList& node) const
    {
        std::vector<std::reference_wrapper<const AST>> flattened;
        flatten(node.get_list(), flattened);
        flatten(node.get_index(), flattened);
        return flattened;
    }

    std::vector<std::reference_wrapper<const AST>> visit(
        const DeepCopy& node) const
    {
        std::vector<std::reference_wrapper<const AST>> flattened;
        flatten(node.get_left_hand_side(), flattened);
        flatten(node.get_right_hand_side(), flattened);
        return flattened;
    }

    std::vector<std::reference_wrapper<const AST>> visit(
        const Block& node) const
    {
        std::vector<std::reference_wrapper<const AST>> flattened;
        for (auto& expression : node.get_expressions())
        {
            flatten(expression, flattened);
        }
        return flattened;
    }

    std::vector<std::reference_wrapper<const AST>> visit(
        const Match& node) const
    {
        std::vector<std::reference_wrapper<const AST>> flattened;
        flatten(node.get_target(), flattened);
        for (auto& pattern : node.get_patterns())
        {
            flatten(pattern.second, flattened);
        }
        auto default_case = node.get_default_case();
        if (default_case.has_value())
        {
            flatten(default_case.value(), flattened);
        }
        return flattened;
    }

    std::vector<std::reference_wrapper<const AST>> visit(const If& node) const
    {
        std::vector<std::reference_wrapper<const AST>> flattened;
        flatten(node.get_condition(), flattened);
        flatten(node.get_body(), flattened);
        auto else_body = node.get_else_body();
        if (else_body.has_value())
        {
            flatten(node.get_else_body().value().get(), flattened);
        }
        return flattened;
    }

    std::vector<std::reference_wrapper<const AST>> visit(
        const Repeat& node) const
    {
        std::vector<std::reference_wrapper<const AST>> flattened;
        flatten(node.get_condition(), flattened);
        flatten(node.get_body(), flattened);
        return flattened;
    }

    std::vector<std::reference_wrapper<const AST>> visit(
        const GenericFunctionDeclaration& node) const
    {
        return {};
    }

    std::vector<std::reference_wrapper<const AST>> visit(
        const GenericFunction& node) const
    {
        std::vector<std::reference_wrapper<const AST>> flattened;
        flatten(node.get_body(), flattened);
        return flattened;
    }

    std::vector<std::reference_wrapper<const AST>> visit(
        const UnionConstructorDeclaration& node) const
    {
        return {};
    }
    std::vector<std::reference_wrapper<const AST>> visit(
        const GenericUnion& node) const
    {
        return {};
    }

    std::vector<std::reference_wrapper<const AST>> visit(
        const Construction& node) const
    {
        std::vector<std::reference_wrapper<const AST>> flattened;
        for (auto& pair : node.get_arguments())
        {
            flatten(pair.second.get(), flattened);
        }
        flatten(node.get_constructor(), flattened);
        return flattened;
    }

    std::vector<std::reference_wrapper<const AST>> visit(
        const GenericStructureDeclaration& node) const
    {
        return {};
    }

    std::vector<std::reference_wrapper<const AST>> visit(
        const GenericStructure& node) const
    {
        return {};
    }

    std::vector<std::reference_wrapper<const AST>> visit(
        const GenericClass& node) const
    {
        std::vector<std::reference_wrapper<const AST>> flattened;

        for (auto& member_variable : node.get_member_variables())
        {
            flatten(member_variable.get(), flattened);
        }
        for (auto& member_function : node.get_member_functions())
        {
            flatten(member_function.get(), flattened);
        }

        return flattened;
    }

    std::vector<std::reference_wrapper<const AST>> visit(
        const TypeID& node) const
    {
        std::vector<std::reference_wrapper<const AST>> flattened;
        flatten(node.get_expression(), flattened);
        return flattened;
    }

    std::vector<std::reference_wrapper<const AST>> visit(
        const Return& node) const
    {
        std::vector<std::reference_wrapper<const AST>> flattened;
        flatten(node.get_expression(), flattened);
        return flattened;
    }

    std::vector<std::reference_wrapper<const AST>> visit(
        const CTypeDeclaration& node) const
    {
        return {};
    }

    std::vector<std::reference_wrapper<const AST>> visit(
        const CFunctionDeclaration& node) const
    {
        return {};
    }
};

Flattener::Flattener() : impl(std::make_unique<FlattenerImpl>()) {}

Flattener::~Flattener() = default;

std::vector<std::reference_wrapper<const AST>> Flattener::visit(const AST& node)
{
    struct FlattenerHelper
    {
        const FlattenerImpl& impl;
        FlattenerHelper(const FlattenerImpl& impl_) : impl(impl_) {}
        void flatten_ast(
            const std::vector<std::reference_wrapper<const AST>>& nodes,
            std::vector<std::reference_wrapper<const AST>>& result)
        {
            if (nodes.size() == 0)
            {
                return;
            }
            for (auto&& node_ : nodes)
            {
                result.push_back(node_);
                flatten_ast(node_.get().accept(impl), result);
            }
            return;
        }
    };
    std::vector<std::reference_wrapper<const AST>> result;
    FlattenerHelper(*impl).flatten_ast(node.accept(*impl), result);
    return result;
}

class ClonerImpl
    : public ASTVisitor<ClonerImpl>  // public Visitor<std::unique_ptr<AST>>
{
    public:
    std::unique_ptr<AST> visit(const Root& node) const
    {
        std::vector<std::unique_ptr<AST>> children = utils::get_transformed(
            node.get_children(),
            [this](const AST& node) { return node.accept(*this); });

        return AST::create<Root>(node.get_location(), std::move(children));
    }

    std::unique_ptr<AST> visit(const Integer& node) const
    {
        return AST::create<Integer>(node.get_location(),
                                    node.get_initial_value());
    }

    std::unique_ptr<AST> visit(const Float& node) const
    {
        return AST::create<Float>(node.get_location(),
                                  node.get_initial_value());
    }

    std::unique_ptr<AST> visit(const List& node) const
    {
        std::vector<std::unique_ptr<AST>> clone_of_initial_values =
            utils::get_transformed(
                node.get_initial_values(),
                [this](const AST& node) { return node.accept(*this); });
        return AST::create<List>(node.get_location(),
                                 std::move(clone_of_initial_values));
    }

    std::unique_ptr<AST> visit(const String& node) const
    {
        return AST::create<String>(node.get_location(),
                                   node.get_initial_value());
    }

    std::unique_ptr<AST> visit(const SetResult& node) const
    {
        return AST::create<SetResult>(node.get_location(),
                                      node.get_expression().accept(*this));
    }

    std::unique_ptr<AST> visit(const Variable& node) const
    {
        return AST::create<Variable>(node.get_location(), node.get_name());
    }

    std::unique_ptr<AST> visit(const Reference& node) const
    {
        auto clone_of_refer_to = node.get_refer_to().accept(*this);
        return AST::create<Reference>(node.get_location(),
                                      std::move(clone_of_refer_to));
    }

    std::unique_ptr<AST> visit(const Dereference& node) const
    {
        auto clone_of_target = node.get_target().accept(*this);
        return AST::create<Dereference>(node.get_location(),
                                        std::move(clone_of_target));
    }

    std::unique_ptr<AST> visit(const VariableDefinition& node) const
    {
        auto clone_of_initial_value = node.get_initial_value().accept(*this);
        return AST::create<VariableDefinition>(
            node.get_location(), node.get_name(),
            std::move(clone_of_initial_value));
    }

    std::unique_ptr<AST> visit(const GlobalVariable& node) const
    {
        auto clone_of_initial_value = node.get_initial_value().accept(*this);
        return AST::create<GlobalVariable>(node.get_location(), node.get_name(),
                                           std::move(clone_of_initial_value));
    }

    std::unique_ptr<AST> visit(const Assignment& node) const
    {
        return AST::create<Assignment>(
            node.get_location(), node.get_left_hand_side().accept(*this),
            node.get_right_hand_side().accept(*this));
    }

    std::unique_ptr<AST> visit(const BinaryExpression& node) const
    {
        return AST::create<BinaryExpression>(
            node.get_location(), node.get_operator(),
            node.get_left_hand_side().accept(*this),
            node.get_right_hand_side().accept(*this));
    }

    std::unique_ptr<AST> visit(const FunctionCall& node) const
    {
        std::vector<std::unique_ptr<AST>> clone_of_arguments =
            utils::get_transformed(
                node.get_arguments(),
                [this](const AST& node) { return node.accept(*this); });
        return AST::create<FunctionCall>(node.get_location(),
                                         node.get_function().accept(*this),
                                         std::move(clone_of_arguments));
    }

    std::unique_ptr<AST> visit(const AccessElement& node) const
    {
        return AST::create<AccessElement>(
            node.get_location(), node.get_left_hand_side().accept(*this),
            node.get_element_name());
    }

    std::unique_ptr<AST> visit(const AccessList& node) const
    {
        return AST::create<AccessList>(node.get_location(),
                                       node.get_list().accept(*this),
                                       node.get_index().accept(*this));
    }

    std::unique_ptr<AST> visit(const DeepCopy& node) const
    {
        return AST::create<DeepCopy>(node.get_location(),
                                     node.get_left_hand_side().accept(*this),
                                     node.get_right_hand_side().accept(*this));
    }

    std::unique_ptr<AST> visit(const Block& node) const
    {
        std::vector<std::unique_ptr<AST>> clone_of_expressions =
            utils::get_transformed(
                node.get_expressions(),
                [this](const AST& node) { return node.accept(*this); });

        return AST::create<Block>(node.get_location(),
                                  std::move(clone_of_expressions));
    }

    std::unique_ptr<AST> visit(const Match& node) const
    {
        auto cloned_target = node.get_target().accept(*this);
        auto cloned_patterns = utils::get_transformed(
            node.get_patterns(),
            [this](
                const std::pair<std::string, std::reference_wrapper<const AST>>&
                    pattern) {
                return std::make_pair(pattern.first,
                                      pattern.second.get().accept(*this));
            });
        auto default_case = node.get_default_case();
        if (default_case.has_value())
        {
            return AST::create<Match>(node.get_location(),
                                      std::move(cloned_target),
                                      std::move(cloned_patterns),
                                      default_case.value().get().accept(*this));
        }
        else
        {
            return AST::create<Match>(node.get_location(),
                                      std::move(cloned_target),
                                      std::move(cloned_patterns));
        }
    }

    std::unique_ptr<AST> visit(const If& node) const
    {
        auto clone_of_condition = node.get_condition().accept(*this);
        auto clone_of_body = node.get_condition().accept(*this);
        if (node.get_else_body().has_value())
        {
            return AST::create<If>(
                node.get_location(), std::move(clone_of_condition),
                std::move(clone_of_body),
                node.get_else_body().value().get().accept(*this));
        }
        else
        {
            return AST::create<If>(node.get_location(),
                                   std::move(clone_of_condition),
                                   std::move(clone_of_body));
        }
    }

    std::unique_ptr<AST> visit(const Repeat& node) const
    {
        return AST::create<Repeat>(node.get_location(), node.get_counter_name(),
                                   node.get_condition().accept(*this),
                                   node.get_body().accept(*this));
    }

    std::unique_ptr<AST> visit(const GenericFunctionDeclaration& node) const
    {
        return AST::create<GenericFunctionDeclaration>(node.get_location(),
                                                       node.get_name());
    }

    std::unique_ptr<AST> visit(const GenericFunction& node) const
    {
        auto clone_of_arguments = node.get_parameters();
        return AST::create<GenericFunction>(node.get_location(),
                                            node.get_name(), clone_of_arguments,
                                            node.get_body().accept(*this));
    }

    std::unique_ptr<AST> visit(const UnionConstructorDeclaration& node) const
    {
        return AST::create<UnionConstructorDeclaration>(
            node.get_location(), node.get_union_name(), node.get_tag_name(),
            node.get_constructor_name());
    }

    std::unique_ptr<AST> visit(const GenericUnion& node) const
    {
        return AST::create<GenericUnion>(node.get_location(), node.get_name(),
                                         node.get_tags());
    }

    std::unique_ptr<AST> visit(const Construction& node) const
    {
        std::vector<std::pair<std::string, std::unique_ptr<AST>>>
            clone_of_arguments;
        for (auto& pair : node.get_arguments())
        {
            clone_of_arguments.push_back(
                std::make_pair(pair.first, pair.second.get().accept(*this)));
        }
        return AST::create<Construction>(node.get_location(),
                                         node.get_constructor().accept(*this),
                                         std::move(clone_of_arguments));
    }

    std::unique_ptr<AST> visit(const GenericStructureDeclaration& node) const
    {
        return AST::create<GenericStructureDeclaration>(node.get_location(),
                                                        node.get_name());
    }

    std::unique_ptr<AST> visit(const GenericStructure& node) const
    {
        return AST::create<GenericStructure>(
            node.get_location(), node.get_name(), node.get_elements());
    }

    std::unique_ptr<AST> visit(const GenericClass& node) const
    {
        std::vector<std::reference_wrapper<const AST>> children;

        auto cloner = [this](const AST& node) { return node.accept(*this); };

        auto clone_of_arguments = node.get_argument_names();
        auto clone_of_member_variables =
            utils::get_transformed(node.get_member_variables(), cloner);
        auto clone_of_member_functions =
            utils::get_transformed(node.get_member_functions(), cloner);

        return AST::create<GenericClass>(node.get_location(), node.get_name(),
                                         clone_of_arguments,
                                         std::move(clone_of_member_variables),
                                         std::move(clone_of_member_functions));
    }

    std::unique_ptr<AST> visit(const TypeID& node) const
    {
        return AST::create<TypeID>(node.get_location(),
                                   node.get_expression().accept(*this));
    }

    std::unique_ptr<AST> visit(const Return& node) const
    {
        return AST::create<Return>(node.get_location(),
                                   node.get_expression().accept(*this));
    }

    std::unique_ptr<AST> visit(const CTypeDeclaration& node) const
    {
        return AST::create<CTypeDeclaration>(
            node.get_location(), node.get_name(), node.get_member_types());
    }
    std::unique_ptr<AST> visit(const CFunctionDeclaration& node) const
    {
        return AST::create<CFunctionDeclaration>(
            node.get_location(), node.get_name(), node.get_argument_types(),
            node.get_return_type());
    }
};

Cloner::Cloner() : impl(std::make_unique<ClonerImpl>()) {}

Cloner::~Cloner() = default;

std::unique_ptr<AST> Cloner::visit(const AST& node) const
{
    return node.accept(*impl);
}

}  // namespace clawn::ast