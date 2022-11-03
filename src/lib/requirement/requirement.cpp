#include <functional>
#include "include/requirement/requirement.hpp"
#include "include/utils/transform.hpp"
#include "include/utils/pattern_match.hpp"

namespace clawn::requirement
{
bool Requirement::is_satisfied_with(
    std::shared_ptr<requirement::Type> type,
    const RequirementEnvironment& environment) const
{
    return utils::Match{content}(
        utils::Type<Reference>(),
        [type, &environment](const Reference& requirement) {
            if (!type->is_type<requirement::ReferenceType>())
            {
                return false;
            }
            auto reference_to = requirement.get_reference_to();
            if (reference_to.has_value())
            {
                return reference_to.value()->is_satisfied_with(
                    type->as<ReferenceType>()
                        .get_reference_to_without_solving(),
                    environment);
            }
            return true;
        },
        utils::Type<Value>(),
        [type, &environment](const Value& requirement) {
            if (type->is_type<requirement::ReferenceType>())
            {
                return false;
            }
            auto requirement_of_type = requirement.get_requirement();
            if (requirement_of_type.has_value())
            {
                return requirement_of_type.value()->is_satisfied_with(
                    type, environment);
            }
            return true;
        },
        utils::Type<Has>(),
        [type, &environment](const Has& requirement) {
            auto member_name = requirement.get_member_name();
            auto member_requirement = requirement.get_member_requirement();
            return utils::Match{type->get_variant()}(
                utils::Type<requirement::StructureType>(),
                [type, member_name, member_requirement, &environment](
                    const requirement::StructureType& structure_type) {
                    auto member_types = structure_type.get_member_types();
                    if (!structure_type.has_member_named(member_name))
                    {
                        return false;
                    }
                    else
                    {
                        if (!member_requirement.has_value())
                        {
                            return true;
                        }
                        return member_requirement.value()->is_satisfied_with(
                            structure_type.get_element_type(member_name),
                            environment);
                    }
                },
                utils::Default(), [](const auto&) { return false; });
        },
        utils::Type<Not>(),
        [type, &environment](const Not& requirement) {
            auto equirement_shold_not_satisfied =
                requirement.get_requirement_shold_not_satisfied();
            return !equirement_shold_not_satisfied->is_satisfied_with(
                type, environment);
        },
        utils::Type<CallableWith>(),
        [type](const CallableWith& requirement) { return false; },
        utils::Type<RequirementList>(),
        [type, &environment](const RequirementList& requirement) {
            for (const auto& requirement_ : requirement.get_requirements())
            {
                if (!requirement_->is_satisfied_with(type, environment))
                {
                    return false;
                }
            }
            return true;
        },
        utils::Type<RequirementText>(),
        [type, &environment](const RequirementText& requirement) {
            return environment.get_requirement(requirement)
                ->is_satisfied_with(type, environment);
        },
        utils::Type<std::shared_ptr<Type>>(),
        [type](const std::shared_ptr<Type>& requirement) {
            auto solver = type->get_type_solver();
            if (solver->solve(type)->is_type<requirement::StructureType>())
            {
                return solver->solve(type)->get_name() ==
                       solver->solve(requirement)->get_name();
            }
            return type->is_same_as(*requirement, false, true);
        },
        utils::Default(), [](const auto&) { return false; });
}

std::string Requirement::to_string(
    const RequirementEnvironment& environment) const
{
    return utils::Match{content}(
        utils::Type<Reference>(),
        [environment](const Reference& requirement) {
            auto reference_to = requirement.get_reference_to();
            if (reference_to.has_value())
            {
                return "reference to " +
                       reference_to.value()->to_string(environment);
            }
            return std::string("reference");
        },
        utils::Type<Value>(),
        [environment](const Value& requirement) {
            auto requirement_of_type = requirement.get_requirement();
            if (requirement_of_type.has_value())
            {
                return "value of " +
                       requirement_of_type.value()->to_string(environment);
            }
            return std::string("value");
        },
        utils::Type<Has>(),
        [environment](const Has& requirement) {
            auto member_name = requirement.get_member_name();
            auto member_requirement = requirement.get_member_requirement();
            if (member_requirement.has_value())
            {
                return "has member named '" + member_name + "': " +
                       member_requirement.value()->to_string(environment);
            }
            return "has member named '" + member_name + "'";
        },
        utils::Type<Not>(),
        [environment](const Not& requirement) {
            auto requirement_shold_not_satisfied =
                requirement.get_requirement_shold_not_satisfied();
            return requirement_shold_not_satisfied->to_string(environment);
        },
        utils::Type<CallableWith>(),
        [](const CallableWith& requirement) { return std::string(); },
        utils::Type<RequirementList>(),
        [environment](const RequirementList& requirement) {
            std::string text = "";
            for (const auto& requirement_ : requirement.get_requirements())
            {
                text += requirement_->to_string(environment) + ", ";
            }
            text.pop_back();
            text.pop_back();
            return text;
        },
        utils::Type<RequirementText>(),
        [environment](const RequirementText& requirement) {
            auto requirement_body = environment.get_requirement(requirement);
            return utils::Match{requirement_body->content}(
                utils::Type<std::shared_ptr<Type>>(),
                [environment, requirement_body](const auto&) {
                    return requirement_body->to_string(environment);
                },
                utils::Default(),
                [environment, requirement, requirement_body](const auto&) {
                    return requirement + "(" +
                           environment.get_requirement(requirement)
                               ->to_string(environment) +
                           ")";
                });
        },
        utils::Type<std::shared_ptr<Type>>(),
        [](const std::shared_ptr<Type>& requirement) {
            return requirement->get_name();
        },
        utils::Default(), [](const auto&) { return std::string(); });
}

const std::optional<std::shared_ptr<Requirement>> Reference::get_reference_to()
    const
{
    return reference_to;
}

const std::optional<std::shared_ptr<Requirement>> Value::get_requirement() const
{
    return requirement;
}

const std::string& Has::get_member_name() const { return member_name; }

const std::optional<std::shared_ptr<Requirement>> Has::get_member_requirement()
    const
{
    return member_requirement;
}

const std::shared_ptr<Requirement> Not::get_requirement_shold_not_satisfied()
    const
{
    return requirement_shold_not_satisfied;
}

const std::vector<std::shared_ptr<Requirement>>&
RequirementList::get_requirements() const
{
    return requirements;
}

void RequirementEnvironment::register_requirement(
    const std::string& name, std::shared_ptr<Requirement> requirement)
{
    requirement_name_map.insert(std::make_pair(name, requirement));
}

std::shared_ptr<Requirement> RequirementEnvironment::get_requirement(
    const std::string& name, const std::string prefix) const
{
    if (requirement_name_map.count(name))
    {
        return requirement_name_map.at(name);
    }
    return std::make_shared<Requirement>(
        type_environment->get_type_by_identifier(prefix + name));
}

std::shared_ptr<Requirement> RequirementBuilder::create_requirement(
    const std::string& text) const
{
    if (text == "value")
    {
        return std::make_shared<Requirement>(Value());
    }
    if (text == "reference")
    {
        return std::make_shared<Requirement>(Reference());
    }
    return std::make_shared<Requirement>(text);
}

std::shared_ptr<Requirement> RequirementBuilder::create_requirement(
    const std::string& text, const std::string& text2) const
{
    if (text == "value")
    {
        return std::make_shared<Requirement>(Value(create_requirement(text2)));
    }
    if (text == "reference")
    {
        return std::make_shared<Requirement>(
            Reference(create_requirement(text2)));
    }
    if (text == "has")
    {
        return std::make_shared<Requirement>(Has(text2));
    }
    throw std::runtime_error("unknown requirement: " + text + " " + text2);
}

std::shared_ptr<Requirement> RequirementBuilder::create_requirement(
    const std::string& text, const std::string& text2,
    const std::string& text3) const
{
    if (text == "has")
    {
        return std::make_shared<Requirement>(
            Has(text2, create_requirement(text3)));
    }
    throw std::runtime_error("unknown requirement: " + text + " " + text2 +
                             " " + text3);
}

std::shared_ptr<Requirement> RequirementBuilder::create_named_requirement_list(
    const std::string& name, std::vector<std::shared_ptr<Requirement>>&& list)
{
    auto requirement_list =
        std::make_shared<Requirement>(RequirementList(list));
    environment->register_requirement(name, requirement_list);
    return requirement_list;
}

}  // namespace clawn::requirement