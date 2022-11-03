#pragma once
#include <string>
#include <memory>
#include <set>
#include "include/ast/ast.hpp"
#include "include/requirement/requirement.hpp"

namespace clawn::ast
{
class AST;
class Builder;
}  // namespace clawn::ast

namespace clawn::requirement
{
class TypeEnvironment;
}

namespace clawn::compiler
{
class Module
{
    public:
    using TypeRequirement =
        std::tuple<std::shared_ptr<requirement::Requirement>,
                   std::shared_ptr<requirement::Type>,
                   std::shared_ptr<clawn::location::Location>>;

    private:
    std::string name;
    std::shared_ptr<ast::Builder> ast_builder;
    std::shared_ptr<requirement::TypeEnvironment> type_environment;
    std::shared_ptr<requirement::RequirementEnvironment>
        requirement_environment;
    std::shared_ptr<requirement::RequirementBuilder> requirement_builder;
    std::vector<TypeRequirement> requirements;

    public:
    Module() = delete;
    Module(const Module&) = delete;
    Module(std::string name, unsigned int current_line_number = 0);
    Module& operator=(const Module&) = delete;
    std::shared_ptr<requirement::TypeEnvironment> get_type_environment() const;
    void register_requirement(const std::string&,
                              std::shared_ptr<requirement::Requirement>);
    void add_requirement(std::shared_ptr<requirement::Requirement>,
                         std::shared_ptr<requirement::Type>,
                         std::shared_ptr<clawn::location::Location>);
    const std::vector<TypeRequirement>& get_requirements() const;
    bool verify_requirements() const;
    const std::string& get_name() const;
    const std::shared_ptr<ast::AST> generate_ast() const;
};
}  // namespace clawn::compiler