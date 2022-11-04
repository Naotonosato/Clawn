#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <memory>
#include <variant>
#include <iostream>
#include "include/requirement/type.hpp"
#include "include/utils/variant_wrapper.hpp"

namespace clawn::requirement
{
class Type;
class Requirement;
using RequirementText = std::string;

class RequirementClauseBase
{
    public:
    bool is_satisfied_with(const std::shared_ptr<Type> type);
    bool is_satisfied_with(
        const std::shared_ptr<Type> type,
        const std::vector<std::shared_ptr<Requirement>>& parameters);
};

class Reference : public RequirementClauseBase
{
    std::optional<std::shared_ptr<Requirement>> reference_to;

    public:
    Reference() : reference_to(std::nullopt) {}
    Reference(std::shared_ptr<Requirement> reference_to)
        : reference_to(reference_to)
    {
    }
    const std::optional<std::shared_ptr<Requirement>> get_reference_to() const;
};

class Value : public RequirementClauseBase
{
    std::optional<std::shared_ptr<Requirement>> requirement;

    public:
    Value() : requirement(std::nullopt) {}
    Value(std::shared_ptr<Requirement> requirement) : requirement(requirement)
    {
    }
    const std::optional<std::shared_ptr<Requirement>> get_requirement() const;
};

class Has : public RequirementClauseBase
{
    private:
    const std::string member_name;
    const std::optional<std::shared_ptr<Requirement>> member_requirement;

    public:
    Has(std::string member_name)
        : member_name(member_name), member_requirement(std::nullopt)
    {
    }
    Has(std::string member_name,
        std::shared_ptr<Requirement> member_requirement)
        : member_name(member_name), member_requirement(member_requirement)
    {
    }
    const std::string& get_member_name() const;
    const std::optional<std::shared_ptr<Requirement>> get_member_requirement()
        const;
};

class Not : public RequirementClauseBase
{
    private:
    const std::shared_ptr<Requirement> requirement_shold_not_satisfied;

    public:
    Not(std::shared_ptr<Requirement> requirement_shold_not_satisfied)
        : requirement_shold_not_satisfied(requirement_shold_not_satisfied)
    {
    }
    const std::shared_ptr<Requirement> get_requirement_shold_not_satisfied()
        const;
};

class CallableWith : public RequirementClauseBase
{
    private:
    const std::vector<std::shared_ptr<Requirement>> arguments_requirements;

    public:
    CallableWith(
        const std::vector<std::shared_ptr<Requirement>>& arguments_requirements)
        : arguments_requirements(arguments_requirements)
    {
    }
};

class RequirementList : public RequirementClauseBase
{
    private:
    const std::vector<std::shared_ptr<Requirement>> requirements;

    public:
    RequirementList(
        const std::vector<std::shared_ptr<Requirement>>& requirements)
        : requirements(requirements)
    {
    }
    const std::vector<std::shared_ptr<Requirement>>& get_requirements() const;
};

class RequirementEnvironment;

class Requirement
{
    using variant_type =
        std::variant<Reference, Value, Has, Not, CallableWith, RequirementList,
                     RequirementText, std::shared_ptr<Type>>;
    variant_type content;

    public:
    Requirement() = delete;
    Requirement(const Requirement&) = default;
    Requirement(Requirement&&) = default;
    template <class ConstraintType>
    Requirement(const ConstraintType& requirement) : content(requirement)
    {
    }
    template <class ConstraintType>
    Requirement(const ConstraintType&& requirement)
        : content(std::move(requirement))
    {
    }

    bool is_satisfied_with(std::shared_ptr<requirement::Type> type,
                           const RequirementEnvironment&) const;
    std::string to_string(const RequirementEnvironment& environment) const;
};

class RequirementEnvironment
{
    std::map<std::string, std::shared_ptr<Requirement>> requirement_name_map;
    std::shared_ptr<TypeEnvironment> type_environment;

    public:
    RequirementEnvironment() = delete;
    RequirementEnvironment(std::shared_ptr<TypeEnvironment> type_environment)
        : type_environment(type_environment)
    {
    }
    void register_requirement(const std::string& name,
                              std::shared_ptr<Requirement>);
    std::shared_ptr<Requirement> get_requirement(
        const std::string& name,
        const std::string prefix = hierarchy::Hierarchy::get_root_name() +
                                   "/") const;
};

class RequirementBuilder
{
    std::shared_ptr<RequirementEnvironment> environment;

    public:
    RequirementBuilder() = delete;
    RequirementBuilder(
        const std::shared_ptr<RequirementEnvironment> environment)
        : environment(environment)
    {
    }
    std::shared_ptr<Requirement> create_requirement(
        const std::string& text) const;
    std::shared_ptr<Requirement> create_requirement(
        const std::string& text, const std::string& text2) const;
    std::shared_ptr<Requirement> create_requirement(
        const std::string& text, const std::string& text2,
        const std::string& text3) const;
    std::shared_ptr<Requirement> create_named_requirement_list(
        const std::string& name,
        std::vector<std::shared_ptr<Requirement>>&& list);
};
}  // namespace clawn::requirement