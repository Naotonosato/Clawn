%{
#include <string>
#include <algorithm>

#include "include/compiler/parser/prelexer.hpp"
#include "include/utils/transform.hpp"
#undef  YY_DECL
#define YY_DECL int clawn::compiler::PreLexer::prelex()
#define yyterminate() 
#define YY_NO_UNISTD_H
std::string current_structure_name = "";

%}

%option yyclass="clawn::compiler::PreLexer"
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

OP_EQUAL    ==
OP_NOT_EQUAL \!=
OP_MORE_EQUAL >=
OP_LESS_EQUAL <=
OP_MORE     >
OP_LESS     <
OP_AND      and
OP_OR       or
EQUAL       =
ARROW       <-
PLUS        \+
MINUS       -
ASTERISK    \*
SLASH       \/
UNION_CONSTRUCTOR    [a-zA-Z_][0-9a-zA-Z_]*#[a-zA-Z_][0-9a-zA-Z_]*
DOUBLE_DOT           \.\.
DOT_IDENTIFIER \.[a-zA-Z_][0-9a-zA-Z_]*
COLON       \:
SEMICOLON   ;
COMMA       ,

IF          if
ELSE        else
FOR         for
IN          in
WHILE       while
GLOBAL      global
IMPORT      import
RETURN      return
FUNCTION_DEFINITION function[ \t]+[a-zA-Z_][0-9a-zA-Z_]*[ \t]*
METHOD_DEFINITION method[ \t]+[a-zA-Z_][0-9a-zA-Z_]*[ \t]*
CONSTRUCTOR_DEFINITION @constructor[ \t]+[a-zA-Z_][0-9a-zA-Z_]*[ \t]*\(([a-zA-Z_][0-9a-zA-Z_]*)*([ \t]*,[ \t]*[a-zA-Z_][0-9a-zA-Z_]*[ \t]*)*\)
DESTRUCTOR_DEFINITION @destructor[ \t]+[a-zA-Z_][0-9a-zA-Z_]*[ \t]*\(([a-zA-Z_][0-9a-zA-Z_]*)*([ \t]*,[ \t]*[a-zA-Z_][0-9a-zA-Z_]*[ \t]*)*\)
GETTER_DEFINITION @[a-zA-Z_][0-9a-zA-Z_]*\.get[ \t]*\(([a-zA-Z_][0-9a-zA-Z_]*)*([ \t]*,[ \t]*[a-zA-Z_][0-9a-zA-Z_]*[ \t]*)*\)
SETTER_DEFINITION @[a-zA-Z_][0-9a-zA-Z_]*\.set*[ \t]*\(([a-zA-Z_][0-9a-zA-Z_]*)*([ \t]*,[ \t]*[a-zA-Z_][0-9a-zA-Z_]*[ \t]*)*\)
CLASS_DEFINITION  class[ \t]+[a-zA-Z_][0-9a-zA-Z_]*[ \t]*\(([a-zA-Z_][0-9a-zA-Z_]*)*([ \t]*,[ \t]*[a-zA-Z_][0-9a-zA-Z_]*[ \t]*)*\)
STRUCTURE_DEFINITION structure[ \t]+[a-zA-Z_][0-9a-zA-Z_]*[ \t]*
CONSTRAINT requirement

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

^[ \t]*\n {}
^[ \t]*\r\n {}
{EOL} {
    
    
    }
{COMMENT} {}
{STRING_LITERAL} {
        }
{INT_LITERAL} {
            }
{FLOAT_LITERAL} {
            }
{LAZY} {
        }
{USE} {
        }
{UNION_CONSTRUCTOR} {
    driver.ast_builder->set_token(std::string(yytext));
    auto declaration_info = ast::extract_union_constructor_info(yytext);
    driver.ast_builder->register_union_constructor_declaration(declaration_info);
}
{DOUBLE_DOT} {
        
}
{DOT_IDENTIFIER} {
        }
{ASTERISK} {
        }
{SLASH} {
        }
{PLUS} {
        }
{MINUS} {
        }
{EQUAL} {
        }
{ARROW} {
        }
{OP_EQUAL} {
        }
{OP_NOT_EQUAL} {
        }
{OP_MORE_EQUAL} {
        }
{OP_LESS_EQUAL} {
        }
{OP_MORE} {
        }
{OP_LESS} {
        }
{OP_AND} {
        }
{OP_OR} {
        }

{CONSTRUCTOR_DEFINITION} {
            }

{DESTRUCTOR_DEFINITION} {
            }

{SETTER_DEFINITION} {
    std::string token_text = std::string(yytext);
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
        }

{GETTER_DEFINITION} {
            }

{FUNCTION_DEFINITION} {
    driver.ast_builder->set_token(std::string(yytext));
    //auto declaration = to_declaration_info(yytext);
    auto function_name = extract_function_name(std::string(yytext));
    //driver.ast_builder->into_named_scope(declaration.name);
    //auto mangled_argument_names = utils::get_transformed(declaration.argument_names,[this](const auto& name){return driver.ast_builder->register_argument(name);});
    //driver.ast_builder->break_out_of_scope();
    driver.ast_builder->register_function_declaration(function_name);
    }

{FUNCTION_DEFINITION} {
    driver.ast_builder->set_token(std::string(yytext));
    auto method_name = extract(std::string(yytext),"method");
    driver.ast_builder->register_method_declaration(method_name,current_structure_name);
}

{STRUCTURE_DEFINITION} {
    driver.ast_builder->set_token(std::string(yytext));
    auto name = extract_structure_name(yytext);
    current_structure_name = name;
    driver.ast_builder->register_structure_declaration(name);
    }

{CLASS_DEFINITION} {
    driver.ast_builder->set_token(std::string(yytext));
        }

{CONSTRAINT} {
        }

{COLON} {
        }
{SEMICOLON} {
        }
{COMMA} {
        }
{LEFT_PARENTHESIS} {
        }
{LEFT_PARENTHESIS_AND_EOL} {
        
        
    }
{RIGHT_PARENTHESIS} {
        }
{LEFT_BRACKET} {
        }
{RIGHT_BRACKET} {
        }
{LEFT_CURLY_BRACE} {
        }
{RIGHT_CURLY_BRACE} {
        }
{IF} {
        }
{ELSE} {
        }
{FOR} {
        }
{IN} {
        }
{WHILE} {
        }
{GLOBAL} {
        }
{IMPORT} {
        }

{RETURN} {
        }

{MEMBER_IDENTIFIER} {
            }
{IDENTIFIER} {
            }
[ \t] {}
<<EOF>> {
            return 0;
    
        }
%%