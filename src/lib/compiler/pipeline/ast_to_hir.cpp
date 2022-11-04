#include <iostream>
#include "include/ast/ast.hpp"
#include "include/ast/visitor.hpp"
#include "include/compiler/module/module.hpp"
#include "include/compiler/constants.hpp"
#include "include/compiler/pipeline.hpp"
#include "include/compiler/builtins.hpp"
#include "include/hir/hir.hpp"
#include "include/requirement/type.hpp"
#include "include/requirement/requirement.hpp"
#include "include/requirement/type_solver.hpp"
#include "include/utils/pattern_match.hpp"
#include "include/utils/transform.hpp"
#include "include/hierarchy/mangler.hpp"

namespace clawn::compiler
{
namespace visitors
{
//---------end forward declarations-------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------

class ASTToHIR
{
    private:
    std::unique_ptr<class ConverterImpl> impl;
    const ast::AST& ast_node;
    const std::shared_ptr<compiler::Module> clawn_module;
    const std::shared_ptr<requirement::TypeEnvironment> type_environment;

    public:
    ASTToHIR(const ast::AST& node,
             const std::shared_ptr<compiler::Module> clawn_module);
    std::unique_ptr<hir::HIR> visit() const;
    ~ASTToHIR();
};

class ConverterImpl : public ast::Visitor<std::unique_ptr<hir::HIR>>
{
    private:
    const std::shared_ptr<compiler::Module> clawn_module;
    const std::shared_ptr<requirement::TypeEnvironment> type_environment;
    const std::shared_ptr<requirement::TypeSolver> type_solver;

    public:
    ConverterImpl() = delete;
    ConverterImpl(std::shared_ptr<compiler::Module> clawn_module)
        : clawn_module(clawn_module),
          type_environment(clawn_module->get_type_environment()),
          type_solver(type_environment->get_solver())
    {
    }

    std::unique_ptr<hir::HIR> visit(const ast::Root& node) const
    {
        auto& program = node.get_children();
        std::vector<std::unique_ptr<hir::HIR>> result;
        for (auto& expression : program)
        {
            result.push_back(expression.get().accept(*this));
        }
        return hir::HIR::create<hir::Root>(
            requirement::Type::create<requirement::VoidType>(type_solver),
            node.get_location(), std::move(result));
    }

    std::unique_ptr<hir::HIR> visit(const ast::Integer& node) const
    {
        return hir::HIR::create<hir::Integer>(
            requirement::Type::create<requirement::IntegerType>(type_solver),
            node.get_location(), node.get_initial_value());
    }

    std::unique_ptr<hir::HIR> visit(const ast::Float& node) const
    {
        return hir::HIR::create<hir::Float>(
            requirement::Type::create<requirement::RealNumberType>(type_solver),
            node.get_location(), node.get_initial_value());
    }

    std::unique_ptr<hir::HIR> visit(const ast::List& node) const
    {
        auto location = node.get_location();
        auto initial_values = utils::get_transformed(
            node.get_initial_values(), [this](const auto& initial_value) {
                return initial_value.get().accept(*this);
            });
        auto element_type =
            requirement::Type::create<requirement::UnsolvedType>(type_solver);
        if (!initial_values.empty())
        {
            type_solver->bind(element_type, initial_values[0]->get_type(),
                              location);
        }
        for (auto& initial_value : initial_values)
        {
            if (initial_value->get_basic_info().get_id() ==
                initial_values.back()->get_basic_info().get_id())
            {
                continue;
            }
            type_solver->bind(initial_value->get_type(), element_type,
                              location);
        }
        requirement::StructureType::MemberTypes list_type_members;
        // list_type_members.push_back(element_type);
        list_type_members.push_back(
            std::make_pair("size", type_environment->get_integer_type()));
        list_type_members.push_back(std::make_pair(
            "allocated_size", type_environment->get_integer_type()));
        list_type_members.push_back(std::make_pair(
            "element_size", type_environment->get_integer_type()));
        list_type_members.push_back(
            std::make_pair("pointer", element_type->get_pointer_to()));
        auto list_type = requirement::Type::create<requirement::StructureType>(
            type_solver, builtins::STRUCTURE_LIST_NAME, list_type_members);

        // auto list_type =
        // requirement::Type::create<requirement::ListType>(type_solver,element_type);
        return hir::HIR::create<hir::List>(list_type, location,
                                           std::move(initial_values));
    }

    std::unique_ptr<hir::HIR> visit(const ast::String& node) const
    {
        return hir::HIR::create<hir::String>(
            type_environment->get_type_by_identifier("String")
                ->get_pointer_to(),
            node.get_location(), node.get_initial_value());
    }

    std::unique_ptr<hir::HIR> visit(const ast::Reference& node) const
    {
        auto refer_to = node.get_refer_to().accept(*this);
        return hir::HIR::create<hir::Reference>(
            requirement::Type::create<requirement::ReferenceType>(
                type_solver, refer_to->get_type()),
            node.get_location(), std::move(refer_to));
    }

    std::unique_ptr<hir::HIR> visit(const ast::Dereference& node) const
    {
        auto target = node.get_target().accept(*this);
        return hir::HIR::create<hir::Dereference>(
            requirement::Type::create<requirement::UnsolvedType>(type_solver),
            node.get_location(), std::move(target));
    }

    std::unique_ptr<hir::HIR> visit(const ast::SetResult& node) const
    {
        auto location = node.get_location();
        auto expression = node.get_expression().accept(*this);
        return hir::HIR::create<hir::SetResult>(
            expression->get_type(), location, std::move(expression));
    }

    std::unique_ptr<hir::HIR> visit(const ast::Variable& node) const
    {
        auto type = type_environment->get_type_by_identifier(node.get_name());
        auto new_type =
            requirement::Type::create<requirement::UnsolvedType>(type_solver);
        type_solver->bind(
            new_type, type,
            node.get_location());  // This may looks like useless, but useful
                                   // when monomorphizer works.
        auto variable = hir::HIR::create<hir::Variable>(
            new_type, node.get_name(), node.get_location());

        return variable;
    }

    std::unique_ptr<hir::HIR> visit(const ast::VariableDefinition& node) const
    {
        auto location = node.get_location();
        auto variable_name = node.get_name();
        auto initial_value = node.get_initial_value().accept(*this);

        auto variable_type =
            requirement::Type::create<requirement::UnsolvedType>(type_solver);

        type_environment->add_identifier_and_type(variable_name, variable_type);
        type_solver->bind(variable_type, initial_value->get_type(), location);
        auto variable = hir::HIR::create<hir::Variable>(
            variable_type, node.get_name(), location);

        auto assignment = hir::HIR::create<hir::Assignment>(
            variable_type, location, std::move(initial_value),
            std::move(variable));
        return assignment;
    }

    std::unique_ptr<hir::HIR> visit(const ast::GlobalVariable& node) const
    {
        return 0;
    }

    std::unique_ptr<hir::HIR> visit(const ast::Assignment& node) const
    {
        auto location = node.get_location();
        auto left_hand_side = node.get_left_hand_side().accept(*this);
        auto right_hand_side = node.get_right_hand_side().accept(*this);

        type_solver->bind(left_hand_side->get_type(),
                          right_hand_side->get_type(), location);

        auto type =
            requirement::Type::create<requirement::UnsolvedType>(type_solver);
        type_solver->bind(type, left_hand_side->get_type(), location);

        auto assignment = hir::HIR::create<hir::Assignment>(
            left_hand_side->get_type(), location, std::move(right_hand_side),
            std::move(left_hand_side), node.is_both_reference());
        return assignment;
    }

    std::unique_ptr<hir::HIR> visit(const ast::BinaryExpression& node) const
    {
        auto location = node.get_location();

        auto left_hand_side = node.get_left_hand_side().accept(*this);
        auto right_hand_side = node.get_right_hand_side().accept(*this);

        auto to_hir_operator_kind = [](const ast::OperatorKind& kind) {
            return hir::OperatorKind(int(kind));
        };
        auto type =
            requirement::Type::create<requirement::UnsolvedType>(type_solver);
        if (node.get_operator() == ast::OperatorKind::Addition ||
            node.get_operator() == ast::OperatorKind::Subtraction ||
            node.get_operator() == ast::OperatorKind::Multiplication)
        {
            type = left_hand_side->get_type();
        }

        if (node.get_operator() == ast::OperatorKind::Division)
        {
            type = type_environment->get_float_type();
        }
        if (node.get_operator() == ast::OperatorKind::GreaterThan ||
            node.get_operator() == ast::OperatorKind::GreaterThanOrEqual ||
            node.get_operator() == ast::OperatorKind::LessThan ||
            node.get_operator() == ast::OperatorKind::LessThanOrEqual ||
            node.get_operator() == ast::OperatorKind::Equal ||
            node.get_operator() == ast::OperatorKind::NotEqual ||
            node.get_operator() == ast::OperatorKind::And ||
            node.get_operator() == ast::OperatorKind::Or)
        {
            type = type_environment->get_boolean_type();
        }

        auto pair = std::make_pair(std::move(left_hand_side),
                                   std::move(right_hand_side));
        return hir::HIR::create<hir::BinaryExpression>(
            type, location, std::move(pair),
            to_hir_operator_kind(node.get_operator()));
    }

    std::unique_ptr<hir::HIR> visit(const ast::FunctionCall& node) const
    {
        auto location = node.get_location();

        std::vector<std::unique_ptr<hir::HIR>> arguments;
        std::vector<std::shared_ptr<requirement::Type>> arguments_types;
        for (auto&& argument : node.get_arguments())
        {
            auto argument_hir = argument.get().accept(*this);
            arguments_types.push_back(argument_hir->get_type());
            arguments.push_back(std::move(argument_hir));
        }

        auto function_obj = node.get_function().accept(*this);

        type_environment->register_instanciation(function_obj->get_type(),
                                                 arguments_types);

        return hir::HIR::create<hir::FunctionCall>(
            requirement::Type::create<requirement::UnsolvedType>(type_solver),
            location, std::move(function_obj), std::move(arguments));
    }

    std::unique_ptr<hir::HIR> visit(const ast::AccessElement& node) const
    {
        auto location = node.get_location();

        auto left_hand_side = node.get_left_hand_side().accept(*this);
        auto element_name = node.get_element_name();

        auto registered_type = type_environment->get_element_type(
            left_hand_side->get_type(), element_name);
        if (registered_type.has_value())
        {
            return hir::HIR::create<hir::AccessElement>(
                registered_type.value(), location, std::move(left_hand_side),
                element_name);
        }

        auto type =
            requirement::Type::create<requirement::UnsolvedType>(type_solver);
        type_environment->register_element_type(left_hand_side->get_type(),
                                                element_name, type);
        return hir::HIR::create<hir::AccessElement>(
            type, location, std::move(left_hand_side), element_name);
    }

    std::unique_ptr<hir::HIR> visit(const ast::AccessList& node) const
    {
        auto location = node.get_location();

        auto list = node.get_list().accept(*this);
        auto index = node.get_index().accept(*this);

        auto registered_type =
            type_environment->get_element_type(list->get_type());
        if (registered_type.has_value())
        {
            return hir::HIR::create<hir::AccessList>(registered_type.value(),
                                                     location, std::move(list),
                                                     std::move(index));
        }

        auto element_type =
            requirement::Type::create<requirement::UnsolvedType>(type_solver);
        type_environment->register_element_type(list->get_type(), element_type);
        return hir::HIR::create<hir::AccessList>(
            element_type, location, std::move(list), std::move(index));
    }

    std::unique_ptr<hir::HIR> visit(const ast::DeepCopy& node) const
    {
        throw std::logic_error("deep copy is not yet implemented or obsoleted");
    }

    std::unique_ptr<hir::HIR> visit(const ast::Block& node) const
    {
        auto location = node.get_location();
        auto type =
            requirement::Type::create<requirement::VoidType>(type_solver);

        std::vector<std::unique_ptr<hir::HIR>> body;
        for (auto& line : node.get_expressions())
        {
            auto hir = line.get().accept(*this);
            if (hir->is_type<hir::SetResult>() || hir->is_type<hir::Return>())
            {
                type = hir->get_type();
            }
            body.push_back(std::move(hir));
        }

        return hir::HIR::create<hir::Block>(type, location, std::move(body));
    }

    std::unique_ptr<hir::HIR> visit(const ast::Match& node) const
    {
        auto location = node.get_location();
        auto target = node.get_target().accept(*this);

        auto type =
            requirement::Type::create<requirement::UnsolvedType>(type_solver);
        auto patterns = utils::get_transformed(
            node.get_patterns(), [this, type, location](const auto& pattern) {
                auto expression = pattern.second.get().accept(*this);
                type_solver->bind(type, expression->get_type(), location);
                return std::make_pair(pattern.first, std::move(expression));
            });

        if (node.get_default_case().has_value())
        {
            auto default_case =
                node.get_default_case().value().get().accept(*this);
            type_solver->bind(type, default_case->get_type(), location);
            return hir::HIR::create<hir::Match>(
                type, location, std::move(target), std::move(patterns),
                std::move(default_case));
        }
        else
        {
            return hir::HIR::create<hir::Match>(
                type, location, std::move(target), std::move(patterns));
        }
    }

    std::unique_ptr<hir::HIR> visit(const ast::If& node) const
    {
        auto location = node.get_location();

        auto condition = node.get_condition().accept(*this);
        auto body = node.get_body().accept(*this);
        auto else_body_ast = node.get_else_body();

        auto type =
            requirement::Type::create<requirement::UnsolvedType>(type_solver);
        type_solver->bind(type, body->get_type(), location);

        if (else_body_ast.has_value())
        {
            auto else_body = else_body_ast.value().get().accept(*this);
            type_solver->bind(else_body->get_type(), body->get_type(),
                              location);
            return hir::HIR::create<hir::If>(
                type, location, std::move(condition), std::move(body),
                std::move(else_body));
        }
        else
        {
            return hir::HIR::create<hir::If>(
                type, location, std::move(condition), std::move(body));
        }
    }

    std::unique_ptr<hir::HIR> visit(const ast::Repeat& node) const
    {
        auto location = node.get_location();
        auto integer_type = type_environment->get_integer_type();
        auto condition = node.get_condition().accept(*this);
        auto counter = hir::HIR::create<hir::Variable>(
            integer_type, node.get_counter_name(), location);
        auto counter_ = hir::HIR::create<hir::Variable>(
            integer_type, node.get_counter_name(), location);
        std::vector<std::unique_ptr<hir::HIR>> body;
        auto body_content = node.get_body().accept(*this);
        auto body_content_type = body_content->get_type();
        body.push_back(hir::HIR::create<hir::SetResult>(
            body_content_type, location, std::move(body_content)));
        auto incremented = hir::HIR::create<hir::BinaryExpression>(
            type_environment->get_integer_type(), node.get_location(),
            std::make_pair(std::move(counter_), hir::HIR::create<hir::Integer>(
                                                    integer_type, location, 1)),
            hir::OperatorKind::Addition);
        auto assignment = hir::HIR::create<hir::Assignment>(
            integer_type, location, std::move(incremented), std::move(counter));
        body.push_back(std::move(assignment));
        auto body_type = body[0]->get_type();
        return hir::HIR::create<hir::Loop>(
            body[0]->get_type(), location, std::move(condition),
            hir::HIR::create<hir::Block>(body[0]->get_type(),
                                         node.get_location(), std::move(body)));
    }

    std::unique_ptr<hir::HIR> visit(
        const ast::GenericFunctionDeclaration& node) const
    {
        /*
        auto location = node.get_location();
        auto function_name = node.get_name();
        std::vector<std::shared_ptr<requirement::Type>> arguments_types;
        for (auto& name : node.get_arguments())
        {
            auto arguments_type =
                requirement::Type::create<requirement::UnsolvedType>(type_solver);
            arguments_types.push_back(arguments_type);
        }
        auto return_type =
            requirement::Type::create<requirement::UnsolvedType>(type_solver);
        auto body = hir::HIR::create<hir::Block>(
            return_type, location, std::vector<std::unique_ptr<hir::HIR>>());

        auto declaration_type =
            requirement::Type::create<requirement::FunctionType>(
                type_solver, node.get_name(), arguments_types,
                node.get_arguments(), return_type);

        type_environment->add_identifier_and_type(node.get_name(),
                                                  declaration_type);
        */
        auto temporary_type =
            requirement::Type::create<requirement::UnsolvedType>(
                type_solver);  // this will be solved by processing
                               // ast::GenericUnion node.
        type_environment->add_identifier_and_type(node.get_name(),
                                                  temporary_type);

        return hir::HIR::create<hir::Block>(
            temporary_type, node.get_location(),
            std::vector<std::unique_ptr<hir::HIR>>());  // empty hir
    }

    std::unique_ptr<hir::HIR> visit(const ast::GenericFunction& node) const
    {
        auto location = node.get_location();

        auto function_name = node.get_name();
        std::vector<std::shared_ptr<requirement::Type>> parameter_types;
        auto& parameters = node.get_parameter_names();
        for (auto&& parameter_name : parameters)
        {
            auto parameter_type =
                requirement::Type::create<requirement::UnsolvedType>(
                    type_solver);
            type_environment->add_identifier_and_type(parameter_name,
                                                      parameter_type);
            auto requirement = node.get_requirement(parameter_name);
            if (requirement.has_value())
            {
                clawn_module->add_requirement(requirement.value(),
                                              parameter_type, location);
            }
            parameter_types.push_back(parameter_type);
        }
        auto body = node.get_body().accept(*this);
        auto return_type = requirement::Type::create<requirement::UnsolvedType>(
            type_solver);  // body->get_type();
        auto function_type =
            requirement::Type::create<requirement::FunctionType>(
                type_solver, function_name, parameter_types, parameters,
                return_type);

        auto declaration_type =
            type_environment->get_type_by_identifier(function_name);
        type_solver->bind(declaration_type, function_type, location);

        return hir::HIR::create<hir::Function>(
            function_type, location, node.get_name(), std::move(body));
    }

    std::unique_ptr<hir::HIR> visit(
        const ast::UnionConstructorDeclaration& node) const
    {
        auto temporary_type =
            requirement::Type::create<requirement::UnsolvedType>(
                type_solver);  // this will be solved by processing
                               // ast::GenericUnion node.
        type_environment->add_identifier_and_type(node.get_constructor_name(),
                                                  temporary_type);

        return hir::HIR::create<hir::Block>(
            temporary_type, node.get_location(),
            std::vector<std::unique_ptr<hir::HIR>>());  // empty hir
    }

    std::unique_ptr<hir::HIR> visit(const ast::GenericUnion& node) const
    {
        auto constructor_names = node.get_constructor_names();
        auto location = node.get_location();
        std::vector<std::unique_ptr<hir::HIR>> constructors;
        for (auto& pair : constructor_names)
        {
            auto tag_name_of_constructor = pair.first;
            auto constructor_name = pair.second;
            if (!type_environment->exists(constructor_name))
            {
                continue;
            }
            auto argument_name = "argument_of_constructor_" + constructor_name;
            auto argument_type =
                requirement::Type::create<requirement::UnsolvedType>(
                    type_solver);
            auto argument = hir::HIR::create<hir::Variable>(
                requirement::Type::create<requirement::UnsolvedType>(
                    type_solver),
                argument_name, location);

            auto requirement = node.get_requirement(tag_name_of_constructor);
            if (requirement.has_value())
            {
                clawn_module->add_requirement(requirement.value(),
                                              argument_type, location);
            }

            std::map<std::string, std::shared_ptr<requirement::Type>> tag_types;
            for (auto& tag_name : node.get_tag_names())
            {
                if (tag_name == tag_name_of_constructor)
                {
                    tag_types[tag_name] = argument_type;
                }
                else
                {
                    tag_types[tag_name] =
                        requirement::Type::create<requirement::UnsolvedType>(
                            type_solver);
                }
            }
            auto union_type = requirement::Type::create<requirement::UnionType>(
                type_solver, node.get_name(), tag_types, type_solver);

            auto constructor_type =
                requirement::Type::create<requirement::FunctionType>(
                    type_solver, constructor_name, std::vector{argument_type},
                    std::vector{argument_name},
                    requirement::Type::create<requirement::UnsolvedType>(
                        type_solver));  // union_type);

            type_solver->bind(
                type_environment->get_type_by_identifier(constructor_name),
                constructor_type, location);

            auto construction = hir::HIR::create<hir::UnionConstruction>(
                union_type, node.get_location(),
                std::make_pair(tag_name_of_constructor, std::move(argument)));

            constructors.push_back(hir::HIR::create<hir::Function>(
                constructor_type, location, constructor_name,
                std::move(construction)));
        }
        return hir::HIR::create<hir::Block>(
            requirement::Type::create<requirement::UnsolvedType>(type_solver),
            node.get_location(), std::move(constructors));
    }

    std::unique_ptr<hir::HIR> visit(const ast::Construction& node) const
    {
        auto location = node.get_location();

        std::vector<std::unique_ptr<hir::HIR>> arguments;
        std::vector<std::shared_ptr<requirement::Type>> arguments_types;
        for (auto&& pair : node.get_arguments())
        {
            auto argument_hir = pair.second.get().accept(*this);
            arguments_types.push_back(argument_hir->get_type());
            arguments.push_back(std::move(argument_hir));
        }

        auto constructor_obj = node.get_constructor().accept(*this);
        type_environment->register_instanciation(constructor_obj->get_type(),
                                                 arguments_types);

        return hir::HIR::create<hir::FunctionCall>(
            requirement::Type::create<requirement::UnsolvedType>(type_solver),
            location, std::move(constructor_obj), std::move(arguments));
    }

    std::unique_ptr<hir::HIR> visit(
        const ast::GenericStructureDeclaration& node) const
    {
        auto temporary_type =
            requirement::Type::create<requirement::UnsolvedType>(type_solver);
        type_environment->add_identifier_and_type(
            node.get_name(),
            temporary_type);  // register actuall structure type.
        type_environment->add_identifier_and_type(
            node.get_constructor_name(),
            requirement::Type::create<requirement::UnsolvedType>(type_solver));
        // register constructor type of the structure
        return hir::HIR::create<hir::Block>(
            temporary_type, node.get_location(),
            std::vector<std::unique_ptr<hir::HIR>>());
    }

    std::unique_ptr<hir::HIR> visit(const ast::GenericStructure& node) const
    {
        auto location = node.get_location();
        auto member_names = node.get_element_names();
        requirement::StructureType::MemberTypes member_types;
        for (auto member_name : member_names)
        {
            member_types.push_back(std::make_pair(
                member_name,
                requirement::Type::create<requirement::UnsolvedType>(
                    type_solver)));
        }

        auto structure_type =
            requirement::Type::create<requirement::StructureType>(
                type_solver, node.get_name(), member_types);
        auto declaration_type =
            type_environment->get_type_by_identifier(node.get_name());
        type_solver->bind(declaration_type, structure_type,
                          node.get_location());

        auto& structure_type_content =
            structure_type->as<requirement::StructureType>();
        auto constructor_arguments = utils::get_transformed(
            node.get_element_names(),
            [&node, structure_type](const std::string& name) {
                return std::make_pair(
                    "arg_" +
                        hierarchy::Hierarchy::create(
                            hierarchy::Hierarchy::create(node.get_name()), name)
                            .to_string(),
                    structure_type->as<requirement::StructureType>()
                        .get_element_type_without_solving(name));
            });
        auto constructor_argument_names = utils::get_transformed(
            constructor_arguments, [](const auto& pair) { return pair.first; });

        std::vector<std::shared_ptr<requirement::Type>>
            constructor_argument_types;
        std::vector<std::pair<std::string, std::unique_ptr<hir::HIR>>>
            arguments_of_construction;

        for (auto&& constructor_argument : constructor_arguments)
        {
            auto constructor_argument_name = constructor_argument.first;
            auto constructor_argument_type = constructor_argument.second;
            // requirement::Type::create<requirement::UnsolvedType>(type_solver);
            type_environment->add_identifier_and_type(
                constructor_argument_name, constructor_argument_type);
            constructor_argument_types.push_back(constructor_argument_type);
            auto member_name =
                hierarchy::demangle(constructor_argument_name).first;
            auto argument_type =
                requirement::Type::create<requirement::UnsolvedType>(
                    type_solver);
            auto argument = hir::HIR::create<hir::Variable>(
                argument_type, constructor_argument_name, location);

            auto requirement = node.get_requirement(member_name);
            if (requirement.has_value())
            {
                clawn_module->add_requirement(requirement.value(),
                                              argument_type, location);
            }

            auto member_type =
                structure_type_content.get_element_type_without_solving(
                    member_name);
            //                structure_type_content.get_member_types_without_solving().at(
            //                    member_name);
            type_solver->bind(member_type, argument->get_type(), location);
            arguments_of_construction.push_back(
                std::make_pair(member_name, std::move(argument)));
        }

        auto construction = hir::HIR::create<hir::Construction>(
            structure_type, node.get_location(),
            std::move(arguments_of_construction));

        auto constructor_type =
            requirement::Type::create<requirement::FunctionType>(
                type_solver, node.get_constructor_name(),
                constructor_argument_types, constructor_argument_names,
                requirement::Type::create<requirement::UnsolvedType>(
                    type_solver));  // structure_type);

        type_solver->bind(type_environment->get_type_by_identifier(
                              node.get_constructor_name()),
                          constructor_type, node.get_location());

        return hir::HIR::create<hir::Function>(constructor_type, location,
                                               node.get_constructor_name(),
                                               std::move(construction));
    }

    std::unique_ptr<hir::HIR> visit(const ast::GenericClass& node) const
    {
        // hir::HIR::create<hir::Construction>();
        //  node.get_member_functions();
        return 0;
    }

    std::unique_ptr<hir::HIR> visit(const ast::TypeID& node) const { return 0; }

    std::unique_ptr<hir::HIR> visit(const ast::Return& node) const
    {
        auto location = node.get_location();
        auto expression = node.get_expression().accept(*this);
        return hir::HIR::create<hir::Return>(expression->get_type(), location,
                                             std::move(expression));
    }

    std::vector<std::string> split(std::string text, std::string splitter) const
    {
        std::vector<std::string> res;
        std::string::size_type index = 0;
        while (index != std::string::npos)
        {
            std::string::size_type found_index = text.find(splitter, index);
            if (found_index == std::string::npos)
            {
                res.push_back(text.substr(index));
                break;
            }
            else
            {
                res.push_back(text.substr(index, found_index - index));
            }
            index = found_index + splitter.size();
        }
        return res;
    }

    std::shared_ptr<requirement::Type> create_C_type(
        std::shared_ptr<location::Location> location,
        const ast::CTypeDescriptor& description) const
    {
        auto last_text = description.back();
        auto parsed = split(last_text, "SIZE_");
        auto last_type_text = parsed.front();
        std::optional<std::shared_ptr<requirement::Type>> last_type;
        auto mangled_last_type_text =
            hierarchy::mangle(last_type_text, hierarchy::Hierarchy::get_root());
        if ((!type_environment->exists(last_type_text)) &&
            !type_environment->exists(mangled_last_type_text))
        {
            if (last_type_text == "__C_VOID__")
            {
                last_type = requirement::Type::create<requirement::VoidType>(
                    type_solver);
            }
            else
            {
                auto last_type_size = std::stoi(parsed.back());
                auto last_type_kind = requirement::CPrimitiveType::UNKNOWN;

                if (last_type_text == "__C_BOOL__")
                {
                    last_type_kind = requirement::CPrimitiveType::BOOL;
                }
                if (last_type_text == "__C_CHAR__")
                {
                    last_type_kind = requirement::CPrimitiveType::CHAR;
                }
                if (last_type_text == "__C_DOUBLE__")
                {
                    last_type_kind = requirement::CPrimitiveType::DOUBLE;
                }
                if (last_type_text == "__C_FLOAT__")
                {
                    last_type_kind = requirement::CPrimitiveType::FLOAT;
                }
                if (last_type_text == "__C_INT__")
                {
                    last_type_kind = requirement::CPrimitiveType::INT;
                }
                if (last_type_text == "__C_LONG__")
                {
                    last_type_kind = requirement::CPrimitiveType::LONG;
                }
                if (last_type_text == "__C_LONGDOUBLE__")
                {
                    last_type_kind = requirement::CPrimitiveType::LONGDOUBLE;
                }
                if (last_type_text == "__C_LONGLONG__")
                {
                    last_type_kind = requirement::CPrimitiveType::LONGLONG;
                }
                if (last_type_text == "__C_SCHAR__")
                {
                    last_type_kind = requirement::CPrimitiveType::SCHAR;
                }
                if (last_type_text == "__C_SHORT__")
                {
                    last_type_kind = requirement::CPrimitiveType::SHORT;
                }
                if (last_type_text == "__C_UCHAR__")
                {
                    last_type_kind = requirement::CPrimitiveType::UCHAR;
                }
                if (last_type_text == "__C_UINT__")
                {
                    last_type_kind = requirement::CPrimitiveType::UINT;
                }
                if (last_type_text == "__C_ULONG__")
                {
                    last_type_kind = requirement::CPrimitiveType::ULONG;
                }
                if (last_type_text == "__C_ULONGLONG__")
                {
                    last_type_kind = requirement::CPrimitiveType::ULONGLONG;
                }
                if (last_type_text == "__C_USHORT__")
                {
                    last_type_kind = requirement::CPrimitiveType::USHORT;
                }
                if (last_type_text == "__C_WCHAR__")
                {
                    last_type_kind = requirement::CPrimitiveType::WCHAR;
                }
                last_type = requirement::Type::create<requirement::CType>(
                    type_solver, requirement::CType::TypeInfo(last_type_kind,
                                                              last_type_size));
            }
        }
        else
        {
            if (type_environment->exists(last_type_text))
            {
                last_type =
                    type_environment->get_type_by_identifier(last_type_text);
            }
            if (type_environment->exists(mangled_last_type_text))
            {
                last_type = type_environment->get_type_by_identifier(
                    mangled_last_type_text);
            }
        }

        if (last_type.has_value())
        {
            auto declaration_copy = description;
            declaration_copy.pop_back();

            for (auto& text : declaration_copy)
            {
                if (text != "__PTR__")
                {
                    throw Error::invalid_C_type_description(location, text);
                }
                last_type = last_type.value()->get_pointer_to();
            }

            return last_type.value();
        }
        else
        {
            throw Error::invalid_C_type_description(location, "");
        }
    }

    std::unique_ptr<hir::HIR> visit(const ast::CTypeDeclaration& node) const
    {
        auto& name = node.get_name();
        auto& members = node.get_member_types();
        requirement::StructureType::MemberTypes member_types;

        for (auto& member : members)
        {
            auto member_name = member.first;
            auto member_type =
                create_C_type(node.get_location(), member.second);
            member_types.push_back(std::make_pair(member_name, member_type));
        }

        auto declared_type =
            requirement::Type::create<requirement::StructureType>(
                type_solver, name, member_types);
        type_environment->add_identifier_and_type(name, declared_type);
        type_environment->add_identifier_and_type(
            hierarchy::mangle(name, hierarchy::Hierarchy::get_root()),
            declared_type);

        return hir::HIR::create<hir::Block>(
            requirement::Type::create<requirement::UnsolvedType>(type_solver),
            node.get_location(), std::vector<std::unique_ptr<hir::HIR>>());
    }
    std::unique_ptr<hir::HIR> visit(const ast::CFunctionDeclaration& node) const
    {
        auto name = node.get_name();
        if ((name == "_OSSwapInt16") || (name == "_OSSwapInt32") ||
            (name == "_OSSwapInt64") || (name == "__sputc") ||
            (name == "alloca") || (name == "zopen"))
        {
            // These functions are not linked for some reason on MacOS, so
            // compiler ignores them
            return hir::HIR::create<hir::Block>(
                requirement::Type::create<requirement::UnsolvedType>(
                    type_solver),
                node.get_location(), std::vector<std::unique_ptr<hir::HIR>>());
        }

        auto location = node.get_location();
        auto argument_types = utils::get_transformed(
            node.get_argument_types(),
            [this, location](const auto& description) {
                return create_C_type(location, description);
            });
        std::vector<std::string> argument_names;
        for (size_t i = 0; i < argument_types.size(); i += 1)
        {
            argument_names.push_back(std::to_string(i));
        }

        auto return_type = create_C_type(location, node.get_return_type());

        auto function_type =
            requirement::Type::create<requirement::FunctionType>(
                type_solver, name, argument_types, argument_names, return_type,
                /*is_polymorphic=*/false);
        auto empty = hir::HIR::create<hir::Block>(
            return_type, node.get_location(),
            std::vector<std::unique_ptr<hir::HIR>>());
        auto declaration = hir::HIR::create<hir::Function>(
            function_type, location, name, std::move(empty),
            /*declaration_only=*/true);

        return declaration;
    }
};

ASTToHIR::ASTToHIR(const ast::AST& node,
                   const std::shared_ptr<compiler::Module> clawn_module)
    : ast_node(node),
      clawn_module(clawn_module),
      type_environment(clawn_module->get_type_environment())
{
    impl = std::make_unique<ConverterImpl>(clawn_module);
}

std::unique_ptr<hir::HIR> ASTToHIR::visit() const
{
    auto hir = ast_node.accept(*impl);
    // type_environment->set_hir(std::move(hir));
    return hir;
}

ASTToHIR::~ASTToHIR() = default;

}  // namespace visitors

namespace pipeline
{
std::unique_ptr<hir::HIR> ast_to_hir(
    const ast::AST& node, const std::shared_ptr<compiler::Module> clawn_module)
{
    return visitors::ASTToHIR(node, clawn_module).visit();
}
}  // namespace pipeline

}  // namespace clawn::compiler