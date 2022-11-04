%{
#include <string>
#include <algorithm>
#include "parser.hpp"
#include "include/compiler/parser/lexer.hpp"
#include "include/utils/transform.hpp"
#undef  YY_DECL
#define YY_DECL int clawn::compiler::Lexer::yylex( clawn::compiler::Parser::semantic_type * const lval, clawn::compiler::Parser::location_type *loc )
#define yyterminate() return clawn::compiler::Parser::token::END;
#define YY_NO_UNISTD_H
#define YY_USER_ACTION loc->step(); loc->columns(yyleng); driver.ast_builder->count_token_index(yyleng);

%}

%option yyclass="clawn::compiler::Lexer"
%option noyywrap
%option noinput
%option nounput
%option c++

COMMENT             \/\/.*\n
STRING_LITERAL      \"[^\"]*\"
INT_LITERAL         [0-9]+
FLOAT_LITERAL       [0-9]+\.[0-9]*
LAZY                lazy
USE                 use

REFER               refer
ACCESS              access
VARIOUS             various

SINGLE_QUOTATION '
OP_EQUAL    ==
OP_NOT_EQUAL \!=
OP_MORE_EQUAL >=
OP_LESS_EQUAL <=
OP_MORE     >
OP_LESS     <
OP_AND      and
OP_OR       or
EQUAL       =
LEFT_ARROW       <-
BOTH_SIDE_ARROW  <->
PLUS        \+
MINUS       -
ASTERISK    \*
SLASH       \/
UNION_CONSTRUCTOR    [a-zA-Z_][0-9a-zA-Z_]*#[a-zA-Z_][0-9a-zA-Z_]*
DOUBLE_DOT           \.\.
TRIPLE_DOT           \.\.\.
DOT_IDENTIFIER       \.[a-zA-Z_][0-9a-zA-Z_]*
RIGHT_ARROW_IDENTIFIER      ->[a-zA-Z_][0-9a-zA-Z_]*
COLON       \:
SEMICOLON   ;
COMMA       ,

IF          if
ELSE        else
REPEAT      repeat
MATCH       match
ARM         =>
DEFAULT     default
IN          in
WHILE       while
GLOBAL      global
IMPORT      import
C_TYPE_DECLARATION @Ctype[ \t]+[a-zA-Z_][0-9a-zA-Z_]*
C_FUNCTION_DECLARATION  @Cfunction[ \t]+[a-zA-Z_][0-9a-zA-Z_]*
C_FUNCTION_ARGUMENT     arguments[ \t]*\:
C_FUNCTION_RETURN       return[ \t]*\:
RETURN      return
FUNCTION_DEFINITION function[ \t]+[a-zA-Z_][0-9a-zA-Z_]*[ \t]*
METHOD_DEFINITION method[ \t]+[a-zA-Z_][0-9a-zA-Z_]*[ \t]*
CONSTRUCTOR_DEFINITION @constructor[ \t]+[a-zA-Z_][0-9a-zA-Z_]*[ \t]*\(([a-zA-Z_][0-9a-zA-Z_]*)*([ \t]*,[ \t]*[a-zA-Z_][0-9a-zA-Z_]*[ \t]*)*\)
DESTRUCTOR_DEFINITION @destructor[ \t]+[a-zA-Z_][0-9a-zA-Z_]*[ \t]*\(([a-zA-Z_][0-9a-zA-Z_]*)*([ \t]*,[ \t]*[a-zA-Z_][0-9a-zA-Z_]*[ \t]*)*\)
GETTER_DEFINITION @[a-zA-Z_][0-9a-zA-Z_]*\.get[ \t]*\(([a-zA-Z_][0-9a-zA-Z_]*)*([ \t]*,[ \t]*[a-zA-Z_][0-9a-zA-Z_]*[ \t]*)*\)
SETTER_DEFINITION @[a-zA-Z_][0-9a-zA-Z_]*\.set*[ \t]*\(([a-zA-Z_][0-9a-zA-Z_]*)*([ \t]*,[ \t]*[a-zA-Z_][0-9a-zA-Z_]*[ \t]*)*\)
CLASS_DEFINITION  class[ \t]+[a-zA-Z_][0-9a-zA-Z_]*[ \t]*\(([a-zA-Z_][0-9a-zA-Z_]*)*([ \t]*,[ \t]*[a-zA-Z_][0-9a-zA-Z_]*[ \t]*)*\)
STRUCTURE_DEFINITION structure[ \t]+[a-zA-Z_][0-9a-zA-Z_]*[ \t]*
UNION_DEFINITION union[ \t]+[a-zA-Z_][0-9a-zA-Z_]*[ \t]*
REQUIREMENT requirement

LEFT_PARENTHESIS  \(
LEFT_PARENTHESIS_AND_EOL \(\n
RIGHT_PARENTHESIS \)
LEFT_BRACKET    \[
RIGHT_BRACKET   \]
LEFT_CURLY_BRACE \{
RIGHT_CURLY_BRACE \}

CALL        .+\(.*\)
MEMBER_IDENTIFIER @[a-zA-Z_][0-9a-zA-Z_]* 
IDENTIFIER  [a-zA-Z_][0-9a-zA-Z_]*
EOL                 \n|\r\n

%%

^[ \t]*\n {loc->lines();driver.ast_builder->count_line_number();}
^[ \t]*\r\n {loc->lines();driver.ast_builder->count_line_number();}
{EOL} {
    loc->lines();
    driver.ast_builder->count_line_number();
    return clawn::compiler::Parser::token::EOL;
}
{COMMENT} {loc->lines();driver.ast_builder->count_line_number();}
{STRING_LITERAL} {
    driver.ast_builder->set_token(std::string(yytext));
    std::string text = yytext;
    text = text.substr(1,text.size()-2);
    lval->build<std::string>() = text;
    return clawn::compiler::Parser::token::STRING_LITERAL;
}
{INT_LITERAL} {
    driver.ast_builder->set_token(std::string(yytext));
    lval->build<long long>() = std::stoll(yytext);
    return clawn::compiler::Parser::token::INT_LITERAL;
}
{FLOAT_LITERAL} {
    driver.ast_builder->set_token(std::string(yytext));
    lval->build<double>() = std::stod(yytext);
    return clawn::compiler::Parser::token::FLOAT_LITERAL;
}
{LAZY} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::LAZY;
}
{REFER} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::REFER;
}
{ACCESS} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::ACCESS;
}
{VARIOUS} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::VARIOUS;
}
{SINGLE_QUOTATION} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::SINGLE_QUOTATION;
}
{USE} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::USE;
}
{UNION_CONSTRUCTOR} {
    driver.ast_builder->set_token(std::string(yytext));
    lval->build<std::unique_ptr<clawn::ast::UnionConstructorInfo>>() = std::make_unique<clawn::ast::UnionConstructorInfo>(ast::extract_union_constructor_info(yytext));
    return clawn::compiler::Parser::token::UNION_CONSTRUCTOR;
}
{DOUBLE_DOT} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::DOUBLE_DOT;
}
{TRIPLE_DOT} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::TRIPLE_DOT;
}
{DOT_IDENTIFIER} {
    driver.ast_builder->set_token(std::string(yytext));
    std::string text = yytext;
    text = text.substr(1,text.size()-1);
    lval->build<std::string>() = text;
    return clawn::compiler::Parser::token::DOT_IDENTIFIER;
}
{RIGHT_ARROW_IDENTIFIER} {
    driver.ast_builder->set_token(std::string(yytext));
    std::string text = yytext;
    text = text.substr(2,text.size()-1);
    lval->build<std::string>() = text;
    return clawn::compiler::Parser::token::RIGHT_ARROW_IDENTIFIER;
}
{ASTERISK} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::ASTERISK;
}
{SLASH} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::SLASH;
}
{PLUS} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::PLUS;
}
{MINUS} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::MINUS;
}
{EQUAL} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::EQUAL;
}
{LEFT_ARROW} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::LEFT_ARROW;
}
{BOTH_SIDE_ARROW} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::BOTH_SIDE_ARROW;
}
{OP_EQUAL} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::OP_EQUAL;
}
{OP_NOT_EQUAL} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::OP_NOT_EQUAL;
}
{OP_MORE_EQUAL} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::OP_MORE_EQUAL;
}
{OP_LESS_EQUAL} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::OP_LESS_EQUAL;
}
{OP_MORE} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::OP_MORE;
}
{OP_LESS} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::OP_LESS;
}
{OP_AND} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::OP_AND;
}
{OP_OR} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::OP_OR;
}

{CONSTRUCTOR_DEFINITION} {
    driver.ast_builder->set_token(std::string(yytext));
    lval->build<std::unique_ptr<clawn::compiler::DeclarationInfo>>() = std::unique_ptr<clawn::compiler::DeclarationInfo>(new clawn::compiler::DeclarationInfo("constructor",{}));
    return clawn::compiler::Parser::token::CONSTRUCTOR_DEFINITION;
}

{DESTRUCTOR_DEFINITION} {
    driver.ast_builder->set_token(std::string(yytext));
    lval->build<std::unique_ptr<clawn::compiler::DeclarationInfo>>() = std::unique_ptr<clawn::compiler::DeclarationInfo>(new clawn::compiler::DeclarationInfo("destructor",{}));
    return clawn::compiler::Parser::token::DESTRUCTOR_DEFINITION;
}

{SETTER_DEFINITION} {
    std::string token_text = std::string(yytext);
    driver.ast_builder->set_token(token_text);
    auto left_parenthesis_start = std::min(token_text.find(" "),token_text.find("("));
    std::string setter_name = token_text.substr(0,left_parenthesis_start);
    std::string without_name = token_text.substr(left_parenthesis_start+1);
    std::string argument_name;
    for (auto c:without_name)
    {
        if (c == '(' )continue;
        if (c == ' ') continue;
        if (c== ')') continue;
        argument_name += c;
    }
    //std::cout << setter_name << argument_name << std::endl;
    lval->build<std::unique_ptr<clawn::compiler::DeclarationInfo>>() = std::unique_ptr<clawn::compiler::DeclarationInfo>(new clawn::compiler::DeclarationInfo(setter_name,{argument_name}));
    return clawn::compiler::Parser::token::SETTER_DEFINITION;
}

{GETTER_DEFINITION} {
    driver.ast_builder->set_token(std::string(yytext));
    lval->build<std::unique_ptr<clawn::compiler::DeclarationInfo>>() = std::make_unique<clawn::compiler::DeclarationInfo>(to_declaration_info(yytext));
    return clawn::compiler::Parser::token::GETTER_DEFINITION;
}

{FUNCTION_DEFINITION} {
    driver.ast_builder->set_token(std::string(yytext));
    /*
    auto declaration = to_declaration_info(yytext);
    
    driver.ast_builder->into_named_scope(declaration.name);
    auto mangled_argument_names = utils::get_transformed(declaration.argument_names,[this](const auto& name){return driver.ast_builder->register_argument(name);});
    driver.ast_builder->break_out_of_scope();
    //driver.ast_builder->register_function_declaration(declaration.name,mangled_argument_names);
    
    lval->build<std::unique_ptr<clawn::compiler::DeclarationInfo>>() = std::make_unique<clawn::compiler::DeclarationInfo>(declaration);
    */
    auto function_name = extract_function_name(std::string(yytext));
    //driver.ast_builder->register_function_declaration(function_name);
    lval->build<std::string>() = function_name;
    return clawn::compiler::Parser::token::FUNCTION_DEFINITION;
}
{METHOD_DEFINITION} {
    driver.ast_builder->set_token(std::string(yytext));
    auto method_name = extract(std::string(yytext),"method");
    lval->build<std::string>() = method_name;
    return clawn::compiler::Parser::token::METHOD_DEFINITION;
}
{CLASS_DEFINITION} {
    driver.ast_builder->set_token(std::string(yytext));
    lval->build<std::unique_ptr<clawn::compiler::DeclarationInfo>>() = std::make_unique<clawn::compiler::DeclarationInfo>(to_declaration_info(yytext));
    return clawn::compiler::Parser::token::CLASS_DEFINITION;
}

{STRUCTURE_DEFINITION} {
    driver.ast_builder->set_token(std::string(yytext));
    lval->build<std::string>() = extract_structure_name(std::string(yytext));
    return clawn::compiler::Parser::token::STRUCTURE_DEFINITION;
}

{UNION_DEFINITION} {
    driver.ast_builder->set_token(std::string(yytext));
    lval->build<std::string>() = extract_union_name(std::string(yytext));
    return clawn::compiler::Parser::token::UNION_DEFINITION;
}

{REQUIREMENT} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::REQUIREMENT;
}

{COLON} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::COLON;
}
{SEMICOLON} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::SEMICOLON;
}
{COMMA} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::COMMA;
}
{LEFT_PARENTHESIS} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::LEFT_PARENTHESIS;
}
{LEFT_PARENTHESIS_AND_EOL} {
    driver.ast_builder->set_token(std::string(yytext));
    loc->lines();
    driver.ast_builder->count_line_number();
    return clawn::compiler::Parser::token::LEFT_PARENTHESIS_AND_EOL;
}
{RIGHT_PARENTHESIS} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::RIGHT_PARENTHESIS;
}
{LEFT_BRACKET} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::LEFT_BRACKET;
}
{RIGHT_BRACKET} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::RIGHT_BRACKET;
}
{LEFT_CURLY_BRACE} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::LEFT_CURLY_BRACE;
}
{RIGHT_CURLY_BRACE} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::RIGHT_CURLY_BRACE;
}
{IF} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::IF;
}
{ELSE} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::ELSE;
}
{REPEAT} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::REPEAT;
}
{MATCH} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::MATCH;
}
{ARM} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::ARM;
}
{DEFAULT} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::DEFAULT;
}
{IN} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::IN;
}
{WHILE} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::WHILE;
}
{GLOBAL} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::GLOBAL;
}
{IMPORT} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::IMPORT;
}

{RETURN} {
    driver.ast_builder->set_token(std::string(yytext));
    return clawn::compiler::Parser::token::RETURN;
}

{C_TYPE_DECLARATION} {
    driver.ast_builder->set_token(std::string(yytext));
    lval->build<std::string>() = extract(yytext,"@Ctype");
    return clawn::compiler::Parser::token::C_TYPE_DECLARATION;
}
{C_FUNCTION_DECLARATION} {
    driver.ast_builder->set_token(std::string(yytext));
    lval->build<std::string>() = extract(yytext,"@CFunction");
    return clawn::compiler::Parser::token::C_FUNCTION_DECLARATION;
}
{C_FUNCTION_ARGUMENT} {
    return clawn::compiler::Parser::token::C_FUNCTION_ARGUMENT;
}
{C_FUNCTION_RETURN} {
    return clawn::compiler::Parser::token::C_FUNCTION_RETURN;
}

{MEMBER_IDENTIFIER} {
    driver.ast_builder->set_token(std::string(yytext));
    lval->build<std::string>() = std::string(yytext).erase(0, 1);
    return clawn::compiler::Parser::token::MEMBER_IDENTIFIER;
}
{IDENTIFIER} {
    driver.ast_builder->set_token(std::string(yytext));
    lval->build<std::string>() = yytext;
    return clawn::compiler::Parser::token::IDENTIFIER;
}
[ \t] {}
<<EOF>> {
    loc->lines();
    driver.ast_builder->count_line_number();
    return 0;
    }
%%