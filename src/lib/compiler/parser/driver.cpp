#include "include/compiler/parser/driver.hpp"
#include <cassert>
#include <cctype>
#include <fstream>
#include "include/ast/ast.hpp"

namespace clawn::compiler
{
clawn::compiler::DeclarationInfo to_declaration_info(std::string token_text)
{
    //"function func  (a,b , c  )"

    char last_char = '\0';
    std::string::size_type identifier_start = 0;

    for (auto c : token_text)
    {
        if (last_char == ' ' and c != ' ') break;
        last_char = c;
        identifier_start += 1;
    }

    std::string::size_type identifier_end =
        token_text.find("(", identifier_start);
    std::string identifier =
        token_text.substr(identifier_start, identifier_end - identifier_start);
    // identifier = "func"

    std::string without_identifier = token_text.substr(identifier_end);
    std::string current_argument;
    std::vector<std::string> arguments;

    for (auto c : without_identifier)
    {
        if (c == ' ' or c == '(')
        {
            continue;
        }
        if (c == ')')
        {
            if (!current_argument.empty())
            {
                arguments.push_back(current_argument);
            }
            break;
        }
        if (c == ',')
        {
            arguments.push_back(current_argument);
            current_argument = "";
            continue;
        }
        current_argument += c;
    }
    return clawn::compiler::DeclarationInfo(identifier, std::move(arguments));
}

std::string extract(const std::string &text, const std::string &keyword)
{
    auto keyword_length = keyword.size();
    auto pre_extracted = text.substr(keyword_length, text.size());
    std::string extracted("");
    for (auto character : pre_extracted)
    {
        if (character == ' ')
        {
            continue;
        }
        extracted += character;
    }
    return extracted;
}

std::string extract_structure_name(const std::string &text)
{
    return extract(text, "structure");
}

std::string extract_union_name(const std::string &text)
{
    return extract(text, "union");
}

std::string extract_function_name(const std::string &text)
{
    return extract(text, "function");
}

void compiler::Driver::parse(const char *const filename)
{
    assert(filename != nullptr);
    std::ifstream stream(filename);
    if (!stream.good())
    {
        exit(1);
    }
    auto prelexer = compiler::PreLexer(&stream, *this);
    prelexer.prelex();
    std::ifstream new_stream(filename);
    auto lexer = compiler::Lexer(&new_stream, *this);
    auto parser = compiler::Parser(lexer, *this);
    const int accept(0);

    if (parser.parse() != accept)
    {
        std::cerr << "syntax error" << std::endl;
    }
}

void compiler::Driver::parse_helper(std::istream &stream) {}
}  // namespace clawn::compiler