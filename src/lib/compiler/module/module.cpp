#include "include/hierarchy/hierarchical_collector.hpp"
#include "include/ast/ast.hpp"
#include "include/location/location.hpp"
#include "include/ast/builder.hpp"
#include "include/compiler/module/module.hpp"
#include "include/compiler/parser/driver.hpp"
#include "include/compiler/constants.hpp"
#include "include/requirement/type_solver.hpp"
#include "include/requirement/type.hpp"

namespace clawn::compiler
{
Module::Module(std::string name, unsigned int current_line_number)
    : name(name),
      type_environment(std::make_shared<requirement::TypeEnvironment>()),
      requirement_environment(
          std::make_shared<requirement::RequirementEnvironment>(
              type_environment)),
      requirement_builder(std::make_shared<requirement::RequirementBuilder>(
          requirement_environment))
{
    auto filename = name.c_str();
    ast_builder = std::make_shared<ast::Builder>(
        filename, constants::VARIOUS_TAGS_LIMIT, current_line_number);
    compiler::Driver driver(ast_builder, requirement_builder);
    driver.parse(filename);
}

std::shared_ptr<requirement::TypeEnvironment> Module::get_type_environment()
    const
{
    return type_environment;
}

void Module::register_requirement(
    const std::string& name,
    std::shared_ptr<requirement::Requirement> requirement)
{
    requirement_environment->register_requirement(name, requirement);
}

void Module::add_requirement(
    std::shared_ptr<requirement::Requirement> requirement,
    std::shared_ptr<requirement::Type> type,
    std::shared_ptr<clawn::location::Location> location)
{
    requirements.push_back(std::make_tuple(requirement, type, location));
}


bool Module::verify_requirements() const
{
    for (auto& type_requirement : requirements)
    {
        auto requirement = std::get<std::shared_ptr<requirement::Requirement>>(
            type_requirement);
        auto type =
            std::get<std::shared_ptr<requirement::Type>>(type_requirement);
        type = type_environment->get_solver()->solve(type);
        auto location = std::get<std::shared_ptr<clawn::location::Location>>(
            type_requirement);
        if (type->is_type<requirement::UnionType>())
        {
            auto& union_type = type->as<requirement::UnionType>();
            if (union_type._is_by_compiler())
            {
                for (auto& tag : union_type.get_solved_tags())
                {
                    if (tag.first == "")
                    {
                        continue;
                    }
                    if (!tag.second->is_type<requirement::UnsolvedType>() &&
                        !requirement->is_satisfied_with(
                            tag.second, *requirement_environment))
                    {
                        std::cout
                            << "\e[1;31m"
                               "Error: requirement '" +
                                   requirement->to_string(
                                       *requirement_environment) +
                                   "' is set at line "
                            << location->get_token_info().get_line_number()
                            << " but not satisfied with type '"
                            << tag.second->to_string() << "'\e[0m" << std::endl;
                        exit(1);
                        return false;
                    }
                }
            }
        }
        else
        {
            if (!type->is_type<requirement::UnsolvedType>() &&
                !requirement->is_satisfied_with(type, *requirement_environment))
            {
                std::cout << "\e[1;31m"
                             "Error: requirement '" +
                                 requirement->to_string(
                                     *requirement_environment) +
                                 "' is set at line "
                          << location->get_token_info().get_line_number()
                          << " but not satisfied with type '"
                          << type->to_string() << "'\e[0m" << std::endl;
                exit(1);
                return false;
            }
        }
    }
    return true;
}

const std::string& Module::get_name() const { return name; }

const std::shared_ptr<ast::AST> Module::generate_ast() const
{
    return ast_builder->build();
}
}  // namespace clawn::compiler