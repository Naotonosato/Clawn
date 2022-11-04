#include <algorithm>
#include "include/ast/ast.hpp"
#include "include/utils/transform.hpp"
#include "include/hierarchy/mangler.hpp"
#include <cassert>

namespace clawn::ast
{
UnionConstructorInfo extract_union_constructor_info(const std::string& text)
{
    std::string union_name;
    std::string tag_name;
    bool union_name_ended = false;
    for (auto character : text)
    {
        if (character == ' ')
        {
            continue;
        }
        if (character == '#')
        {
            union_name_ended = true;
            continue;
        }
        if (!union_name_ended)
        {
            union_name += character;
        }
        else
        {
            tag_name += character;
        }
    }
    return UnionConstructorInfo(union_name, tag_name);
}

const unsigned long NodeBase::get_id() const { return id; }

const std::shared_ptr<location::Location> NodeBase::get_location() const
{
    return location;
}

const std::vector<std::reference_wrapper<const AST>> Root::get_children() const
{
    return utils::get_transformed(
        children,
        [](const std::unique_ptr<AST>& ptr) { return std::cref(*ptr); });
}

const int64_t Integer::get_initial_value() const { return initial_value; }

const double Float::get_initial_value() const { return initial_value; }

const std::vector<std::reference_wrapper<const AST>> List::get_initial_values()
    const
{
    return utils::get_transformed(
        initial_values,
        [](const std::unique_ptr<AST>& ptr) { return std::cref(*ptr); });
}

const std::string& String::get_initial_value() const { return initial_value; }

const AST& Reference::get_refer_to() const { return *refer_to; }

const AST& Dereference::get_target() const { return *target; }

const std::string& Variable::get_name() const { return name; }

const std::string& VariableDefinition::get_name() const { return name; }

const AST& VariableDefinition::get_initial_value() const
{
    return *initial_value;
}

const std::string GlobalVariable::get_name() const { return name; }

const AST& GlobalVariable::get_initial_value() const { return *initial_value; }
/*
const std::optional<std::reference_wrapper<Node>>&
UninitializedVariableNode::get_initial_value() const {
    if (initial_value.has_value()) {
        return *initial_value.value();
    }
    return std::nullopt;
}
*/
const AST& SetResult::get_expression() const { return *expression; }

const AST& Assignment::get_left_hand_side() const { return *left_hand_side; }

const AST& Assignment::get_right_hand_side() const { return *right_hand_side; }

bool Assignment::is_both_reference() const { return _is_both_reference; }

std::string Operator::get_name() const { return name; }

const OperatorKind BinaryExpression::get_operator() const
{
    return operator_kind;
}

const AST& BinaryExpression::get_left_hand_side() const
{
    return *left_hand_side;
}

const AST& BinaryExpression::get_right_hand_side() const
{
    return *right_hand_side;
}

const AST& FunctionCall::get_function() const { return *function; }

void FunctionCall::set_function(std::unique_ptr<AST>&& new_function)
{
    old_functions.push_back(std::move(function));
    function = std::move(new_function);
}

const std::vector<std::reference_wrapper<const AST>>
FunctionCall::get_arguments() const
{
    return utils::get_transformed(
        arguments, [](const auto& ptr) { return std::cref(*ptr); });
}

const AST& AccessElement::get_left_hand_side() const { return *left_hand_side; }

const std::string& AccessElement::get_element_name() const
{
    return element_name;
}

const AST& AccessList::get_list() const { return *list; }

const AST& AccessList::get_index() const { return *index; }

const AST& DeepCopy::get_left_hand_side() const { return *left_hand_side; }

const AST& DeepCopy::get_right_hand_side() const { return *right_hand_side; }

const std::vector<std::reference_wrapper<const AST>> Block::get_expressions()
    const
{
    return utils::get_transformed(
        expressions, [](const auto& ptr) { return std::cref(*ptr); });
}

const AST& Match::get_target() const { return *target; }

const std::vector<std::pair<std::string, std::reference_wrapper<const AST>>>
Match::get_patterns() const
{
    std::vector<std::pair<std::string, std::reference_wrapper<const AST>>>
        result;
    for (auto& pattern : patterns)
    {
        result.push_back(
            std::make_pair(pattern.first, std::cref(*pattern.second)));
    }
    return result;
}

std::optional<std::reference_wrapper<const AST>> Match::get_default_case() const
{
    if (default_case.has_value())
    {
        return {*default_case.value()};
    }
    return std::nullopt;
}

const AST& If::get_condition() const { return *condition; }

const AST& If::get_body() const { return *body; }

const std::optional<std::reference_wrapper<const AST>> If::get_else_body() const
{
    if (else_body.has_value())
    {
        return *else_body.value();
    }
    return std::nullopt;
}

const std::string& Repeat::get_counter_name() const { return counter_name; }

const AST& Repeat::get_condition() const { return *condition; }

const AST& Repeat::get_body() const { return *body; }

const std::string& GenericFunctionDeclaration::get_name() const { return name; }

const std::string& GenericFunction::get_name() const { return name; }

const std::vector<std::string> GenericFunction::get_parameter_names() const
{
    return utils::get_transformed(parameters,
                                  [](const auto& pair) { return pair.first; });
}

const Parameters& GenericFunction::get_parameters() const { return parameters; }

const std::optional<std::shared_ptr<requirement::Requirement>>
GenericFunction::get_requirement(const std::string& parameter_name) const
{
    for (auto& parameter : parameters)
    {
        if (parameter.first == parameter_name)
        {
            return parameter.second;
        }
    }
    throw std::runtime_error("There is no such parameter:" + parameter_name);
}

const AST& GenericFunction::get_body() const { return *body; }

const std::string UnionConstructorDeclaration::get_union_name() const
{
    return union_name;
}
const std::string UnionConstructorDeclaration::get_tag_name() const
{
    return tag_name;
}
const std::string UnionConstructorDeclaration::get_constructor_name() const
{
    return constructor_name;
}

const std::string GenericUnion::get_name() const { return name; }

const std::vector<std::string> GenericUnion::get_tag_names() const
{
    std::vector<std::string> tag_names;
    for (auto& tag : tags)
    {
        tag_names.push_back(tag.first);
    }
    return tag_names;
}

const Parameters& GenericUnion::get_tags() const { return tags; }

const std::optional<std::shared_ptr<requirement::Requirement>>
GenericUnion::get_requirement(const std::string& tag_name) const
{
    for (auto& tag : tags)
    {
        if (tag.first == tag_name)
        {
            return tag.second;
        }
    }
    throw std::runtime_error("There's no tag: " + tag_name + " in " + name);
}

const std::map<std::string, std::string> GenericUnion::get_constructor_names()
    const
{
    std::map<std::string, std::string> constructor_names;
    for (auto& tag : tags)
    {
        auto tag_name = tag.first;
        constructor_names.insert(
            std::make_pair(tag_name, name + "#" + tag_name));
    }
    return constructor_names;
}

const std::string& GenericStructureDeclaration::get_name() const
{
    return name;
}

const std::string GenericStructureDeclaration::get_constructor_name() const
{
    return hierarchy::mangle("constructor@" + hierarchy::demangle(name).first,
                             hierarchy::demangle(name).second);
}

const std::string& GenericStructure::get_name() const { return name; }

const std::vector<std::string> GenericStructure::get_element_names() const
{
    std::vector<std::string> element_names;
    for (auto& element : elements)
    {
        element_names.push_back(element.first);
    }
    return element_names;
}

const Parameters& GenericStructure::get_elements() const { return elements; }

const std::optional<std::shared_ptr<requirement::Requirement>>
GenericStructure::get_requirement(const std::string& element_name) const
{
    for (auto& element : elements)
    {
        if (element.first == element_name)
        {
            return element.second;
        }
    }
    throw std::runtime_error("There's no element: " + element_name + " in " +
                             name);
}

const std::string GenericStructure::get_constructor_name() const
{
    return hierarchy::mangle("constructor@" + hierarchy::demangle(name).first,
                             hierarchy::demangle(name).second);
}

const AST& Construction::get_constructor() const { return *constructor; }

const std::vector<std::pair<std::string, std::reference_wrapper<const AST>>>
Construction::get_arguments() const
{
    std::vector<std::pair<std::string, std::reference_wrapper<const AST>>>
        result;
    for (auto& pair : arguments)
    {
        result.push_back(std::make_pair(pair.first, std::cref(*pair.second)));
    }
    return result;
}

const std::string& GenericClass::get_name() const { return name; }

const std::vector<std::string>& GenericClass::get_argument_names() const
{
    return arguments;
}

const std::vector<std::reference_wrapper<const AST>>
GenericClass::get_member_variables() const
{
    return utils::get_transformed(
        member_variables, [](const auto& ptr) { return std::cref(*ptr); });
}

const std::vector<std::reference_wrapper<const AST>>
GenericClass::get_member_functions() const
{
    return utils::get_transformed(
        member_functions, [](const auto& ptr) { return std::cref(*ptr); });
}

const AST& TypeID::get_expression() const { return *expression; }

const AST& Return::get_expression() const { return *expression; }

const std::string& CTypeDeclaration::get_name() const { return name; }

const std::vector<std::pair<std::string, CTypeDescriptor>>&
CTypeDeclaration::get_member_types() const
{
    return member_types;
}

const std::string& CFunctionDeclaration::get_name() const { return name; }

const std::vector<CTypeDescriptor>& CFunctionDeclaration::get_argument_types()
    const
{
    return argument_types;
}

const CTypeDescriptor& CFunctionDeclaration::get_return_type() const
{
    return return_type;
}

}  // namespace clawn::ast