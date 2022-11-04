#pragma once
#include <cstddef>
#include <istream>
#include <memory>
// #include "parser.tab.hh"
#include "include/compiler/parser/lexer.hpp"
#include "include/compiler/parser/prelexer.hpp"

namespace clawn::ast
{
class Builder;
class AST;
}  // namespace clawn::ast

namespace clawn::requirement
{
class RequirementBuilder;
}

namespace clawn::compiler
{
struct DeclarationInfo
{
    std::string name;
    std::vector<std::string> argument_names;
    DeclarationInfo() = default;
    DeclarationInfo(std::string name, std::vector<std::string>&& argument_names)
        : name(name), argument_names(std::move(argument_names))
    {
    }
};

DeclarationInfo to_declaration_info(std::string token_text);

std::string extract(const std::string& text, const std::string& keyword);

std::string extract_structure_name(const std::string& text);

std::string extract_union_name(const std::string& text);

std::string extract_function_name(const std::string& text);

class Driver
{
    public:
    std::shared_ptr<ast::Builder> ast_builder;
    std::shared_ptr<requirement::RequirementBuilder> requirement_builder;

    public:
    Driver(std::shared_ptr<ast::Builder> ast_builder,
           std::shared_ptr<requirement::RequirementBuilder> requirement_builder)
        : ast_builder(ast_builder), requirement_builder(requirement_builder){};
    void parse(const char* const filename);
    void parse(std::istream& iss);

    private:
    void parse_helper(std::istream& stream);
};

}  // namespace clawn::compiler
