%skeleton "lalr1.cc"
%require  "3.0"
%debug
%defines 
%define api.namespace {clawn::compiler}
%define api.parser.class {Parser}
%locations
%language "c++"

%{
    #define YYDEBUG 1
%}

%code requires{
    #include <utility>
    #include <memory>
    #include "include/ast/ast.hpp"
    #include "include/ast/builder.hpp"
    #include "include/requirement/requirement.hpp"
    #include "include/compiler/constants.hpp"
 
    namespace clawn::compiler {
        class Driver;
        class Lexer;
        struct DeclarationInfo;
        class UnionConstructorInfo;
        struct RepetationInfo
    {
        std::unique_ptr<clawn::ast::AST> counter;
        std::string counter_name;
        std::unique_ptr<clawn::ast::AST> condition;
        RepetationInfo()=delete;
        RepetationInfo(std::unique_ptr<clawn::ast::AST>&& counter,
        std::string counter_name,
        std::unique_ptr<clawn::ast::AST>&& condition)
            :counter(std::move(counter)),
            counter_name(counter_name),
            condition(std::move(condition)){}
    };
    }

    // The following definitions is missing when %locations isn't used
    # ifndef YY_NULLPTR
    #  if defined __cplusplus && 201103L <= __cplusplus
    #   define YY_NULLPTR nullptr
    #  else
    #   define YY_NULLPTR 0
    #  endif
    # endif
}

%parse-param { Lexer  &lexer  }
%parse-param { Driver  &driver  }

%code{
    #include <iostream>
    #include <cstdlib>
    #include <fstream>
    #include <memory>
    #include "include/compiler/parser/driver.hpp"
    #undef yylex
    #define yylex lexer.yylex
    enum CLAWN_STATE
    {
        EXIST_IF = 0,
        NO_IF = 1,
        GLOBAL = 2,
        NOT_GLOBAL = 3
    };
    CLAWN_STATE clawn_state = NO_IF;
    CLAWN_STATE is_global = NOT_GLOBAL;
    unsigned int VARIOUS_EXPRESSIONS_COUNT = 0;
    namespace clawn::compiler
    {
    }
}

%define api.value.type variant
%define parse.assert
%token <std::string> MEMBER_IDENTIFIER
%token <std::string> IDENTIFIER
%right RETURN

%right EQUAL LEFT_ARROW BOTH_SIDE_ARROW
%right VARIOUS
%left   REFER
%left   ACCESS
%left  SEMICOLON
        LEFT_BRACKET RIGHT_BRACKET
%left   LEFT_CURLY_BRACE RIGHT_CURLY_BRACE
%left   LEFT_PARENTHESIS RIGHT_PARENTHESIS
%token  LEFT_PARENTHESIS_AND_EOL
%left OP_AND OP_OR
%left OP_EQUAL OP_NOT_EQUAL OP_MORE_EQUAL OP_LESS_EQUAL OP_MORE OP_LESS
%left PLUS MINUS
%left ASTERISK SLASH
%left UMINUS
%left DOUBLE_DOT TRIPLE_DOT
%left <std::string> DOT_IDENTIFIER
%left <std::string> RIGHT_ARROW_IDENTIFIER
%left <std::unique_ptr<ast::UnionConstructorInfo>> UNION_CONSTRUCTOR

%token <std::string> C_TYPE_DECLARATION
%token <std::string> C_FUNCTION_DECLARATION
%token C_FUNCTION_ARGUMENT
%token C_FUNCTION_RETURN
%token <std::string> FUNCTION_DEFINITION
%token <std::string> METHOD_DEFINITION
%token <std::unique_ptr<clawn::compiler::DeclarationInfo>> CONSTRUCTOR_DEFINITION
%token <std::unique_ptr<clawn::compiler::DeclarationInfo>> DESTRUCTOR_DEFINITION
%token <std::unique_ptr<clawn::compiler::DeclarationInfo>> SETTER_DEFINITION
%token <std::unique_ptr<clawn::compiler::DeclarationInfo>> GETTER_DEFINITION
%token <std::unique_ptr<clawn::compiler::DeclarationInfo>> CLASS_DEFINITION
%token <std::string> STRUCTURE_DEFINITION
%token <std::string> UNION_DEFINITION
%token REQUIREMENT


%token  SINGLE_QUOTATION
%token  USE
%token  LAZY
%token  COMMA
        COLON
        ELSE
        IF
        REPEAT
        MATCH
        ARM
        DEFAULT
        IN
        WHILE
        GLOBAL
        IMPORT
%token <std::string> STRING_LITERAL
%token <long long> INT_LITERAL
%token <double> FLOAT_LITERAL
%type <std::unique_ptr<ast::AST>> program

%type <std::vector<std::unique_ptr<ast::AST>>> lines
%type <std::unique_ptr<ast::AST>> line
%type <std::unique_ptr<ast::AST>> block
%type <std::unique_ptr<ast::AST>> definition
%type <std::vector<std::shared_ptr<requirement::Requirement>>> requirements
%type <std::shared_ptr<requirement::Requirement>> requirement_clause
%type <std::unique_ptr<ast::AST>> assign_variable
%type <std::unique_ptr<ast::AST>> global_variable_definition
%type <std::unique_ptr<ast::AST>> function_definition
%type <std::unique_ptr<ast::AST>> method_definition
%type <std::unique_ptr<ast::AST>> union_definition
%type <std::unique_ptr<ast::AST>> structure_definition
%type <std::unique_ptr<ast::AST>> C_type_declaration
%type <std::unique_ptr<ast::AST>> C_function_declaration
%type <std::vector<std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>>>> parameters
%type <std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>>> parameter
%type <std::vector<std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>>>> elements
%type <std::vector<std::tuple<std::string,std::optional<std::shared_ptr<requirement::Requirement>>,std::optional<std::unique_ptr<ast::AST>> >>> structure_elements
%type <std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>>> element
%type <std::vector<const std::string>> C_type_descriptor
%type <std::vector<std::pair<const std::string,std::vector<const std::string>>>> C_members_descriptions
%type <std::vector<std::vector<const std::string>>> C_arguments
%type <std::string> function_start
%type <std::string> method_start
%type <std::string> structure_start
%type <std::vector<std::unique_ptr<ast::AST>>> expressions
%type <std::vector<std::pair<std::string,std::unique_ptr<ast::AST>>>> named_arguments
%type <std::unique_ptr<ast::AST>> else_body
%type <std::string> pattern_start
%type <std::pair<std::string,std::unique_ptr<ast::AST>>> pattern
%type <std::unique_ptr<ast::AST>> default_pattern
%type <std::vector<std::pair<std::string,std::unique_ptr<ast::AST>>>> patterns
%type <std::unique_ptr<ast::AST>> expression
%type <std::unique_ptr<RepetationInfo>> repeat_start
%type <std::pair<std::string,std::unique_ptr<ast::AST>>> repeat_first_start
%type <std::unique_ptr<ast::AST>> access
%type <std::unique_ptr<ast::AST>> call
%type <std::unique_ptr<ast::AST>> construction
%type <std::unique_ptr<ast::AST>> monomial
%type <std::unique_ptr<ast::AST>> return_value
%type <std::unique_ptr<ast::AST>> named_value
%token EOL
%token END 0 "end of file" 

/*
def f()
    retunr val;

def f()
    if:
        return val;
    else:
        return val;
    return val;
*/


%%
program: 
    lines
    {
        driver.ast_builder->break_out_of_scope();
        driver.ast_builder->create_root(std::move($1));
        $$ = driver.ast_builder->create_integer(0);
    };  
EOL_OR_EOF: EOL | END;

lines:
    line
    {
        $$.push_back(std::move($1));
    }
    |lines line
    {
        $$ = std::move($1);
        $$.push_back(std::move($2));
    };
line:
    IMPORT STRING_LITERAL EOL_OR_EOF
    {
        $$ = driver.ast_builder->create_block({});
    }
    |expression EOL_OR_EOF
    {
        $$ = std::move($1);
    }
    |requirement_definition EOL_OR_EOF
    {
        $$ = driver.ast_builder->create_block({});
    }
    |C_type_declaration
    {
        //Due to parsing EOL,C_type_declaration is in `line` rule.
        $$ = std::move($1);
    };
definition:
    function_definition
    {
        $$ = std::move($1);
    }
    |union_definition
    {
        $$ = std::move($1);
    }
    |structure_definition
    {
        $$ = std::move($1);
    }
    |C_function_declaration
    {
        $$ = std::move($1);
    };
requirement_definition:
    REQUIREMENT IDENTIFIER EOL LEFT_CURLY_BRACE EOL requirements RIGHT_CURLY_BRACE
    {
        driver.requirement_builder->create_named_requirement_list($2,std::move($6));
    };
requirements:
    requirement_clause EOL
    {
        $$.push_back($1);
    }
    |requirements requirement_clause EOL
    {
        $$ = std::move($1);
        $$.push_back($2);
    };
requirement_clause:
    IDENTIFIER
    {
        // reference_to / value
        $$ = driver.requirement_builder->create_requirement($1);
    }
    |IDENTIFIER COLON IDENTIFIER
    {
        // reference_to: integer
        $$ = driver.requirement_builder->create_requirement($1,$3);
    }
    |IDENTIFIER COLON IDENTIFIER COLON IDENTIFIER
    {
        // has member:integer
        $$ = driver.requirement_builder->create_requirement($1,$3,$5);
    };
block:
    LEFT_CURLY_BRACE lines RIGHT_CURLY_BRACE
    {
        $$ = driver.ast_builder->create_block(std::move($2));
    }
    |LEFT_CURLY_BRACE EOL lines RIGHT_CURLY_BRACE
    {
        $$ = driver.ast_builder->create_block(std::move($3));
    };
function_definition:
    function_start LEFT_PARENTHESIS parameters RIGHT_PARENTHESIS block
    {
        auto function_name = $1;
        $$ = driver.ast_builder->create_generic_function_definition(function_name,std::move($3),std::move($5));
        driver.ast_builder->break_out_of_scope();
    }
    |function_start LEFT_PARENTHESIS parameters RIGHT_PARENTHESIS EOL block
    {
        auto function_name = $1;
        $$ = driver.ast_builder->create_generic_function_definition(function_name,std::move($3),std::move($6));
        driver.ast_builder->break_out_of_scope();
    }
    |function_start LEFT_PARENTHESIS RIGHT_PARENTHESIS block
    {
        auto function_name = $1;
        $$ = driver.ast_builder->create_generic_function_definition(function_name,{},std::move($4));
        driver.ast_builder->break_out_of_scope();
    }
    |function_start LEFT_PARENTHESIS RIGHT_PARENTHESIS EOL block
    {
        auto function_name = $1;
        $$ = driver.ast_builder->create_generic_function_definition(function_name,{},std::move($5));
        driver.ast_builder->break_out_of_scope();
    };
function_start:
    FUNCTION_DEFINITION
    {
        //auto name = $1->name;
        driver.ast_builder->into_named_scope($1);
        
        $$ = $1;
    };
method_definition:
    method_start LEFT_PARENTHESIS parameters RIGHT_PARENTHESIS block
    {
        auto method_name = $1;
        $$ = driver.ast_builder->create_generic_function_definition(method_name,std::move($3),std::move($5));
        driver.ast_builder->break_out_of_scope();
    }
    |method_start LEFT_PARENTHESIS parameters RIGHT_PARENTHESIS EOL block
    {
        auto method_name = $1;
        $$ = driver.ast_builder->create_generic_function_definition(method_name,std::move($3),std::move($6));
        driver.ast_builder->break_out_of_scope();
    };
method_start:
    METHOD_DEFINITION
    {
        //auto name = $1->name;
        driver.ast_builder->into_named_scope($1);
        $$ = $1;
    };
parameters:
    parameter
    {
        $$.push_back($1);
    }
    |parameters COMMA parameter
    {
        $$ = std::move($1);
        $$.push_back($3);
    };
parameter:
    IDENTIFIER
    {
        $$ = std::make_pair(driver.ast_builder->register_argument($1),std::optional<std::shared_ptr<requirement::Requirement>>(std::nullopt));
    }
    |IDENTIFIER COLON requirement_clause
    {
        $$ = std::make_pair(driver.ast_builder->register_argument($1),std::optional<std::shared_ptr<requirement::Requirement>>($3));
    };
structure_definition:
    structure_start EOL LEFT_CURLY_BRACE EOL elements RIGHT_CURLY_BRACE
    {
        //methds are not yet implemented
        $$ = driver.ast_builder->create_generic_structure_definition($1,$5);
        driver.ast_builder->break_out_of_scope();
    }
    |structure_start EOL LEFT_CURLY_BRACE EOL RIGHT_CURLY_BRACE
    {
        $$ = driver.ast_builder->create_generic_structure_definition($1,{});
        driver.ast_builder->break_out_of_scope();
    };
structure_start:
    STRUCTURE_DEFINITION
    {
        $$ = $1;
        driver.ast_builder->into_named_scope($1);
    };
union_definition:
    UNION_DEFINITION EOL LEFT_CURLY_BRACE EOL elements RIGHT_CURLY_BRACE
    {
        $$ = driver.ast_builder->create_generic_union_definition($1,$5);
    };
structure_elements:
    element EOL
    {
        $$.push_back(std::move($1));
    }
    |method_definition EOL
    {
        $$.push_back(std::move($1));
    }
    |structure_elements element EOL
    {
        $$ = std::move($1);
        $$.push_back($2);
    }
    |structure_elements method_definition EOL
    {
        $$ = std::move($1);
        $$.push_back($2);
    };
elements:
    element EOL
    {
        $$.push_back($1);
    }
    |elements element EOL
    {
        $$ = std::move($1);
        $$.push_back($2);
    };
element:
    IDENTIFIER
    {
        $$ = std::make_pair($1,std::optional<std::shared_ptr<requirement::Requirement>>(std::nullopt));
    }
    |IDENTIFIER COLON requirement_clause
    {
        $$ = std::make_pair($1,std::optional<std::shared_ptr<requirement::Requirement>>($3));
    };

C_type_declaration:
    C_TYPE_DECLARATION EOL_OR_EOF
    {
        $$ = driver.ast_builder->create_C_type_declaration($1,{});
    }
    |C_TYPE_DECLARATION EOL C_members_descriptions
    {
        $$ = driver.ast_builder->create_C_type_declaration($1,$3);
    };
C_function_declaration:
    C_FUNCTION_DECLARATION EOL C_FUNCTION_ARGUMENT C_arguments EOL C_FUNCTION_RETURN C_type_descriptor
    {
        $$ = driver.ast_builder->create_C_function_declaration($1,$4,$7);
    }
    |C_FUNCTION_DECLARATION EOL C_FUNCTION_ARGUMENT EOL C_FUNCTION_RETURN C_type_descriptor
    {
        $$ = driver.ast_builder->create_C_function_declaration($1,{},$6);
    };
C_members_descriptions:
    MEMBER_IDENTIFIER COLON C_type_descriptor EOL_OR_EOF
    {
        $$.push_back(std::make_pair($1,$3));
    }
    |C_members_descriptions MEMBER_IDENTIFIER COLON C_type_descriptor EOL_OR_EOF
    {
        $$ = std::move($1);
        $$.push_back(std::make_pair($2,$4));
    };
C_type_descriptor:
    IDENTIFIER
    {
        $$.push_back($1);
    }
    |C_type_descriptor IDENTIFIER
    {
        $$ = std::move($1);
        $$.push_back($2);
    };
C_arguments:
    C_type_descriptor
    {
        $$.push_back($1);
    }
    |C_arguments COMMA C_type_descriptor
    {
        $$ = std::move($1);
        $$.push_back($3);
    };

expressions:
    expression
    {
        $$.push_back(std::move($1));
    }
    |expressions COMMA expression
    {
        $$ = std::move($1);
        $$.push_back(std::move($3));
    };
if_start:
    IF
    {
        driver.ast_builder->into_anonymous_scope();
    };
else_start:
    ELSE
    {
        driver.ast_builder->break_out_of_scope();
        driver.ast_builder->into_anonymous_scope();
    };
repeat_start:
    repeat_first_start COLON expression
    {
        $$ = std::make_unique<RepetationInfo>(std::move($1.second),$1.first,std::move($3));
    };

repeat_first_start:
    REPEAT IDENTIFIER
    {
        driver.ast_builder->into_anonymous_scope();
        auto counter = driver.ast_builder->create_assignment_or_variable_definition(
            $2,driver.ast_builder->create_integer(0)
        );
        $$ = std::make_pair($2,std::move(counter));
    };


else_body:
    else_start EOL block
    {
        $$ = std::move($3);
        driver.ast_builder->break_out_of_scope();
    };
pattern_start:
    IDENTIFIER
    {
        $$ = $1;
        driver.ast_builder->into_anonymous_scope();
    };
default_pattern_start:
    DEFAULT
    {
        driver.ast_builder->into_anonymous_scope();
    };
pattern:
    pattern_start ARM expression
    {
        std::vector<std::unique_ptr<ast::AST>> body;
        body.push_back(std::move($3));
        $$ = std::make_pair($1,driver.ast_builder->create_block(std::move(body)));
        driver.ast_builder->break_out_of_scope();      
    }
    |pattern_start ARM block
    {
        $$ = std::make_pair($1,std::move($3));
        driver.ast_builder->break_out_of_scope();
    };
default_pattern:
    default_pattern_start ARM expression
    {
        std::vector<std::unique_ptr<ast::AST>> body;
        body.push_back(std::move($3));
        $$ = driver.ast_builder->create_block(std::move(body));
        driver.ast_builder->break_out_of_scope();
    }
    |default_pattern_start ARM block
    {
        $$ = std::move($3);
        driver.ast_builder->break_out_of_scope();
    };
patterns:
    pattern
    {
        $$.push_back(std::move($1));
    }
    |patterns EOL pattern
    {
        $$ = std::move($1);
        $$.push_back(std::move($3));
    };
match_start:MATCH;
expression:
    definition
    {
        $$ = std::move($1);
    }
    |block
    {
        $$ = std::move($1);
    }
    |return_value
    {
        $$ = driver.ast_builder->create_return(std::move($1));
        //$$ = std::move($1);
    }
    |ARM expression
    {
        $$ = driver.ast_builder->create_set_result(std::move($2));
    }
    |if_start expression EOL block else_body
    {
        $$ = driver.ast_builder->create_if(std::move($2),std::move($4),std::move($5));
    }
    |if_start expression EOL block
    {
        $$ = driver.ast_builder->create_if(std::move($2),std::move($4), driver.ast_builder->create_block({}));
        driver.ast_builder->break_out_of_scope();
    }
 
    |repeat_start EOL block
    {
        $$ = driver.ast_builder->create_repeat($1->counter_name,std::move($1->counter),std::move($1->condition),std::move($3));
        driver.ast_builder->break_out_of_scope();
    }
    |match_start expression EOL LEFT_CURLY_BRACE EOL patterns EOL RIGHT_CURLY_BRACE
    {
        $$ = driver.ast_builder->create_match(std::move($2),std::move($6));
    }
    |match_start expression EOL LEFT_CURLY_BRACE EOL patterns EOL default_pattern EOL RIGHT_CURLY_BRACE
    {
        $$ = driver.ast_builder->create_match(std::move($2),std::move($6),std::move($8));
    }
    |assign_variable
    {
        $$ = std::move($1);
    }
    |global_variable_definition
    {
        $$ = std::move($1);
    }
    |expression LEFT_ARROW expression
    {
        $$ = driver.ast_builder->create_assignment(driver.ast_builder->create_dereference(std::move($1)),std::move($3));
       //$$ = driver.ast_builder->create_deep_copy(std::move($1),std::move($3));
    }
    |expression BOTH_SIDE_ARROW expression
    {
        $$ = driver.ast_builder->create_assignment(std::move($1),std::move($3),true);
       //$$ = driver.ast_builder->create_deep_copy(std::move($1),std::move($3));
    }
    |expression PLUS expression
    {
        $$ = driver.ast_builder->create_binary_expression(std::move($1),std::move($3),ast::OperatorKind::Addition);
    }
    |expression MINUS expression
    {
        $$ = driver.ast_builder->create_binary_expression(std::move($1),std::move($3),ast::OperatorKind::Subtraction);
    }
    |expression ASTERISK expression
    {
        $$ = driver.ast_builder->create_binary_expression(std::move($1),std::move($3),ast::OperatorKind::Multiplication);
    }
    |expression SLASH expression
    {
        $$ = driver.ast_builder->create_binary_expression(std::move($1),std::move($3),ast::OperatorKind::Division);
    }
    |expression OP_AND expression
    {
        $$ = driver.ast_builder->create_binary_expression(std::move($1),std::move($3),ast::OperatorKind::And);
    }
    |expression OP_OR expression
    {
        $$ = driver.ast_builder->create_binary_expression(std::move($1),std::move($3),ast::OperatorKind::Or);
    }
    |expression OP_MORE_EQUAL expression
    {
        $$ = driver.ast_builder->create_binary_expression(std::move($1),std::move($3),ast::OperatorKind::GreaterThanOrEqual);
    }
    |expression OP_LESS_EQUAL expression
    {
        $$ = driver.ast_builder->create_binary_expression(std::move($1),std::move($3),ast::OperatorKind::LessThanOrEqual);
    }
    |expression OP_MORE expression
    {
        $$ = driver.ast_builder->create_binary_expression(std::move($1),std::move($3),ast::OperatorKind::GreaterThan);
    }
    |expression OP_LESS expression
    {
        $$ = driver.ast_builder->create_binary_expression(std::move($1),std::move($3),ast::OperatorKind::LessThan);
    }
    |expression OP_NOT_EQUAL expression
    {
        $$ = driver.ast_builder->create_binary_expression(std::move($1),std::move($3),ast::OperatorKind::NotEqual);
    }
    |expression OP_EQUAL expression
    {
        $$ = driver.ast_builder->create_binary_expression(std::move($1),std::move($3),ast::OperatorKind::Equal);
    }
    |expression LEFT_BRACKET expression RIGHT_BRACKET
    {
        $$ = driver.ast_builder->create_access_list(std::move($1),std::move($3));
    }
    |expression DOUBLE_DOT IDENTIFIER
    {
        $$ = driver.ast_builder->create_common_access(std::move($1),std::move($3));
    }
    |expression DOUBLE_DOT LEFT_PARENTHESIS expressions RIGHT_PARENTHESIS
    {
        $$ = driver.ast_builder->create_common_call(std::move($1),std::move($4));
    }
    
    |REFER expression
    {
        $$ = driver.ast_builder->create_reference(std::move($2));
    }
    |ACCESS expression
    {
        $$ = driver.ast_builder->create_dereference(std::move($2));
    }
    |VARIOUS expression
    {
        $$ = driver.ast_builder->create_various(std::move($2));
    }
    |monomial
    {
        $$ = std::move($1);
    }
    |UNION_CONSTRUCTOR
    {
        $$ = driver.ast_builder->create_union_constructor(*$1);
    }
    |access
    {
        $$ = std::move($1);
    }
    |MINUS expression %prec UMINUS
    {
        $$ = driver.ast_builder->create_minus(std::move($2));
    }
    |LEFT_PARENTHESIS expression RIGHT_PARENTHESIS
    {
        $$ = std::move($2);
    };

access:
    expression DOT_IDENTIFIER
    {
        $$ = driver.ast_builder->create_access(std::move($1),$2);
    }
    |expression RIGHT_ARROW_IDENTIFIER
    {
        $$ = driver.ast_builder->create_access(driver.ast_builder->create_dereference(std::move($1)),$2);
    };
global_variable_definition:
    GLOBAL IDENTIFIER EQUAL expression
    {
        $$ = driver.ast_builder->create_global_variable_definition($2,std::move($4));
    };
assign_variable:
    IDENTIFIER EQUAL expression
    {
        if (driver.ast_builder->exist_named_expression($1))
        {
            auto named_expression = driver.ast_builder->create_named_node($1);
            $$ = driver.ast_builder->create_assignment(std::move(named_expression),std::move($3));
        }
        else
        {
            $$ = driver.ast_builder->create_assignment_or_variable_definition($1,std::move($3));
        }
    }
    |expression EQUAL expression
    {
        $$ = driver.ast_builder->create_assignment(std::move($1),std::move($3));
    };
return_value:
    RETURN expression 
    {
        $$ = std::move($2);
    };
monomial:
    call
    {
        $$ = std::move($1);
    }
    |construction
    {
        $$ = std::move($1);
    }
    |STRING_LITERAL
    {
        $$ = driver.ast_builder->create_string($1);
    }
    |FLOAT_LITERAL
    {
        $$ = driver.ast_builder->create_float($1);
    }
    |INT_LITERAL
    { 
        $$ = driver.ast_builder->create_integer($1);
    }
    |LEFT_BRACKET RIGHT_BRACKET
    {
        $$ = driver.ast_builder->create_list();
    }
    |LEFT_BRACKET expressions RIGHT_BRACKET
    {   
        $$ = driver.ast_builder->create_list(std::move($2));
    }
    |named_value
    {
        $$ = std::move($1);
    };
    
call:
    IDENTIFIER LEFT_PARENTHESIS expressions RIGHT_PARENTHESIS
    {
        if (driver.ast_builder->exist_named_expression($1))
        {
            auto function_object = driver.ast_builder->create_named_node($1);
            $$ = driver.ast_builder->create_call(std::move(function_object),std::move($3));
        }
        else
        {
            $$ = driver.ast_builder->create_call($1,std::move($3));
        }
    }
    |IDENTIFIER LEFT_PARENTHESIS RIGHT_PARENTHESIS
    {
        if (driver.ast_builder->exist_named_expression($1))
        {
            auto function_object = driver.ast_builder->create_named_node($1);
            $$ = driver.ast_builder->create_call(std::move(function_object),{});
        }
        else
        {
            $$ = driver.ast_builder->create_call($1,{});
        }
    }
    |expression LEFT_PARENTHESIS expressions RIGHT_PARENTHESIS
    {
        $$ = driver.ast_builder->create_call(std::move($1),std::move($3));
    }
    |expression LEFT_PARENTHESIS RIGHT_PARENTHESIS
    {
        $$ = driver.ast_builder->create_call(std::move($1),{});
    };
construction:
    expression LEFT_CURLY_BRACE named_arguments RIGHT_CURLY_BRACE
    {
        $$ = driver.ast_builder->create_construction(std::move($1),std::move($3));
    }
    |expression LEFT_CURLY_BRACE RIGHT_CURLY_BRACE
    {
        $$ = driver.ast_builder->create_construction(std::move($1),{});
    };

named_arguments:
    IDENTIFIER COLON expression
    {
        $$.push_back(std::make_pair($1,std::move($3)));
    }
    |named_arguments COMMA IDENTIFIER COLON expression
    {
        $$ = std::move($1);
        $$.push_back(std::make_pair($3,std::move($5)));
    };

named_value:
    IDENTIFIER
    {
        $$ = driver.ast_builder->create_named_node($1);
    };
%%

void clawn::compiler::Parser::error( const location_type &l, const std::string &err_message )
{
    auto debug_info = driver.ast_builder->create_current_debug_info();
    auto& token_info = debug_info->get_token_info();
    //auto& scope = debug_info.get_scope_id();
    auto& filename = debug_info->get_filename();
    std::cerr << "Syntax error at '" <<
    token_info.get_token_string() <<
    "' in line " << token_info.get_line_number() << " in file '"
    << filename << "'" << std::endl;
    exit(1);
} 