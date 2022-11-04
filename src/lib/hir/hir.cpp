#include "include/requirement/type.hpp"
#include "include/requirement/type_solver.hpp"
#include "include/hir/hir.hpp"
#include "include/hir/visitor.hpp"
#include "include/location/location.hpp"
#include "include/utils/transform.hpp"
#include "include/hierarchy/mangler.hpp"

namespace clawn::hir
{
HIRID HIRBase::get_id() const { return id; }

const std::shared_ptr<requirement::Type> HIRBase::get_type() const
{
    return type->get_type_solver()->solve(type);
}

const std::shared_ptr<requirement::Type> HIRBase::get_type(
    requirement::TypeSolver& solver) const
{
    return solver.solve(type);
}

const std::shared_ptr<requirement::Type> HIRBase::get_type_without_solving()
    const
{
    return type;
}

const std::shared_ptr<location::Location> HIRBase::get_location() const
{
    return location;
}

const std::vector<std::reference_wrapper<const HIR>> HIRBase::get_children()
    const
{
    throw std::invalid_argument("get_children");
    return {};
}

Root::Root(std::shared_ptr<requirement::Type> type,
           const std::shared_ptr<location::Location> location,
           std::vector<std::unique_ptr<HIR>>&& program)
    : HIRBase(type, location), program(std::move(program))
{
}

const std::vector<std::reference_wrapper<const HIR>> Root::get_children() const
{
    return utils::get_transformed(
        program, [](const auto& ptr) { return std::cref(*ptr); });
}

void Root::insert(std::vector<std::unique_ptr<HIR>>&& new_hirs)
{
    program.insert(program.end(), std::make_move_iterator(new_hirs.begin()),
                   std::make_move_iterator(new_hirs.end()));
}

std::vector<std::unique_ptr<HIR>>& Root::get_hirs() { return program; }

Integer::Integer(std::shared_ptr<requirement::Type> type,
                 const std::shared_ptr<location::Location> location,
                 int64_t initial_value)
    : HIRBase(type, location), initial_value(initial_value)
{
}

const int64_t Integer::get_initial_value() const { return initial_value; }

const std::vector<std::reference_wrapper<const HIR>> Integer::get_children()
    const
{
    return {};
}

Float::Float(std::shared_ptr<requirement::Type> type,
             const std::shared_ptr<location::Location> location,
             long double initial_value)
    : HIRBase(type, location), initial_value(initial_value)
{
}

const long double Float::get_initial_value() const { return initial_value; }

const std::vector<std::reference_wrapper<const HIR>> Float::get_children() const
{
    return {};
}

String::String(std::shared_ptr<requirement::Type> type,
               const std::shared_ptr<location::Location> location,
               const std::string& initial_value)
    : HIRBase(type, location), initial_value(initial_value)
{
}

const std::string& String::get_initial_value() const { return initial_value; }

const std::vector<std::reference_wrapper<const HIR>> String::get_children()
    const
{
    return {};
}

const HIR& Reference::get_refer_to() const { return *refer_to; }

const std::vector<std::reference_wrapper<const HIR>> Reference::get_children()
    const
{
    return {std::cref(*refer_to)};
}

const HIR& Dereference::get_target() const { return *target; }

const std::vector<std::reference_wrapper<const HIR>> Dereference::get_children()
    const
{
    return {std::cref(*target)};
}

List::List(const std::shared_ptr<requirement::Type> type,
           const std::shared_ptr<location::Location> location,
           std::vector<std::unique_ptr<HIR>>&& initial_values)
    : HIRBase(type, location), initial_values(std::move(initial_values))
{
}

const std::vector<std::reference_wrapper<const HIR>> List::get_initial_values()
    const
{
    return utils::get_transformed(
        initial_values, [](const auto& ptr) { return std::cref(*ptr); });
}

const std::vector<std::reference_wrapper<const HIR>> List::get_children() const
{
    return get_initial_values();
}

Variable::Variable(std::shared_ptr<requirement::Type> type,
                   const std::string& name,
                   const std::shared_ptr<location::Location> location)
    : HIRBase(type, location), name(name)
{
}

const std::string& Variable::get_name() const { return name; }

const std::vector<std::reference_wrapper<const HIR>> Variable::get_children()
    const
{
    return {};
}

bool Variable::is_global() const
{
    return hierarchy::Hierarchy::get_root() ==
           hierarchy::demangle(get_name()).second;
}

Function::Function(std::shared_ptr<requirement::Type> type,
                   const std::shared_ptr<location::Location> location,
                   const std::string& name, std::unique_ptr<HIR>&& body,
                   bool is_declaration_only)
    : HIRBase(type, location),
      body(std::move(body)),
      name(name),
      _is_declaration_only(is_declaration_only)
{
}

const std::string& Function::get_name() const { return name; }

const HIR& Function::get_body() const { return *body; }

const std::vector<std::reference_wrapper<const HIR>> Function::get_children()
    const
{
    return {std::cref(*body)};
}

bool Function::is_declaration_only() const { return _is_declaration_only; }

Assignment::Assignment(const std::shared_ptr<requirement::Type> type,
                       const std::shared_ptr<location::Location> location,
                       std::unique_ptr<HIR>&& right_hand_side,
                       std::unique_ptr<HIR>&& left_hand_side,
                       bool _is_both_reference)
    : HIRBase(type, location),
      right_hand_side(std::move(right_hand_side)),
      left_hand_side(std::move(left_hand_side)),
      _is_both_reference(_is_both_reference)
{
}

const HIR& Assignment::get_right_hand_side() const { return *right_hand_side; }

const HIR& Assignment::get_left_hand_side() const { return *left_hand_side; }

const bool Assignment::is_both_reference() const { return _is_both_reference; }

const std::vector<std::reference_wrapper<const HIR>> Assignment::get_children()
    const
{
    return {std::cref(*left_hand_side), std::cref(*right_hand_side)};
}

FunctionCall::FunctionCall(const std::shared_ptr<requirement::Type> type,
                           const std::shared_ptr<location::Location> location,
                           std::unique_ptr<HIR>&& function_obj,
                           std::vector<std::unique_ptr<HIR>>&& arguments)
    : HIRBase(type, location),
      function_obj(std::move(function_obj)),
      arguments(std::move(arguments))
{
}

const HIR& FunctionCall::get_function_obj() const { return *function_obj; }

const std::vector<std::reference_wrapper<const HIR>>
FunctionCall::get_arguments() const
{
    return utils::get_transformed(
        arguments, [](const auto& ptr) { return std::cref(*ptr); });
}

const std::vector<std::reference_wrapper<const HIR>>
FunctionCall::get_children() const
{
    auto children = get_arguments();
    children.push_back(std::cref(*function_obj));
    return children;
}

Argument::Argument(const std::shared_ptr<requirement::Type> type,
                   const std::shared_ptr<location::Location> location,
                   const std::string name)
    : HIRBase(type, location), name(name)
{
}

const std::vector<std::reference_wrapper<const HIR>> Argument::get_children()
    const
{
    return {};
}

AccessElement::AccessElement(const std::shared_ptr<requirement::Type> type,
                             const std::shared_ptr<location::Location> location,
                             std::unique_ptr<HIR>&& left_hand_side,
                             const std::string element_name)
    : HIRBase(type, location),
      left_hand_side(std::move(left_hand_side)),
      element_name(element_name)
{
}

const HIR& AccessElement::get_left_hand_side() const { return *left_hand_side; }

const std::string& AccessElement::get_element_name() const
{
    return element_name;
}

const std::vector<std::reference_wrapper<const HIR>>
AccessElement::get_children() const
{
    return {std::cref(*left_hand_side)};
}

const HIR& AccessList::get_list() const { return *list; }

const HIR& AccessList::get_index() const { return *index; }

const std::vector<std::reference_wrapper<const HIR>> AccessList::get_children()
    const
{
    return {std::cref(*list), std::cref(*index)};
}

Construction::Construction(
    const std::shared_ptr<requirement::Type> type,
    const std::shared_ptr<location::Location> location,
    std::vector<std::pair<std::string, std::unique_ptr<HIR>>>&& arguments)
    : HIRBase(type, location), arguments(std::move(arguments))
{
}

const std::vector<std::reference_wrapper<const HIR>>
Construction::get_children() const
{
    std::vector<std::reference_wrapper<const HIR>> result;
    for (auto& pair : arguments)
    {
        result.push_back(std::cref(*pair.second));
    }
    return result;
}

UnionConstruction::UnionConstruction(
    const std::shared_ptr<requirement::Type> type,
    const std::shared_ptr<location::Location> location,
    std::pair<std::string, std::unique_ptr<HIR>>&& argument)
    : HIRBase(type, location), argument(std::move(argument))
{
}

const std::vector<std::reference_wrapper<const HIR>>
UnionConstruction::get_children() const
{
    return {std::cref(*argument.second)};
}

const std::pair<std::string, std::reference_wrapper<const HIR>>
UnionConstruction::get_argument() const
{
    return std::make_pair(argument.first, std::cref(*argument.second));
}

Block::Block(std::shared_ptr<requirement::Type> type,
             const std::shared_ptr<location::Location> location,
             std::vector<std::unique_ptr<HIR>>&& hir_list)
    : HIRBase(type, location), hir_list(std::move(hir_list))
{
}

const std::vector<std::reference_wrapper<const HIR>> Block::get_list() const
{
    return utils::get_transformed(
        hir_list, [](const auto& ptr) { return std::cref(*ptr); });
}

const std::vector<std::reference_wrapper<const HIR>> Block::get_children() const
{
    return get_list();
}

std::vector<std::unique_ptr<HIR>>& Block::get_mutable_list()
{
    return hir_list;
}

const HIR& Match::get_target() const { return *target; }

const std::vector<std::pair<std::string, std::reference_wrapper<const HIR>>>
Match::get_patterns() const
{
    std::vector<std::pair<std::string, std::reference_wrapper<const HIR>>>
        result;
    for (auto& pattern : patterns)
    {
        result.push_back(
            std::make_pair(pattern.first, std::cref(*pattern.second)));
    }
    return result;
}

std::optional<std::reference_wrapper<const HIR>> Match::get_default_case() const
{
    if (default_case.has_value())
    {
        return {*default_case.value()};
    }
    return std::nullopt;
}

const std::vector<std::reference_wrapper<const HIR>> Match::get_children() const
{
    std::vector<std::reference_wrapper<const HIR>> children;
    children.push_back(std::cref(*target));
    auto pattern_expressions = utils::get_transformed(
        patterns,
        [](const std::pair<std::string, std::unique_ptr<HIR>>& pattern) {
            return std::cref(*pattern.second);
        });
    children.insert(children.end(), pattern_expressions.begin(),
                    pattern_expressions.end());
    if (default_case.has_value())
    {
        children.push_back(*default_case.value());
    }
    return children;
}

If::If(const std::shared_ptr<requirement::Type> type,
       const std::shared_ptr<location::Location> location,
       std::unique_ptr<HIR>&& condition, std::unique_ptr<HIR>&& body,
       std::unique_ptr<HIR>&& else_body)
    : HIRBase(type, location),
      condition(std::move(condition)),
      body(std::move(body)),
      else_body(std::move(else_body))
{
}

If::If(const std::shared_ptr<requirement::Type> type,
       const std::shared_ptr<location::Location> location,
       std::unique_ptr<HIR>&& condition, std::unique_ptr<HIR>&& body)
    : HIRBase(type, location),
      condition(std::move(condition)),
      body(std::move(body)),
      else_body(std::nullopt)
{
}

const HIR& If::get_condition() const { return *condition; }

const HIR& If::get_body() const { return *body; }

const std::optional<std::reference_wrapper<const HIR>> If::get_else_body() const
{
    if (else_body.has_value())
    {
        return *else_body.value();
    }
    else
    {
        return std::nullopt;
    }
}

const std::vector<std::reference_wrapper<const HIR>> If::get_children() const
{
    if (else_body.has_value())
    {
        return {std::cref(*condition), std::cref(*body),
                std::cref(*else_body.value())};
    }
    else
    {
        return {std::cref(*condition), std::cref(*body)};
    }
}

const HIR& Loop::get_condition() const { return *condition; }

const HIR& Loop::get_body() const { return *body; }

const std::vector<std::reference_wrapper<const HIR>> Loop::get_children() const
{
    return {std::cref(*condition), std::cref(*body)};
}

BinaryExpression::BinaryExpression(
    const std::shared_ptr<requirement::Type> type,
    const std::shared_ptr<location::Location> location,
    std::pair<std::unique_ptr<HIR>, std::unique_ptr<HIR>>&& targets,
    const OperatorKind& kind)
    : HIRBase(type, location), targets(std::move(targets)), kind(kind)
{
}

const OperatorKind BinaryExpression::get_kind() const { return kind; }

std::pair<std::reference_wrapper<const HIR>, std::reference_wrapper<const HIR>>
BinaryExpression::get_targets() const
{
    return std::make_pair(std::cref(*targets.first),
                          std::cref(*targets.second));
}

const std::vector<std::reference_wrapper<const HIR>>
BinaryExpression::get_children() const
{
    return {std::cref(*targets.first), std::cref(*targets.second)};
}

const std::vector<std::pair<std::string, std::reference_wrapper<const HIR>>>
Construction::get_arguments() const
{
    std::vector<std::pair<std::string, std::reference_wrapper<const HIR>>>
        result;
    for (auto& pair : arguments)
    {
        result.push_back(std::make_pair(pair.first, std::cref(*pair.second)));
    }
    return result;
}

SetResult::SetResult(const std::shared_ptr<requirement::Type> type,
                     const std::shared_ptr<location::Location> location,
                     std::unique_ptr<HIR>&& expression)
    : HIRBase(type, location), expression(std::move(expression))
{
}

const HIR& SetResult::get_expression() const { return *expression; }

const std::vector<std::reference_wrapper<const HIR>> SetResult::get_children()
    const
{
    return {std::cref(*expression)};
}

Return::Return(const std::shared_ptr<requirement::Type> type,
               const std::shared_ptr<location::Location> location,
               std::unique_ptr<HIR>&& expression)
    : HIRBase(type, location), expression(std::move(expression))
{
}

const HIR& Return::get_expression() const { return *expression; }

const std::vector<std::reference_wrapper<const HIR>> Return::get_children()
    const
{
    return {std::cref(*expression)};
}

std::unique_ptr<HIR> HIR::clone(requirement::TypeEnvironment& type_environment,
                                requirement::ClonedTypeMap map) const
{
    return accept(hir::Cloner(type_environment, map));
}

std::string HIR::to_string() const { return accept(hir::Printer()); }

}  // namespace clawn::hir