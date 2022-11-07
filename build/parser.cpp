// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.



// First part of user prologue.
#line 10 "./src/lib/compiler/parser/parser.yy"

    #define YYDEBUG 1

#line 45 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"


#include "parser.hpp"


// Unqualified %code blocks.
#line 55 "./src/lib/compiler/parser/parser.yy"

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

#line 75 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 5 "./src/lib/compiler/parser/parser.yy"
namespace clawn { namespace compiler {
#line 168 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"

  /// Build a parser object.
  Parser::Parser (Lexer  &lexer_yyarg, Driver  &driver_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      lexer (lexer_yyarg),
      driver (driver_yyarg)
  {}

  Parser::~Parser ()
  {}

  Parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/

  // basic_symbol.
  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->kind ())
    {
      case symbol_kind::S_FLOAT_LITERAL: // FLOAT_LITERAL
        value.copy< double > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INT_LITERAL: // INT_LITERAL
        value.copy< long long > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_parameter: // parameter
      case symbol_kind::S_element: // element
        value.copy< std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_repeat_first_start: // repeat_first_start
      case symbol_kind::S_pattern: // pattern
        value.copy< std::pair<std::string,std::unique_ptr<ast::AST>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_requirement_clause: // requirement_clause
        value.copy< std::shared_ptr<requirement::Requirement> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_MEMBER_IDENTIFIER: // MEMBER_IDENTIFIER
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_DOT_IDENTIFIER: // DOT_IDENTIFIER
      case symbol_kind::S_RIGHT_ARROW_IDENTIFIER: // RIGHT_ARROW_IDENTIFIER
      case symbol_kind::S_C_TYPE_DECLARATION: // C_TYPE_DECLARATION
      case symbol_kind::S_C_FUNCTION_DECLARATION: // C_FUNCTION_DECLARATION
      case symbol_kind::S_FUNCTION_DEFINITION: // FUNCTION_DEFINITION
      case symbol_kind::S_METHOD_DEFINITION: // METHOD_DEFINITION
      case symbol_kind::S_STRUCTURE_DEFINITION: // STRUCTURE_DEFINITION
      case symbol_kind::S_UNION_DEFINITION: // UNION_DEFINITION
      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
      case symbol_kind::S_function_start: // function_start
      case symbol_kind::S_structure_start: // structure_start
      case symbol_kind::S_pattern_start: // pattern_start
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_repeat_start: // repeat_start
        value.copy< std::unique_ptr<RepetationInfo> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_program: // program
      case symbol_kind::S_line: // line
      case symbol_kind::S_definition: // definition
      case symbol_kind::S_block: // block
      case symbol_kind::S_function_definition: // function_definition
      case symbol_kind::S_structure_definition: // structure_definition
      case symbol_kind::S_union_definition: // union_definition
      case symbol_kind::S_C_type_declaration: // C_type_declaration
      case symbol_kind::S_C_function_declaration: // C_function_declaration
      case symbol_kind::S_else_body: // else_body
      case symbol_kind::S_default_pattern: // default_pattern
      case symbol_kind::S_expression: // expression
      case symbol_kind::S_access: // access
      case symbol_kind::S_global_variable_definition: // global_variable_definition
      case symbol_kind::S_assign_variable: // assign_variable
      case symbol_kind::S_return_value: // return_value
      case symbol_kind::S_monomial: // monomial
      case symbol_kind::S_call: // call
      case symbol_kind::S_construction: // construction
      case symbol_kind::S_named_value: // named_value
        value.copy< std::unique_ptr<ast::AST> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_UNION_CONSTRUCTOR: // UNION_CONSTRUCTOR
        value.copy< std::unique_ptr<ast::UnionConstructorInfo> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_CONSTRUCTOR_DEFINITION: // CONSTRUCTOR_DEFINITION
      case symbol_kind::S_DESTRUCTOR_DEFINITION: // DESTRUCTOR_DEFINITION
      case symbol_kind::S_SETTER_DEFINITION: // SETTER_DEFINITION
      case symbol_kind::S_GETTER_DEFINITION: // GETTER_DEFINITION
      case symbol_kind::S_CLASS_DEFINITION: // CLASS_DEFINITION
        value.copy< std::unique_ptr<clawn::compiler::DeclarationInfo> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_C_type_descriptor: // C_type_descriptor
        value.copy< std::vector<const std::string> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_C_members_descriptions: // C_members_descriptions
        value.copy< std::vector<std::pair<const std::string,std::vector<const std::string>>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_parameters: // parameters
      case symbol_kind::S_elements: // elements
        value.copy< std::vector<std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_patterns: // patterns
      case symbol_kind::S_named_arguments: // named_arguments
        value.copy< std::vector<std::pair<std::string,std::unique_ptr<ast::AST>>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_requirements: // requirements
        value.copy< std::vector<std::shared_ptr<requirement::Requirement>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_lines: // lines
      case symbol_kind::S_expressions: // expressions
        value.copy< std::vector<std::unique_ptr<ast::AST>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_C_arguments: // C_arguments
        value.copy< std::vector<std::vector<const std::string>> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }




  template <typename Base>
  Parser::symbol_kind_type
  Parser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  Parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  Parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
    {
      case symbol_kind::S_FLOAT_LITERAL: // FLOAT_LITERAL
        value.move< double > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_INT_LITERAL: // INT_LITERAL
        value.move< long long > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_parameter: // parameter
      case symbol_kind::S_element: // element
        value.move< std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_repeat_first_start: // repeat_first_start
      case symbol_kind::S_pattern: // pattern
        value.move< std::pair<std::string,std::unique_ptr<ast::AST>> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_requirement_clause: // requirement_clause
        value.move< std::shared_ptr<requirement::Requirement> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_MEMBER_IDENTIFIER: // MEMBER_IDENTIFIER
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_DOT_IDENTIFIER: // DOT_IDENTIFIER
      case symbol_kind::S_RIGHT_ARROW_IDENTIFIER: // RIGHT_ARROW_IDENTIFIER
      case symbol_kind::S_C_TYPE_DECLARATION: // C_TYPE_DECLARATION
      case symbol_kind::S_C_FUNCTION_DECLARATION: // C_FUNCTION_DECLARATION
      case symbol_kind::S_FUNCTION_DEFINITION: // FUNCTION_DEFINITION
      case symbol_kind::S_METHOD_DEFINITION: // METHOD_DEFINITION
      case symbol_kind::S_STRUCTURE_DEFINITION: // STRUCTURE_DEFINITION
      case symbol_kind::S_UNION_DEFINITION: // UNION_DEFINITION
      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
      case symbol_kind::S_function_start: // function_start
      case symbol_kind::S_structure_start: // structure_start
      case symbol_kind::S_pattern_start: // pattern_start
        value.move< std::string > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_repeat_start: // repeat_start
        value.move< std::unique_ptr<RepetationInfo> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_program: // program
      case symbol_kind::S_line: // line
      case symbol_kind::S_definition: // definition
      case symbol_kind::S_block: // block
      case symbol_kind::S_function_definition: // function_definition
      case symbol_kind::S_structure_definition: // structure_definition
      case symbol_kind::S_union_definition: // union_definition
      case symbol_kind::S_C_type_declaration: // C_type_declaration
      case symbol_kind::S_C_function_declaration: // C_function_declaration
      case symbol_kind::S_else_body: // else_body
      case symbol_kind::S_default_pattern: // default_pattern
      case symbol_kind::S_expression: // expression
      case symbol_kind::S_access: // access
      case symbol_kind::S_global_variable_definition: // global_variable_definition
      case symbol_kind::S_assign_variable: // assign_variable
      case symbol_kind::S_return_value: // return_value
      case symbol_kind::S_monomial: // monomial
      case symbol_kind::S_call: // call
      case symbol_kind::S_construction: // construction
      case symbol_kind::S_named_value: // named_value
        value.move< std::unique_ptr<ast::AST> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_UNION_CONSTRUCTOR: // UNION_CONSTRUCTOR
        value.move< std::unique_ptr<ast::UnionConstructorInfo> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_CONSTRUCTOR_DEFINITION: // CONSTRUCTOR_DEFINITION
      case symbol_kind::S_DESTRUCTOR_DEFINITION: // DESTRUCTOR_DEFINITION
      case symbol_kind::S_SETTER_DEFINITION: // SETTER_DEFINITION
      case symbol_kind::S_GETTER_DEFINITION: // GETTER_DEFINITION
      case symbol_kind::S_CLASS_DEFINITION: // CLASS_DEFINITION
        value.move< std::unique_ptr<clawn::compiler::DeclarationInfo> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_C_type_descriptor: // C_type_descriptor
        value.move< std::vector<const std::string> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_C_members_descriptions: // C_members_descriptions
        value.move< std::vector<std::pair<const std::string,std::vector<const std::string>>> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_parameters: // parameters
      case symbol_kind::S_elements: // elements
        value.move< std::vector<std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>>> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_patterns: // patterns
      case symbol_kind::S_named_arguments: // named_arguments
        value.move< std::vector<std::pair<std::string,std::unique_ptr<ast::AST>>> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_requirements: // requirements
        value.move< std::vector<std::shared_ptr<requirement::Requirement>> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_lines: // lines
      case symbol_kind::S_expressions: // expressions
        value.move< std::vector<std::unique_ptr<ast::AST>> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_C_arguments: // C_arguments
        value.move< std::vector<std::vector<const std::string>> > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_kind.
  Parser::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  Parser::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  Parser::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  Parser::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  void
  Parser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  Parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  Parser::symbol_kind_type
  Parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  Parser::symbol_kind_type
  Parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }



  // by_state.
  Parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  Parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  Parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  Parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  Parser::symbol_kind_type
  Parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  Parser::stack_symbol_type::stack_symbol_type ()
  {}

  Parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_FLOAT_LITERAL: // FLOAT_LITERAL
        value.YY_MOVE_OR_COPY< double > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INT_LITERAL: // INT_LITERAL
        value.YY_MOVE_OR_COPY< long long > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_parameter: // parameter
      case symbol_kind::S_element: // element
        value.YY_MOVE_OR_COPY< std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_repeat_first_start: // repeat_first_start
      case symbol_kind::S_pattern: // pattern
        value.YY_MOVE_OR_COPY< std::pair<std::string,std::unique_ptr<ast::AST>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_requirement_clause: // requirement_clause
        value.YY_MOVE_OR_COPY< std::shared_ptr<requirement::Requirement> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_MEMBER_IDENTIFIER: // MEMBER_IDENTIFIER
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_DOT_IDENTIFIER: // DOT_IDENTIFIER
      case symbol_kind::S_RIGHT_ARROW_IDENTIFIER: // RIGHT_ARROW_IDENTIFIER
      case symbol_kind::S_C_TYPE_DECLARATION: // C_TYPE_DECLARATION
      case symbol_kind::S_C_FUNCTION_DECLARATION: // C_FUNCTION_DECLARATION
      case symbol_kind::S_FUNCTION_DEFINITION: // FUNCTION_DEFINITION
      case symbol_kind::S_METHOD_DEFINITION: // METHOD_DEFINITION
      case symbol_kind::S_STRUCTURE_DEFINITION: // STRUCTURE_DEFINITION
      case symbol_kind::S_UNION_DEFINITION: // UNION_DEFINITION
      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
      case symbol_kind::S_function_start: // function_start
      case symbol_kind::S_structure_start: // structure_start
      case symbol_kind::S_pattern_start: // pattern_start
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_repeat_start: // repeat_start
        value.YY_MOVE_OR_COPY< std::unique_ptr<RepetationInfo> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_program: // program
      case symbol_kind::S_line: // line
      case symbol_kind::S_definition: // definition
      case symbol_kind::S_block: // block
      case symbol_kind::S_function_definition: // function_definition
      case symbol_kind::S_structure_definition: // structure_definition
      case symbol_kind::S_union_definition: // union_definition
      case symbol_kind::S_C_type_declaration: // C_type_declaration
      case symbol_kind::S_C_function_declaration: // C_function_declaration
      case symbol_kind::S_else_body: // else_body
      case symbol_kind::S_default_pattern: // default_pattern
      case symbol_kind::S_expression: // expression
      case symbol_kind::S_access: // access
      case symbol_kind::S_global_variable_definition: // global_variable_definition
      case symbol_kind::S_assign_variable: // assign_variable
      case symbol_kind::S_return_value: // return_value
      case symbol_kind::S_monomial: // monomial
      case symbol_kind::S_call: // call
      case symbol_kind::S_construction: // construction
      case symbol_kind::S_named_value: // named_value
        value.YY_MOVE_OR_COPY< std::unique_ptr<ast::AST> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_UNION_CONSTRUCTOR: // UNION_CONSTRUCTOR
        value.YY_MOVE_OR_COPY< std::unique_ptr<ast::UnionConstructorInfo> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_CONSTRUCTOR_DEFINITION: // CONSTRUCTOR_DEFINITION
      case symbol_kind::S_DESTRUCTOR_DEFINITION: // DESTRUCTOR_DEFINITION
      case symbol_kind::S_SETTER_DEFINITION: // SETTER_DEFINITION
      case symbol_kind::S_GETTER_DEFINITION: // GETTER_DEFINITION
      case symbol_kind::S_CLASS_DEFINITION: // CLASS_DEFINITION
        value.YY_MOVE_OR_COPY< std::unique_ptr<clawn::compiler::DeclarationInfo> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_C_type_descriptor: // C_type_descriptor
        value.YY_MOVE_OR_COPY< std::vector<const std::string> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_C_members_descriptions: // C_members_descriptions
        value.YY_MOVE_OR_COPY< std::vector<std::pair<const std::string,std::vector<const std::string>>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_parameters: // parameters
      case symbol_kind::S_elements: // elements
        value.YY_MOVE_OR_COPY< std::vector<std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_patterns: // patterns
      case symbol_kind::S_named_arguments: // named_arguments
        value.YY_MOVE_OR_COPY< std::vector<std::pair<std::string,std::unique_ptr<ast::AST>>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_requirements: // requirements
        value.YY_MOVE_OR_COPY< std::vector<std::shared_ptr<requirement::Requirement>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_lines: // lines
      case symbol_kind::S_expressions: // expressions
        value.YY_MOVE_OR_COPY< std::vector<std::unique_ptr<ast::AST>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_C_arguments: // C_arguments
        value.YY_MOVE_OR_COPY< std::vector<std::vector<const std::string>> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  Parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_FLOAT_LITERAL: // FLOAT_LITERAL
        value.move< double > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INT_LITERAL: // INT_LITERAL
        value.move< long long > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_parameter: // parameter
      case symbol_kind::S_element: // element
        value.move< std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_repeat_first_start: // repeat_first_start
      case symbol_kind::S_pattern: // pattern
        value.move< std::pair<std::string,std::unique_ptr<ast::AST>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_requirement_clause: // requirement_clause
        value.move< std::shared_ptr<requirement::Requirement> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_MEMBER_IDENTIFIER: // MEMBER_IDENTIFIER
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_DOT_IDENTIFIER: // DOT_IDENTIFIER
      case symbol_kind::S_RIGHT_ARROW_IDENTIFIER: // RIGHT_ARROW_IDENTIFIER
      case symbol_kind::S_C_TYPE_DECLARATION: // C_TYPE_DECLARATION
      case symbol_kind::S_C_FUNCTION_DECLARATION: // C_FUNCTION_DECLARATION
      case symbol_kind::S_FUNCTION_DEFINITION: // FUNCTION_DEFINITION
      case symbol_kind::S_METHOD_DEFINITION: // METHOD_DEFINITION
      case symbol_kind::S_STRUCTURE_DEFINITION: // STRUCTURE_DEFINITION
      case symbol_kind::S_UNION_DEFINITION: // UNION_DEFINITION
      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
      case symbol_kind::S_function_start: // function_start
      case symbol_kind::S_structure_start: // structure_start
      case symbol_kind::S_pattern_start: // pattern_start
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_repeat_start: // repeat_start
        value.move< std::unique_ptr<RepetationInfo> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_program: // program
      case symbol_kind::S_line: // line
      case symbol_kind::S_definition: // definition
      case symbol_kind::S_block: // block
      case symbol_kind::S_function_definition: // function_definition
      case symbol_kind::S_structure_definition: // structure_definition
      case symbol_kind::S_union_definition: // union_definition
      case symbol_kind::S_C_type_declaration: // C_type_declaration
      case symbol_kind::S_C_function_declaration: // C_function_declaration
      case symbol_kind::S_else_body: // else_body
      case symbol_kind::S_default_pattern: // default_pattern
      case symbol_kind::S_expression: // expression
      case symbol_kind::S_access: // access
      case symbol_kind::S_global_variable_definition: // global_variable_definition
      case symbol_kind::S_assign_variable: // assign_variable
      case symbol_kind::S_return_value: // return_value
      case symbol_kind::S_monomial: // monomial
      case symbol_kind::S_call: // call
      case symbol_kind::S_construction: // construction
      case symbol_kind::S_named_value: // named_value
        value.move< std::unique_ptr<ast::AST> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_UNION_CONSTRUCTOR: // UNION_CONSTRUCTOR
        value.move< std::unique_ptr<ast::UnionConstructorInfo> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_CONSTRUCTOR_DEFINITION: // CONSTRUCTOR_DEFINITION
      case symbol_kind::S_DESTRUCTOR_DEFINITION: // DESTRUCTOR_DEFINITION
      case symbol_kind::S_SETTER_DEFINITION: // SETTER_DEFINITION
      case symbol_kind::S_GETTER_DEFINITION: // GETTER_DEFINITION
      case symbol_kind::S_CLASS_DEFINITION: // CLASS_DEFINITION
        value.move< std::unique_ptr<clawn::compiler::DeclarationInfo> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_C_type_descriptor: // C_type_descriptor
        value.move< std::vector<const std::string> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_C_members_descriptions: // C_members_descriptions
        value.move< std::vector<std::pair<const std::string,std::vector<const std::string>>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_parameters: // parameters
      case symbol_kind::S_elements: // elements
        value.move< std::vector<std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_patterns: // patterns
      case symbol_kind::S_named_arguments: // named_arguments
        value.move< std::vector<std::pair<std::string,std::unique_ptr<ast::AST>>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_requirements: // requirements
        value.move< std::vector<std::shared_ptr<requirement::Requirement>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_lines: // lines
      case symbol_kind::S_expressions: // expressions
        value.move< std::vector<std::unique_ptr<ast::AST>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_C_arguments: // C_arguments
        value.move< std::vector<std::vector<const std::string>> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_FLOAT_LITERAL: // FLOAT_LITERAL
        value.copy< double > (that.value);
        break;

      case symbol_kind::S_INT_LITERAL: // INT_LITERAL
        value.copy< long long > (that.value);
        break;

      case symbol_kind::S_parameter: // parameter
      case symbol_kind::S_element: // element
        value.copy< std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>> > (that.value);
        break;

      case symbol_kind::S_repeat_first_start: // repeat_first_start
      case symbol_kind::S_pattern: // pattern
        value.copy< std::pair<std::string,std::unique_ptr<ast::AST>> > (that.value);
        break;

      case symbol_kind::S_requirement_clause: // requirement_clause
        value.copy< std::shared_ptr<requirement::Requirement> > (that.value);
        break;

      case symbol_kind::S_MEMBER_IDENTIFIER: // MEMBER_IDENTIFIER
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_DOT_IDENTIFIER: // DOT_IDENTIFIER
      case symbol_kind::S_RIGHT_ARROW_IDENTIFIER: // RIGHT_ARROW_IDENTIFIER
      case symbol_kind::S_C_TYPE_DECLARATION: // C_TYPE_DECLARATION
      case symbol_kind::S_C_FUNCTION_DECLARATION: // C_FUNCTION_DECLARATION
      case symbol_kind::S_FUNCTION_DEFINITION: // FUNCTION_DEFINITION
      case symbol_kind::S_METHOD_DEFINITION: // METHOD_DEFINITION
      case symbol_kind::S_STRUCTURE_DEFINITION: // STRUCTURE_DEFINITION
      case symbol_kind::S_UNION_DEFINITION: // UNION_DEFINITION
      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
      case symbol_kind::S_function_start: // function_start
      case symbol_kind::S_structure_start: // structure_start
      case symbol_kind::S_pattern_start: // pattern_start
        value.copy< std::string > (that.value);
        break;

      case symbol_kind::S_repeat_start: // repeat_start
        value.copy< std::unique_ptr<RepetationInfo> > (that.value);
        break;

      case symbol_kind::S_program: // program
      case symbol_kind::S_line: // line
      case symbol_kind::S_definition: // definition
      case symbol_kind::S_block: // block
      case symbol_kind::S_function_definition: // function_definition
      case symbol_kind::S_structure_definition: // structure_definition
      case symbol_kind::S_union_definition: // union_definition
      case symbol_kind::S_C_type_declaration: // C_type_declaration
      case symbol_kind::S_C_function_declaration: // C_function_declaration
      case symbol_kind::S_else_body: // else_body
      case symbol_kind::S_default_pattern: // default_pattern
      case symbol_kind::S_expression: // expression
      case symbol_kind::S_access: // access
      case symbol_kind::S_global_variable_definition: // global_variable_definition
      case symbol_kind::S_assign_variable: // assign_variable
      case symbol_kind::S_return_value: // return_value
      case symbol_kind::S_monomial: // monomial
      case symbol_kind::S_call: // call
      case symbol_kind::S_construction: // construction
      case symbol_kind::S_named_value: // named_value
        value.copy< std::unique_ptr<ast::AST> > (that.value);
        break;

      case symbol_kind::S_UNION_CONSTRUCTOR: // UNION_CONSTRUCTOR
        value.copy< std::unique_ptr<ast::UnionConstructorInfo> > (that.value);
        break;

      case symbol_kind::S_CONSTRUCTOR_DEFINITION: // CONSTRUCTOR_DEFINITION
      case symbol_kind::S_DESTRUCTOR_DEFINITION: // DESTRUCTOR_DEFINITION
      case symbol_kind::S_SETTER_DEFINITION: // SETTER_DEFINITION
      case symbol_kind::S_GETTER_DEFINITION: // GETTER_DEFINITION
      case symbol_kind::S_CLASS_DEFINITION: // CLASS_DEFINITION
        value.copy< std::unique_ptr<clawn::compiler::DeclarationInfo> > (that.value);
        break;

      case symbol_kind::S_C_type_descriptor: // C_type_descriptor
        value.copy< std::vector<const std::string> > (that.value);
        break;

      case symbol_kind::S_C_members_descriptions: // C_members_descriptions
        value.copy< std::vector<std::pair<const std::string,std::vector<const std::string>>> > (that.value);
        break;

      case symbol_kind::S_parameters: // parameters
      case symbol_kind::S_elements: // elements
        value.copy< std::vector<std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>>> > (that.value);
        break;

      case symbol_kind::S_patterns: // patterns
      case symbol_kind::S_named_arguments: // named_arguments
        value.copy< std::vector<std::pair<std::string,std::unique_ptr<ast::AST>>> > (that.value);
        break;

      case symbol_kind::S_requirements: // requirements
        value.copy< std::vector<std::shared_ptr<requirement::Requirement>> > (that.value);
        break;

      case symbol_kind::S_lines: // lines
      case symbol_kind::S_expressions: // expressions
        value.copy< std::vector<std::unique_ptr<ast::AST>> > (that.value);
        break;

      case symbol_kind::S_C_arguments: // C_arguments
        value.copy< std::vector<std::vector<const std::string>> > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_FLOAT_LITERAL: // FLOAT_LITERAL
        value.move< double > (that.value);
        break;

      case symbol_kind::S_INT_LITERAL: // INT_LITERAL
        value.move< long long > (that.value);
        break;

      case symbol_kind::S_parameter: // parameter
      case symbol_kind::S_element: // element
        value.move< std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>> > (that.value);
        break;

      case symbol_kind::S_repeat_first_start: // repeat_first_start
      case symbol_kind::S_pattern: // pattern
        value.move< std::pair<std::string,std::unique_ptr<ast::AST>> > (that.value);
        break;

      case symbol_kind::S_requirement_clause: // requirement_clause
        value.move< std::shared_ptr<requirement::Requirement> > (that.value);
        break;

      case symbol_kind::S_MEMBER_IDENTIFIER: // MEMBER_IDENTIFIER
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_DOT_IDENTIFIER: // DOT_IDENTIFIER
      case symbol_kind::S_RIGHT_ARROW_IDENTIFIER: // RIGHT_ARROW_IDENTIFIER
      case symbol_kind::S_C_TYPE_DECLARATION: // C_TYPE_DECLARATION
      case symbol_kind::S_C_FUNCTION_DECLARATION: // C_FUNCTION_DECLARATION
      case symbol_kind::S_FUNCTION_DEFINITION: // FUNCTION_DEFINITION
      case symbol_kind::S_METHOD_DEFINITION: // METHOD_DEFINITION
      case symbol_kind::S_STRUCTURE_DEFINITION: // STRUCTURE_DEFINITION
      case symbol_kind::S_UNION_DEFINITION: // UNION_DEFINITION
      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
      case symbol_kind::S_function_start: // function_start
      case symbol_kind::S_structure_start: // structure_start
      case symbol_kind::S_pattern_start: // pattern_start
        value.move< std::string > (that.value);
        break;

      case symbol_kind::S_repeat_start: // repeat_start
        value.move< std::unique_ptr<RepetationInfo> > (that.value);
        break;

      case symbol_kind::S_program: // program
      case symbol_kind::S_line: // line
      case symbol_kind::S_definition: // definition
      case symbol_kind::S_block: // block
      case symbol_kind::S_function_definition: // function_definition
      case symbol_kind::S_structure_definition: // structure_definition
      case symbol_kind::S_union_definition: // union_definition
      case symbol_kind::S_C_type_declaration: // C_type_declaration
      case symbol_kind::S_C_function_declaration: // C_function_declaration
      case symbol_kind::S_else_body: // else_body
      case symbol_kind::S_default_pattern: // default_pattern
      case symbol_kind::S_expression: // expression
      case symbol_kind::S_access: // access
      case symbol_kind::S_global_variable_definition: // global_variable_definition
      case symbol_kind::S_assign_variable: // assign_variable
      case symbol_kind::S_return_value: // return_value
      case symbol_kind::S_monomial: // monomial
      case symbol_kind::S_call: // call
      case symbol_kind::S_construction: // construction
      case symbol_kind::S_named_value: // named_value
        value.move< std::unique_ptr<ast::AST> > (that.value);
        break;

      case symbol_kind::S_UNION_CONSTRUCTOR: // UNION_CONSTRUCTOR
        value.move< std::unique_ptr<ast::UnionConstructorInfo> > (that.value);
        break;

      case symbol_kind::S_CONSTRUCTOR_DEFINITION: // CONSTRUCTOR_DEFINITION
      case symbol_kind::S_DESTRUCTOR_DEFINITION: // DESTRUCTOR_DEFINITION
      case symbol_kind::S_SETTER_DEFINITION: // SETTER_DEFINITION
      case symbol_kind::S_GETTER_DEFINITION: // GETTER_DEFINITION
      case symbol_kind::S_CLASS_DEFINITION: // CLASS_DEFINITION
        value.move< std::unique_ptr<clawn::compiler::DeclarationInfo> > (that.value);
        break;

      case symbol_kind::S_C_type_descriptor: // C_type_descriptor
        value.move< std::vector<const std::string> > (that.value);
        break;

      case symbol_kind::S_C_members_descriptions: // C_members_descriptions
        value.move< std::vector<std::pair<const std::string,std::vector<const std::string>>> > (that.value);
        break;

      case symbol_kind::S_parameters: // parameters
      case symbol_kind::S_elements: // elements
        value.move< std::vector<std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>>> > (that.value);
        break;

      case symbol_kind::S_patterns: // patterns
      case symbol_kind::S_named_arguments: // named_arguments
        value.move< std::vector<std::pair<std::string,std::unique_ptr<ast::AST>>> > (that.value);
        break;

      case symbol_kind::S_requirements: // requirements
        value.move< std::vector<std::shared_ptr<requirement::Requirement>> > (that.value);
        break;

      case symbol_kind::S_lines: // lines
      case symbol_kind::S_expressions: // expressions
        value.move< std::vector<std::unique_ptr<ast::AST>> > (that.value);
        break;

      case symbol_kind::S_C_arguments: // C_arguments
        value.move< std::vector<std::vector<const std::string>> > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  Parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  Parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  Parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  Parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  Parser::state_type
  Parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  Parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  Parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Parser::operator() ()
  {
    return parse ();
  }

  int
  Parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.kind_ = yytranslate_ (yylex (&yyla.value, &yyla.location));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_FLOAT_LITERAL: // FLOAT_LITERAL
        yylhs.value.emplace< double > ();
        break;

      case symbol_kind::S_INT_LITERAL: // INT_LITERAL
        yylhs.value.emplace< long long > ();
        break;

      case symbol_kind::S_parameter: // parameter
      case symbol_kind::S_element: // element
        yylhs.value.emplace< std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>> > ();
        break;

      case symbol_kind::S_repeat_first_start: // repeat_first_start
      case symbol_kind::S_pattern: // pattern
        yylhs.value.emplace< std::pair<std::string,std::unique_ptr<ast::AST>> > ();
        break;

      case symbol_kind::S_requirement_clause: // requirement_clause
        yylhs.value.emplace< std::shared_ptr<requirement::Requirement> > ();
        break;

      case symbol_kind::S_MEMBER_IDENTIFIER: // MEMBER_IDENTIFIER
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_DOT_IDENTIFIER: // DOT_IDENTIFIER
      case symbol_kind::S_RIGHT_ARROW_IDENTIFIER: // RIGHT_ARROW_IDENTIFIER
      case symbol_kind::S_C_TYPE_DECLARATION: // C_TYPE_DECLARATION
      case symbol_kind::S_C_FUNCTION_DECLARATION: // C_FUNCTION_DECLARATION
      case symbol_kind::S_FUNCTION_DEFINITION: // FUNCTION_DEFINITION
      case symbol_kind::S_METHOD_DEFINITION: // METHOD_DEFINITION
      case symbol_kind::S_STRUCTURE_DEFINITION: // STRUCTURE_DEFINITION
      case symbol_kind::S_UNION_DEFINITION: // UNION_DEFINITION
      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
      case symbol_kind::S_function_start: // function_start
      case symbol_kind::S_structure_start: // structure_start
      case symbol_kind::S_pattern_start: // pattern_start
        yylhs.value.emplace< std::string > ();
        break;

      case symbol_kind::S_repeat_start: // repeat_start
        yylhs.value.emplace< std::unique_ptr<RepetationInfo> > ();
        break;

      case symbol_kind::S_program: // program
      case symbol_kind::S_line: // line
      case symbol_kind::S_definition: // definition
      case symbol_kind::S_block: // block
      case symbol_kind::S_function_definition: // function_definition
      case symbol_kind::S_structure_definition: // structure_definition
      case symbol_kind::S_union_definition: // union_definition
      case symbol_kind::S_C_type_declaration: // C_type_declaration
      case symbol_kind::S_C_function_declaration: // C_function_declaration
      case symbol_kind::S_else_body: // else_body
      case symbol_kind::S_default_pattern: // default_pattern
      case symbol_kind::S_expression: // expression
      case symbol_kind::S_access: // access
      case symbol_kind::S_global_variable_definition: // global_variable_definition
      case symbol_kind::S_assign_variable: // assign_variable
      case symbol_kind::S_return_value: // return_value
      case symbol_kind::S_monomial: // monomial
      case symbol_kind::S_call: // call
      case symbol_kind::S_construction: // construction
      case symbol_kind::S_named_value: // named_value
        yylhs.value.emplace< std::unique_ptr<ast::AST> > ();
        break;

      case symbol_kind::S_UNION_CONSTRUCTOR: // UNION_CONSTRUCTOR
        yylhs.value.emplace< std::unique_ptr<ast::UnionConstructorInfo> > ();
        break;

      case symbol_kind::S_CONSTRUCTOR_DEFINITION: // CONSTRUCTOR_DEFINITION
      case symbol_kind::S_DESTRUCTOR_DEFINITION: // DESTRUCTOR_DEFINITION
      case symbol_kind::S_SETTER_DEFINITION: // SETTER_DEFINITION
      case symbol_kind::S_GETTER_DEFINITION: // GETTER_DEFINITION
      case symbol_kind::S_CLASS_DEFINITION: // CLASS_DEFINITION
        yylhs.value.emplace< std::unique_ptr<clawn::compiler::DeclarationInfo> > ();
        break;

      case symbol_kind::S_C_type_descriptor: // C_type_descriptor
        yylhs.value.emplace< std::vector<const std::string> > ();
        break;

      case symbol_kind::S_C_members_descriptions: // C_members_descriptions
        yylhs.value.emplace< std::vector<std::pair<const std::string,std::vector<const std::string>>> > ();
        break;

      case symbol_kind::S_parameters: // parameters
      case symbol_kind::S_elements: // elements
        yylhs.value.emplace< std::vector<std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>>> > ();
        break;

      case symbol_kind::S_patterns: // patterns
      case symbol_kind::S_named_arguments: // named_arguments
        yylhs.value.emplace< std::vector<std::pair<std::string,std::unique_ptr<ast::AST>>> > ();
        break;

      case symbol_kind::S_requirements: // requirements
        yylhs.value.emplace< std::vector<std::shared_ptr<requirement::Requirement>> > ();
        break;

      case symbol_kind::S_lines: // lines
      case symbol_kind::S_expressions: // expressions
        yylhs.value.emplace< std::vector<std::unique_ptr<ast::AST>> > ();
        break;

      case symbol_kind::S_C_arguments: // C_arguments
        yylhs.value.emplace< std::vector<std::vector<const std::string>> > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // program: lines
#line 199 "./src/lib/compiler/parser/parser.yy"
    {
        driver.ast_builder->break_out_of_scope();
        driver.ast_builder->create_root(std::move(yystack_[0].value.as < std::vector<std::unique_ptr<ast::AST>> > ()));
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_integer(0);
    }
#line 1425 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 5: // lines: line
#line 208 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::vector<std::unique_ptr<ast::AST>> > ().push_back(std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()));
    }
#line 1433 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 6: // lines: lines line
#line 212 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::vector<std::unique_ptr<ast::AST>> > () = std::move(yystack_[1].value.as < std::vector<std::unique_ptr<ast::AST>> > ());
        yylhs.value.as < std::vector<std::unique_ptr<ast::AST>> > ().push_back(std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()));
    }
#line 1442 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 7: // line: IMPORT STRING_LITERAL EOL_OR_EOF
#line 218 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_block({});
    }
#line 1450 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 8: // line: expression EOL_OR_EOF
#line 222 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = std::move(yystack_[1].value.as < std::unique_ptr<ast::AST> > ());
    }
#line 1458 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 9: // line: requirement_definition EOL_OR_EOF
#line 226 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_block({});
    }
#line 1466 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 10: // line: C_type_declaration
#line 230 "./src/lib/compiler/parser/parser.yy"
    {
        //Due to parsing EOL,C_type_declaration is in `line` rule.
        yylhs.value.as < std::unique_ptr<ast::AST> > () = std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ());
    }
#line 1475 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 11: // definition: function_definition
#line 236 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ());
    }
#line 1483 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 12: // definition: union_definition
#line 240 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ());
    }
#line 1491 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 13: // definition: structure_definition
#line 244 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ());
    }
#line 1499 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 14: // definition: C_function_declaration
#line 248 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ());
    }
#line 1507 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 15: // requirement_definition: REQUIREMENT IDENTIFIER EOL LEFT_CURLY_BRACE EOL requirements RIGHT_CURLY_BRACE
#line 253 "./src/lib/compiler/parser/parser.yy"
    {
        driver.requirement_builder->create_named_requirement_list(yystack_[5].value.as < std::string > (),std::move(yystack_[1].value.as < std::vector<std::shared_ptr<requirement::Requirement>> > ()));
    }
#line 1515 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 16: // requirements: requirement_clause EOL
#line 258 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::vector<std::shared_ptr<requirement::Requirement>> > ().push_back(yystack_[1].value.as < std::shared_ptr<requirement::Requirement> > ());
    }
#line 1523 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 17: // requirements: requirements requirement_clause EOL
#line 262 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::vector<std::shared_ptr<requirement::Requirement>> > () = std::move(yystack_[2].value.as < std::vector<std::shared_ptr<requirement::Requirement>> > ());
        yylhs.value.as < std::vector<std::shared_ptr<requirement::Requirement>> > ().push_back(yystack_[1].value.as < std::shared_ptr<requirement::Requirement> > ());
    }
#line 1532 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 18: // requirement_clause: IDENTIFIER
#line 268 "./src/lib/compiler/parser/parser.yy"
    {
        // reference_to / value
        yylhs.value.as < std::shared_ptr<requirement::Requirement> > () = driver.requirement_builder->create_requirement(yystack_[0].value.as < std::string > ());
    }
#line 1541 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 19: // requirement_clause: IDENTIFIER COLON IDENTIFIER
#line 273 "./src/lib/compiler/parser/parser.yy"
    {
        // reference_to: integer
        yylhs.value.as < std::shared_ptr<requirement::Requirement> > () = driver.requirement_builder->create_requirement(yystack_[2].value.as < std::string > (),yystack_[0].value.as < std::string > ());
    }
#line 1550 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 20: // requirement_clause: IDENTIFIER COLON IDENTIFIER COLON IDENTIFIER
#line 278 "./src/lib/compiler/parser/parser.yy"
    {
        // has member:integer
        yylhs.value.as < std::shared_ptr<requirement::Requirement> > () = driver.requirement_builder->create_requirement(yystack_[4].value.as < std::string > (),yystack_[2].value.as < std::string > (),yystack_[0].value.as < std::string > ());
    }
#line 1559 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 21: // block: LEFT_CURLY_BRACE lines RIGHT_CURLY_BRACE
#line 284 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_block(std::move(yystack_[1].value.as < std::vector<std::unique_ptr<ast::AST>> > ()));
    }
#line 1567 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 22: // block: LEFT_CURLY_BRACE EOL lines RIGHT_CURLY_BRACE
#line 288 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_block(std::move(yystack_[1].value.as < std::vector<std::unique_ptr<ast::AST>> > ()));
    }
#line 1575 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 23: // function_definition: function_start LEFT_PARENTHESIS parameters RIGHT_PARENTHESIS block
#line 293 "./src/lib/compiler/parser/parser.yy"
    {
        auto function_name = yystack_[4].value.as < std::string > ();
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_generic_function_definition(function_name,std::move(yystack_[2].value.as < std::vector<std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>>> > ()),std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()));
        driver.ast_builder->break_out_of_scope();
    }
#line 1585 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 24: // function_definition: function_start LEFT_PARENTHESIS parameters RIGHT_PARENTHESIS EOL block
#line 299 "./src/lib/compiler/parser/parser.yy"
    {
        auto function_name = yystack_[5].value.as < std::string > ();
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_generic_function_definition(function_name,std::move(yystack_[3].value.as < std::vector<std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>>> > ()),std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()));
        driver.ast_builder->break_out_of_scope();
    }
#line 1595 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 25: // function_definition: function_start LEFT_PARENTHESIS RIGHT_PARENTHESIS block
#line 305 "./src/lib/compiler/parser/parser.yy"
    {
        auto function_name = yystack_[3].value.as < std::string > ();
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_generic_function_definition(function_name,{},std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()));
        driver.ast_builder->break_out_of_scope();
    }
#line 1605 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 26: // function_definition: function_start LEFT_PARENTHESIS RIGHT_PARENTHESIS EOL block
#line 311 "./src/lib/compiler/parser/parser.yy"
    {
        auto function_name = yystack_[4].value.as < std::string > ();
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_generic_function_definition(function_name,{},std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()));
        driver.ast_builder->break_out_of_scope();
    }
#line 1615 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 27: // function_start: FUNCTION_DEFINITION
#line 318 "./src/lib/compiler/parser/parser.yy"
    {
        //auto name = $1->name;
        driver.ast_builder->into_named_scope(yystack_[0].value.as < std::string > ());
        
        yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > ();
    }
#line 1626 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 28: // parameters: parameter
#line 346 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::vector<std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>>> > ().push_back(yystack_[0].value.as < std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>> > ());
    }
#line 1634 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 29: // parameters: parameters COMMA parameter
#line 350 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::vector<std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>>> > () = std::move(yystack_[2].value.as < std::vector<std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>>> > ());
        yylhs.value.as < std::vector<std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>>> > ().push_back(yystack_[0].value.as < std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>> > ());
    }
#line 1643 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 30: // parameter: IDENTIFIER
#line 356 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>> > () = std::make_pair(driver.ast_builder->register_argument(yystack_[0].value.as < std::string > ()),std::optional<std::shared_ptr<requirement::Requirement>>(std::nullopt));
    }
#line 1651 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 31: // parameter: IDENTIFIER COLON requirement_clause
#line 360 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>> > () = std::make_pair(driver.ast_builder->register_argument(yystack_[2].value.as < std::string > ()),std::optional<std::shared_ptr<requirement::Requirement>>(yystack_[0].value.as < std::shared_ptr<requirement::Requirement> > ()));
    }
#line 1659 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 32: // structure_definition: structure_start EOL LEFT_CURLY_BRACE EOL elements RIGHT_CURLY_BRACE
#line 365 "./src/lib/compiler/parser/parser.yy"
    {
        //methds are not yet implemented
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_generic_structure_definition(yystack_[5].value.as < std::string > (),yystack_[1].value.as < std::vector<std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>>> > ());
        driver.ast_builder->break_out_of_scope();
    }
#line 1669 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 33: // structure_definition: structure_start EOL LEFT_CURLY_BRACE EOL RIGHT_CURLY_BRACE
#line 371 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_generic_structure_definition(yystack_[4].value.as < std::string > (),{});
        driver.ast_builder->break_out_of_scope();
    }
#line 1678 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 34: // structure_start: STRUCTURE_DEFINITION
#line 377 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > ();
        driver.ast_builder->into_named_scope(yystack_[0].value.as < std::string > ());
    }
#line 1687 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 35: // union_definition: UNION_DEFINITION EOL LEFT_CURLY_BRACE EOL elements RIGHT_CURLY_BRACE
#line 383 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_generic_union_definition(yystack_[5].value.as < std::string > (),yystack_[1].value.as < std::vector<std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>>> > ());
    }
#line 1695 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 36: // elements: element EOL
#line 407 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::vector<std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>>> > ().push_back(yystack_[1].value.as < std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>> > ());
    }
#line 1703 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 37: // elements: elements element EOL
#line 411 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::vector<std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>>> > () = std::move(yystack_[2].value.as < std::vector<std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>>> > ());
        yylhs.value.as < std::vector<std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>>> > ().push_back(yystack_[1].value.as < std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>> > ());
    }
#line 1712 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 38: // element: IDENTIFIER
#line 417 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>> > () = std::make_pair(yystack_[0].value.as < std::string > (),std::optional<std::shared_ptr<requirement::Requirement>>(std::nullopt));
    }
#line 1720 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 39: // element: IDENTIFIER COLON requirement_clause
#line 421 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>> > () = std::make_pair(yystack_[2].value.as < std::string > (),std::optional<std::shared_ptr<requirement::Requirement>>(yystack_[0].value.as < std::shared_ptr<requirement::Requirement> > ()));
    }
#line 1728 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 40: // C_type_declaration: C_TYPE_DECLARATION EOL_OR_EOF
#line 427 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_C_type_declaration(yystack_[1].value.as < std::string > (),{});
    }
#line 1736 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 41: // C_type_declaration: C_TYPE_DECLARATION EOL C_members_descriptions
#line 431 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_C_type_declaration(yystack_[2].value.as < std::string > (),yystack_[0].value.as < std::vector<std::pair<const std::string,std::vector<const std::string>>> > ());
    }
#line 1744 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 42: // C_function_declaration: C_FUNCTION_DECLARATION EOL C_FUNCTION_ARGUMENT C_arguments EOL C_FUNCTION_RETURN C_type_descriptor
#line 436 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_C_function_declaration(yystack_[6].value.as < std::string > (),yystack_[3].value.as < std::vector<std::vector<const std::string>> > (),yystack_[0].value.as < std::vector<const std::string> > ());
    }
#line 1752 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 43: // C_function_declaration: C_FUNCTION_DECLARATION EOL C_FUNCTION_ARGUMENT EOL C_FUNCTION_RETURN C_type_descriptor
#line 440 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_C_function_declaration(yystack_[5].value.as < std::string > (),{},yystack_[0].value.as < std::vector<const std::string> > ());
    }
#line 1760 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 44: // C_members_descriptions: MEMBER_IDENTIFIER COLON C_type_descriptor EOL_OR_EOF
#line 445 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::vector<std::pair<const std::string,std::vector<const std::string>>> > ().push_back(std::make_pair(yystack_[3].value.as < std::string > (),yystack_[1].value.as < std::vector<const std::string> > ()));
    }
#line 1768 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 45: // C_members_descriptions: C_members_descriptions MEMBER_IDENTIFIER COLON C_type_descriptor EOL_OR_EOF
#line 449 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::vector<std::pair<const std::string,std::vector<const std::string>>> > () = std::move(yystack_[4].value.as < std::vector<std::pair<const std::string,std::vector<const std::string>>> > ());
        yylhs.value.as < std::vector<std::pair<const std::string,std::vector<const std::string>>> > ().push_back(std::make_pair(yystack_[3].value.as < std::string > (),yystack_[1].value.as < std::vector<const std::string> > ()));
    }
#line 1777 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 46: // C_type_descriptor: IDENTIFIER
#line 455 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::vector<const std::string> > ().push_back(yystack_[0].value.as < std::string > ());
    }
#line 1785 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 47: // C_type_descriptor: C_type_descriptor IDENTIFIER
#line 459 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::vector<const std::string> > () = std::move(yystack_[1].value.as < std::vector<const std::string> > ());
        yylhs.value.as < std::vector<const std::string> > ().push_back(yystack_[0].value.as < std::string > ());
    }
#line 1794 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 48: // C_arguments: C_type_descriptor
#line 465 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::vector<std::vector<const std::string>> > ().push_back(yystack_[0].value.as < std::vector<const std::string> > ());
    }
#line 1802 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 49: // C_arguments: C_arguments COMMA C_type_descriptor
#line 469 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::vector<std::vector<const std::string>> > () = std::move(yystack_[2].value.as < std::vector<std::vector<const std::string>> > ());
        yylhs.value.as < std::vector<std::vector<const std::string>> > ().push_back(yystack_[0].value.as < std::vector<const std::string> > ());
    }
#line 1811 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 50: // expressions: expression
#line 476 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::vector<std::unique_ptr<ast::AST>> > ().push_back(std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()));
    }
#line 1819 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 51: // expressions: expressions COMMA expression
#line 480 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::vector<std::unique_ptr<ast::AST>> > () = std::move(yystack_[2].value.as < std::vector<std::unique_ptr<ast::AST>> > ());
        yylhs.value.as < std::vector<std::unique_ptr<ast::AST>> > ().push_back(std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()));
    }
#line 1828 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 52: // if_start: IF
#line 486 "./src/lib/compiler/parser/parser.yy"
    {
        driver.ast_builder->into_anonymous_scope();
    }
#line 1836 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 53: // else_start: ELSE
#line 491 "./src/lib/compiler/parser/parser.yy"
    {
        driver.ast_builder->break_out_of_scope();
        driver.ast_builder->into_anonymous_scope();
    }
#line 1845 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 54: // repeat_start: repeat_first_start COLON expression
#line 497 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<RepetationInfo> > () = std::make_unique<RepetationInfo>(std::move(yystack_[2].value.as < std::pair<std::string,std::unique_ptr<ast::AST>> > ().second),yystack_[2].value.as < std::pair<std::string,std::unique_ptr<ast::AST>> > ().first,std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()));
    }
#line 1853 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 55: // repeat_first_start: REPEAT IDENTIFIER
#line 503 "./src/lib/compiler/parser/parser.yy"
    {
        driver.ast_builder->into_anonymous_scope();
        auto counter = driver.ast_builder->create_assignment_or_variable_definition(
            yystack_[0].value.as < std::string > (),driver.ast_builder->create_integer(0)
        );
        yylhs.value.as < std::pair<std::string,std::unique_ptr<ast::AST>> > () = std::make_pair(yystack_[0].value.as < std::string > (),std::move(counter));
    }
#line 1865 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 56: // else_body: else_start EOL block
#line 514 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ());
        driver.ast_builder->break_out_of_scope();
    }
#line 1874 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 57: // pattern_start: IDENTIFIER
#line 520 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > ();
        driver.ast_builder->into_anonymous_scope();
    }
#line 1883 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 58: // default_pattern_start: DEFAULT
#line 526 "./src/lib/compiler/parser/parser.yy"
    {
        driver.ast_builder->into_anonymous_scope();
    }
#line 1891 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 59: // pattern: pattern_start ARM expression
#line 531 "./src/lib/compiler/parser/parser.yy"
    {
        std::vector<std::unique_ptr<ast::AST>> body;
        body.push_back(std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()));
        yylhs.value.as < std::pair<std::string,std::unique_ptr<ast::AST>> > () = std::make_pair(yystack_[2].value.as < std::string > (),driver.ast_builder->create_block(std::move(body)));
        driver.ast_builder->break_out_of_scope();      
    }
#line 1902 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 60: // pattern: pattern_start ARM block
#line 538 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::pair<std::string,std::unique_ptr<ast::AST>> > () = std::make_pair(yystack_[2].value.as < std::string > (),std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()));
        driver.ast_builder->break_out_of_scope();
    }
#line 1911 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 61: // default_pattern: default_pattern_start ARM expression
#line 544 "./src/lib/compiler/parser/parser.yy"
    {
        std::vector<std::unique_ptr<ast::AST>> body;
        body.push_back(std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()));
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_block(std::move(body));
        driver.ast_builder->break_out_of_scope();
    }
#line 1922 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 62: // default_pattern: default_pattern_start ARM block
#line 551 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ());
        driver.ast_builder->break_out_of_scope();
    }
#line 1931 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 63: // patterns: pattern
#line 557 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::vector<std::pair<std::string,std::unique_ptr<ast::AST>>> > ().push_back(std::move(yystack_[0].value.as < std::pair<std::string,std::unique_ptr<ast::AST>> > ()));
    }
#line 1939 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 64: // patterns: patterns EOL pattern
#line 561 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::vector<std::pair<std::string,std::unique_ptr<ast::AST>>> > () = std::move(yystack_[2].value.as < std::vector<std::pair<std::string,std::unique_ptr<ast::AST>>> > ());
        yylhs.value.as < std::vector<std::pair<std::string,std::unique_ptr<ast::AST>>> > ().push_back(std::move(yystack_[0].value.as < std::pair<std::string,std::unique_ptr<ast::AST>> > ()));
    }
#line 1948 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 66: // expression: definition
#line 568 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ());
    }
#line 1956 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 67: // expression: block
#line 572 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ());
    }
#line 1964 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 68: // expression: return_value
#line 576 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_return(std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()));
        //$$ = std::move($1);
    }
#line 1973 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 69: // expression: ARM expression
#line 581 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_set_result(std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()));
    }
#line 1981 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 70: // expression: if_start expression EOL block else_body
#line 585 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_if(std::move(yystack_[3].value.as < std::unique_ptr<ast::AST> > ()),std::move(yystack_[1].value.as < std::unique_ptr<ast::AST> > ()),std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()));
    }
#line 1989 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 71: // expression: if_start expression EOL block
#line 589 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_if(std::move(yystack_[2].value.as < std::unique_ptr<ast::AST> > ()),std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()), driver.ast_builder->create_block({}));
        driver.ast_builder->break_out_of_scope();
    }
#line 1998 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 72: // expression: repeat_start EOL block
#line 595 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_repeat(yystack_[2].value.as < std::unique_ptr<RepetationInfo> > ()->counter_name,std::move(yystack_[2].value.as < std::unique_ptr<RepetationInfo> > ()->counter),std::move(yystack_[2].value.as < std::unique_ptr<RepetationInfo> > ()->condition),std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()));
        driver.ast_builder->break_out_of_scope();
    }
#line 2007 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 73: // expression: match_start expression EOL LEFT_CURLY_BRACE EOL patterns EOL RIGHT_CURLY_BRACE
#line 600 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_match(std::move(yystack_[6].value.as < std::unique_ptr<ast::AST> > ()),std::move(yystack_[2].value.as < std::vector<std::pair<std::string,std::unique_ptr<ast::AST>>> > ()));
    }
#line 2015 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 74: // expression: match_start expression EOL LEFT_CURLY_BRACE EOL patterns EOL default_pattern EOL RIGHT_CURLY_BRACE
#line 604 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_match(std::move(yystack_[8].value.as < std::unique_ptr<ast::AST> > ()),std::move(yystack_[4].value.as < std::vector<std::pair<std::string,std::unique_ptr<ast::AST>>> > ()),std::move(yystack_[2].value.as < std::unique_ptr<ast::AST> > ()));
    }
#line 2023 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 75: // expression: assign_variable
#line 608 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ());
    }
#line 2031 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 76: // expression: global_variable_definition
#line 612 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ());
    }
#line 2039 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 77: // expression: expression LEFT_ARROW expression
#line 616 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_assignment(driver.ast_builder->create_dereference(std::move(yystack_[2].value.as < std::unique_ptr<ast::AST> > ())),std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()));
       //$$ = driver.ast_builder->create_deep_copy(std::move($1),std::move($3));
    }
#line 2048 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 78: // expression: expression BOTH_SIDE_ARROW expression
#line 621 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_assignment(std::move(yystack_[2].value.as < std::unique_ptr<ast::AST> > ()),std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()),true);
       //$$ = driver.ast_builder->create_deep_copy(std::move($1),std::move($3));
    }
#line 2057 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 79: // expression: expression PLUS expression
#line 626 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_binary_expression(std::move(yystack_[2].value.as < std::unique_ptr<ast::AST> > ()),std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()),ast::OperatorKind::Addition);
    }
#line 2065 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 80: // expression: expression MINUS expression
#line 630 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_binary_expression(std::move(yystack_[2].value.as < std::unique_ptr<ast::AST> > ()),std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()),ast::OperatorKind::Subtraction);
    }
#line 2073 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 81: // expression: expression ASTERISK expression
#line 634 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_binary_expression(std::move(yystack_[2].value.as < std::unique_ptr<ast::AST> > ()),std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()),ast::OperatorKind::Multiplication);
    }
#line 2081 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 82: // expression: expression SLASH expression
#line 638 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_binary_expression(std::move(yystack_[2].value.as < std::unique_ptr<ast::AST> > ()),std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()),ast::OperatorKind::Division);
    }
#line 2089 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 83: // expression: expression OP_AND expression
#line 642 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_binary_expression(std::move(yystack_[2].value.as < std::unique_ptr<ast::AST> > ()),std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()),ast::OperatorKind::And);
    }
#line 2097 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 84: // expression: expression OP_OR expression
#line 646 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_binary_expression(std::move(yystack_[2].value.as < std::unique_ptr<ast::AST> > ()),std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()),ast::OperatorKind::Or);
    }
#line 2105 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 85: // expression: expression OP_MORE_EQUAL expression
#line 650 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_binary_expression(std::move(yystack_[2].value.as < std::unique_ptr<ast::AST> > ()),std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()),ast::OperatorKind::GreaterThanOrEqual);
    }
#line 2113 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 86: // expression: expression OP_LESS_EQUAL expression
#line 654 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_binary_expression(std::move(yystack_[2].value.as < std::unique_ptr<ast::AST> > ()),std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()),ast::OperatorKind::LessThanOrEqual);
    }
#line 2121 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 87: // expression: expression OP_MORE expression
#line 658 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_binary_expression(std::move(yystack_[2].value.as < std::unique_ptr<ast::AST> > ()),std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()),ast::OperatorKind::GreaterThan);
    }
#line 2129 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 88: // expression: expression OP_LESS expression
#line 662 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_binary_expression(std::move(yystack_[2].value.as < std::unique_ptr<ast::AST> > ()),std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()),ast::OperatorKind::LessThan);
    }
#line 2137 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 89: // expression: expression OP_NOT_EQUAL expression
#line 666 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_binary_expression(std::move(yystack_[2].value.as < std::unique_ptr<ast::AST> > ()),std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()),ast::OperatorKind::NotEqual);
    }
#line 2145 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 90: // expression: expression OP_EQUAL expression
#line 670 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_binary_expression(std::move(yystack_[2].value.as < std::unique_ptr<ast::AST> > ()),std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()),ast::OperatorKind::Equal);
    }
#line 2153 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 91: // expression: expression LEFT_BRACKET expression RIGHT_BRACKET
#line 674 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_access_list(std::move(yystack_[3].value.as < std::unique_ptr<ast::AST> > ()),std::move(yystack_[1].value.as < std::unique_ptr<ast::AST> > ()));
    }
#line 2161 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 92: // expression: expression DOUBLE_DOT IDENTIFIER
#line 678 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_common_access(std::move(yystack_[2].value.as < std::unique_ptr<ast::AST> > ()),std::move(yystack_[0].value.as < std::string > ()));
    }
#line 2169 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 93: // expression: expression DOUBLE_DOT LEFT_PARENTHESIS expressions RIGHT_PARENTHESIS
#line 682 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_common_call(std::move(yystack_[4].value.as < std::unique_ptr<ast::AST> > ()),std::move(yystack_[1].value.as < std::vector<std::unique_ptr<ast::AST>> > ()));
    }
#line 2177 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 94: // expression: REFER expression
#line 687 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_reference(std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()));
    }
#line 2185 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 95: // expression: ACCESS expression
#line 691 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_dereference(std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()));
    }
#line 2193 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 96: // expression: VARIOUS expression
#line 695 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_various(std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()));
    }
#line 2201 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 97: // expression: monomial
#line 699 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ());
    }
#line 2209 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 98: // expression: UNION_CONSTRUCTOR
#line 703 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_union_constructor(*yystack_[0].value.as < std::unique_ptr<ast::UnionConstructorInfo> > ());
    }
#line 2217 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 99: // expression: access
#line 707 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ());
    }
#line 2225 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 100: // expression: MINUS expression
#line 711 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_minus(std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()));
    }
#line 2233 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 101: // expression: LEFT_PARENTHESIS expression RIGHT_PARENTHESIS
#line 715 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = std::move(yystack_[1].value.as < std::unique_ptr<ast::AST> > ());
    }
#line 2241 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 102: // access: expression DOT_IDENTIFIER
#line 721 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_access(std::move(yystack_[1].value.as < std::unique_ptr<ast::AST> > ()),yystack_[0].value.as < std::string > ());
    }
#line 2249 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 103: // access: expression RIGHT_ARROW_IDENTIFIER
#line 725 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_access(driver.ast_builder->create_dereference(std::move(yystack_[1].value.as < std::unique_ptr<ast::AST> > ())),yystack_[0].value.as < std::string > ());
    }
#line 2257 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 104: // global_variable_definition: GLOBAL IDENTIFIER EQUAL expression
#line 730 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_global_variable_definition(yystack_[2].value.as < std::string > (),std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()));
    }
#line 2265 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 105: // assign_variable: IDENTIFIER EQUAL expression
#line 735 "./src/lib/compiler/parser/parser.yy"
    {
        if (driver.ast_builder->exist_named_expression(yystack_[2].value.as < std::string > ()))
        {
            auto named_expression = driver.ast_builder->create_named_node(yystack_[2].value.as < std::string > ());
            yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_assignment(std::move(named_expression),std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()));
        }
        else
        {
            yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_assignment_or_variable_definition(yystack_[2].value.as < std::string > (),std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()));
        }
    }
#line 2281 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 106: // assign_variable: expression EQUAL expression
#line 747 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_assignment(std::move(yystack_[2].value.as < std::unique_ptr<ast::AST> > ()),std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ()));
    }
#line 2289 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 107: // return_value: RETURN expression
#line 752 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ());
    }
#line 2297 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 108: // monomial: call
#line 757 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ());
    }
#line 2305 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 109: // monomial: construction
#line 761 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ());
    }
#line 2313 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 110: // monomial: STRING_LITERAL
#line 765 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_string(yystack_[0].value.as < std::string > ());
    }
#line 2321 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 111: // monomial: FLOAT_LITERAL
#line 769 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_float(yystack_[0].value.as < double > ());
    }
#line 2329 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 112: // monomial: INT_LITERAL
#line 773 "./src/lib/compiler/parser/parser.yy"
    { 
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_integer(yystack_[0].value.as < long long > ());
    }
#line 2337 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 113: // monomial: LEFT_BRACKET RIGHT_BRACKET
#line 777 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_list();
    }
#line 2345 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 114: // monomial: LEFT_BRACKET expressions RIGHT_BRACKET
#line 781 "./src/lib/compiler/parser/parser.yy"
    {   
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_list(std::move(yystack_[1].value.as < std::vector<std::unique_ptr<ast::AST>> > ()));
    }
#line 2353 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 115: // monomial: named_value
#line 785 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ());
    }
#line 2361 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 116: // call: IDENTIFIER LEFT_PARENTHESIS expressions RIGHT_PARENTHESIS
#line 791 "./src/lib/compiler/parser/parser.yy"
    {
        if (driver.ast_builder->exist_named_expression(yystack_[3].value.as < std::string > ()))
        {
            auto function_object = driver.ast_builder->create_named_node(yystack_[3].value.as < std::string > ());
            yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_call(std::move(function_object),std::move(yystack_[1].value.as < std::vector<std::unique_ptr<ast::AST>> > ()));
        }
        else
        {
            yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_call(yystack_[3].value.as < std::string > (),std::move(yystack_[1].value.as < std::vector<std::unique_ptr<ast::AST>> > ()));
        }
    }
#line 2377 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 117: // call: IDENTIFIER LEFT_PARENTHESIS RIGHT_PARENTHESIS
#line 803 "./src/lib/compiler/parser/parser.yy"
    {
        if (driver.ast_builder->exist_named_expression(yystack_[2].value.as < std::string > ()))
        {
            auto function_object = driver.ast_builder->create_named_node(yystack_[2].value.as < std::string > ());
            yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_call(std::move(function_object),{});
        }
        else
        {
            yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_call(yystack_[2].value.as < std::string > (),{});
        }
    }
#line 2393 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 118: // call: expression LEFT_PARENTHESIS expressions RIGHT_PARENTHESIS
#line 815 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_call(std::move(yystack_[3].value.as < std::unique_ptr<ast::AST> > ()),std::move(yystack_[1].value.as < std::vector<std::unique_ptr<ast::AST>> > ()));
    }
#line 2401 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 119: // call: expression LEFT_PARENTHESIS RIGHT_PARENTHESIS
#line 819 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_call(std::move(yystack_[2].value.as < std::unique_ptr<ast::AST> > ()),{});
    }
#line 2409 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 120: // construction: expression LEFT_CURLY_BRACE named_arguments RIGHT_CURLY_BRACE
#line 824 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_construction(std::move(yystack_[3].value.as < std::unique_ptr<ast::AST> > ()),std::move(yystack_[1].value.as < std::vector<std::pair<std::string,std::unique_ptr<ast::AST>>> > ()));
    }
#line 2417 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 121: // construction: expression LEFT_CURLY_BRACE RIGHT_CURLY_BRACE
#line 828 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_construction(std::move(yystack_[2].value.as < std::unique_ptr<ast::AST> > ()),{});
    }
#line 2425 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 122: // named_arguments: IDENTIFIER COLON expression
#line 834 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::vector<std::pair<std::string,std::unique_ptr<ast::AST>>> > ().push_back(std::make_pair(yystack_[2].value.as < std::string > (),std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ())));
    }
#line 2433 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 123: // named_arguments: named_arguments COMMA IDENTIFIER COLON expression
#line 838 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::vector<std::pair<std::string,std::unique_ptr<ast::AST>>> > () = std::move(yystack_[4].value.as < std::vector<std::pair<std::string,std::unique_ptr<ast::AST>>> > ());
        yylhs.value.as < std::vector<std::pair<std::string,std::unique_ptr<ast::AST>>> > ().push_back(std::make_pair(yystack_[2].value.as < std::string > (),std::move(yystack_[0].value.as < std::unique_ptr<ast::AST> > ())));
    }
#line 2442 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;

  case 124: // named_value: IDENTIFIER
#line 845 "./src/lib/compiler/parser/parser.yy"
    {
        yylhs.value.as < std::unique_ptr<ast::AST> > () = driver.ast_builder->create_named_node(yystack_[0].value.as < std::string > ());
    }
#line 2450 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"
    break;


#line 2454 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        std::string msg = YY_("syntax error");
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

#if YYDEBUG || 0
  const char *
  Parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytname_[yysymbol];
  }
#endif // #if YYDEBUG || 0









  const short Parser::yypact_ninf_ = -159;

  const signed char Parser::yytable_ninf_ = -63;

  const short
  Parser::yypact_[] =
  {
     387,    69,   621,   621,   621,   621,   621,   453,   188,   621,
    -159,    10,   -45,  -159,  -159,   -43,    24,  -159,    27,  -159,
     621,    29,   -33,  -159,  -159,  -159,    36,   387,  -159,  -159,
      12,  -159,  -159,    32,  -159,    -3,  -159,  -159,  -159,   621,
       0,    13,   621,   124,  -159,  -159,  -159,  -159,  -159,  -159,
    -159,  -159,   621,   519,   814,   839,   839,   839,   152,  -159,
      -1,   814,   387,   255,   752,  -159,    71,  -159,    68,    48,
      42,  -159,   814,   107,    12,  -159,  -159,  -159,  -159,    11,
      86,   685,    87,   621,   716,   621,   621,   621,   621,   621,
     621,   621,   621,   621,   621,   621,   621,   621,   621,   621,
     621,    20,   585,    26,  -159,  -159,  -159,   814,  -159,    73,
    -159,   621,   321,  -159,  -159,    60,   114,     7,    49,    92,
     621,  -159,    65,    -6,    74,  -159,    53,    87,  -159,   814,
      97,   814,   814,   814,   862,   862,   141,   141,   141,   141,
     141,   141,   463,   463,   152,   152,   783,    70,  -159,     3,
    -159,    79,  -159,   621,  -159,   814,  -159,   129,    93,  -159,
     115,   144,    52,   151,    95,   814,   162,    87,  -159,    -5,
     166,    31,   118,   102,  -159,   621,  -159,   175,  -159,    80,
       9,   129,   129,  -159,   129,   142,   130,    37,   119,   162,
     134,  -159,  -159,    87,  -159,  -159,  -159,    43,  -159,   121,
    -159,   191,   814,   140,  -159,  -159,     9,   144,   144,   129,
     162,  -159,   131,  -159,    44,   132,   196,  -159,  -159,    87,
    -159,   143,  -159,   133,   621,  -159,   144,  -159,  -159,  -159,
     135,  -159,   150,  -159,   621,    16,   814,  -159,   203,   138,
     814,  -159,  -159,   149,  -159,   145,  -159,   621,   182,   147,
     814,  -159
  };

  const signed char
  Parser::yydefact_[] =
  {
       0,   124,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    27,    34,     0,     0,    52,     0,    65,
       0,     0,     0,   110,   112,   111,     0,     2,     5,    66,
       0,    67,    11,     0,    13,     0,    12,    10,    14,     0,
       0,     0,     0,     0,    99,    76,    75,    68,    97,   108,
     109,   115,     0,     0,   107,    96,    94,    95,   100,   113,
       0,    50,     0,     0,     0,     4,     3,    40,     0,     0,
       0,    55,    69,     0,     0,     1,     6,     3,     9,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   102,   103,     8,   105,   117,     0,
     114,     0,     0,    21,   101,     0,    41,     0,     0,     0,
       0,     7,    30,     0,     0,    28,     0,     0,    72,    54,
       0,   106,    77,    78,    83,    84,    90,    89,    85,    86,
      87,    88,    79,    80,    81,    82,     0,     0,   121,     0,
     119,     0,    92,     0,   116,    51,    22,     0,     0,    46,
       0,    48,     0,     0,     0,   104,     0,     0,    25,     0,
       0,     0,    71,     0,    91,     0,   120,     0,   118,     0,
       0,     0,     0,    47,     0,     0,    38,     0,     0,     0,
      18,    31,    26,     0,    23,    29,    33,     0,    53,     0,
      70,     0,   122,     0,    93,    44,     0,    43,    49,     0,
       0,    35,     0,    36,     0,     0,     0,    24,    32,     0,
      57,     0,    63,     0,     0,    45,    42,    39,    37,    15,
       0,    16,    19,    56,     0,     0,   123,    17,     0,    67,
      59,    73,    58,     0,    64,     0,    20,     0,     0,    67,
      61,    74
  };

  const short
  Parser::yypgoto_[] =
  {
    -159,  -159,   -22,     6,   -10,  -159,  -159,  -159,  -151,   -66,
    -159,  -159,  -159,    50,  -159,  -159,  -159,    41,  -158,  -159,
    -159,  -159,  -138,  -159,   -47,  -159,  -159,  -159,  -159,  -159,
    -159,  -159,   -16,  -159,  -159,  -159,    -2,  -159,  -159,  -159,
    -159,  -159,  -159,  -159,  -159,  -159
  };

  const unsigned char
  Parser::yydefgoto_[] =
  {
       0,    26,    67,    27,    28,    29,    30,   214,   191,    31,
      32,    33,   124,   125,    34,    35,    36,   187,   188,    37,
      38,   116,   161,   162,    60,    39,   199,    40,    41,   200,
     221,   243,   222,   245,   223,    42,    43,    44,    45,    46,
      47,    48,    49,    50,   149,    51
  };

  const short
  Parser::yytable_[] =
  {
      54,    55,    56,    57,    58,    61,   109,    64,    78,    65,
      65,   159,    65,   183,    63,   122,   128,    76,    72,   180,
     220,   106,     8,     8,   147,    68,   110,    69,    70,   212,
     152,    71,   176,    73,    74,   186,    75,    81,   215,   212,
      84,   186,   123,   206,   207,   241,   208,   186,   190,   148,
     107,    61,   121,    76,   111,   151,   153,   168,   177,   227,
     196,   172,    79,   230,   167,   193,   211,    80,   112,    83,
      82,   226,   218,   229,   115,    52,   118,   160,   242,    77,
      66,   129,    77,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,    53,
      61,   192,    76,   194,   154,   169,   179,   184,   117,   155,
     178,   204,   119,   120,   126,     8,   157,   158,   165,   163,
     164,   166,   185,   171,    65,   173,   175,   217,   111,   170,
      85,    86,    87,   159,   111,   111,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   183,   181,
     100,    61,   101,   233,   102,   186,   182,   103,   205,   104,
     105,    96,    97,    98,    99,   189,   190,   100,   239,   101,
     122,   102,   201,   202,   103,   198,   104,   105,   100,   203,
     101,   249,   102,   209,   225,   103,   210,   104,   105,   213,
     216,   219,     1,     2,    77,   220,   224,     3,     4,     5,
     232,   228,   231,   235,   234,   237,   238,   246,   -60,     6,
     247,   251,   197,     0,     7,   248,     8,   -62,     9,   244,
     195,     0,   236,     0,     0,    10,    11,    12,     0,     0,
      13,     0,   240,     0,     0,     0,     0,    14,    15,    16,
       0,     0,     0,     0,     0,   250,    17,    18,    19,    20,
       0,     0,     0,    21,    22,    23,    24,    25,    62,     1,
       2,     0,     0,     0,     3,     4,     5,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     6,     0,     0,     0,
       0,     7,     0,     8,   113,     9,     0,     0,     0,     0,
       0,     0,    10,    11,    12,     0,     0,    13,     0,     0,
       0,     0,     0,     0,    14,    15,    16,     0,     0,     0,
       0,     0,     0,    17,    18,    19,    20,     0,     0,     0,
      21,    22,    23,    24,    25,     1,     2,     0,     0,     0,
       3,     4,     5,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     6,     0,     0,     0,     0,     7,     0,     8,
     156,     9,     0,     0,     0,     0,     0,     0,    10,    11,
      12,     0,     0,    13,     0,     0,     0,     0,     0,     0,
      14,    15,    16,     0,     0,     0,     0,     0,     0,    17,
      18,    19,    20,     0,     0,     0,    21,    22,    23,    24,
      25,     1,     2,     0,     0,     0,     3,     4,     5,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     6,     0,
       0,     0,     0,     7,     0,     8,     0,     9,     0,     0,
       0,     0,     0,     0,    10,    11,    12,     0,     0,    13,
       0,     0,     0,     0,     0,     0,    14,    15,    16,     0,
       0,     0,     0,     0,     0,    17,    18,    19,    20,     0,
       0,     0,    21,    22,    23,    24,    25,     1,     2,     0,
       0,     0,     3,     4,     5,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     6,     0,     0,     0,     0,     7,
      59,     8,     0,     9,     0,    98,    99,     0,     0,   100,
      10,   101,    12,   102,     0,    13,   103,     0,   104,   105,
       0,     0,    14,    15,     0,     0,     0,     0,     0,     0,
       0,    17,    18,    19,    20,     0,     0,     0,    21,     0,
      23,    24,    25,     1,     2,     0,     0,     0,     3,     4,
       5,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       6,     0,     0,     0,     0,     7,     0,     8,     0,     9,
     108,     0,     0,     0,     0,     0,    10,     0,    12,     0,
       0,    13,     0,     0,     0,     0,     0,     0,    14,    15,
       0,     0,     0,     0,     0,     0,     0,    17,    18,    19,
      20,     0,     0,     0,    21,     0,    23,    24,    25,     1,
       2,     0,     0,     0,     3,     4,     5,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     6,     0,     0,     0,
       0,     7,     0,     8,     0,     9,   150,     0,     0,     0,
       0,     0,    10,     0,    12,     1,     2,    13,     0,     0,
       3,     4,     5,     0,    14,    15,     0,     0,     0,     0,
       0,     0,     6,    17,    18,    19,    20,     7,     0,     8,
      21,     9,    23,    24,    25,     0,     0,     0,    10,     0,
      12,     0,     0,    13,     0,     0,     0,     0,     0,     0,
      14,    15,     0,     0,     0,     0,     0,     0,     0,    17,
      18,    19,    20,     0,     0,     0,    21,     0,    23,    24,
      25,    85,    86,    87,     0,     0,     0,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,     0,
       0,   100,     0,   101,     0,   102,     0,     0,   103,     0,
     104,   105,    85,    86,    87,     0,     0,     0,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
       0,     0,   100,     0,   101,     0,   102,     0,     0,   103,
       0,   104,   105,     0,     0,   127,     0,     0,    85,    86,
      87,     0,     0,     0,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,     0,     0,   100,     0,
     101,     0,   102,   114,     0,   103,   130,   104,   105,    85,
      86,    87,     0,     0,     0,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,     0,     0,   100,
     174,   101,     0,   102,     0,     0,   103,     0,   104,   105,
      85,    86,    87,     0,     0,     0,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,     0,     0,
     100,     0,   101,     0,   102,     0,     0,   103,     0,   104,
     105,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,     0,     0,   100,     0,   101,     0,   102,
       0,     0,   103,     0,   104,   105,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,     0,     0,   100,     0,
     101,     0,   102,     0,     0,   103,     0,   104,   105
  };

  const short
  Parser::yycheck_[] =
  {
       2,     3,     4,     5,     6,     7,    53,     9,    30,     0,
       0,     4,     0,     4,     8,     4,    82,    27,    20,   157,
       4,    43,    28,    28,     4,    70,    27,    70,     4,   187,
       4,     4,    29,     4,    67,     4,     0,    39,   189,   197,
      42,     4,    31,   181,   182,    29,   184,     4,     4,    29,
      52,    53,    74,    63,    55,   102,    30,   123,    55,   210,
      29,   127,    30,   214,    70,    70,    29,    70,    62,    56,
      70,   209,    29,    29,     3,     6,    28,    70,    62,    70,
      70,    83,    70,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,    30,
     102,   167,   112,   169,    31,    31,   153,    55,    40,   111,
      31,    31,    70,     6,    28,    28,    56,     3,   120,    70,
      28,    56,    70,    70,     0,    28,    56,   193,    55,    55,
       6,     7,     8,     4,    55,    55,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,     4,    56,
      26,   153,    28,   219,    30,     4,    41,    33,   180,    35,
      36,    20,    21,    22,    23,    70,     4,    26,   234,    28,
       4,    30,    70,   175,    33,    57,    35,    36,    26,     4,
      28,   247,    30,    41,   206,    33,    56,    35,    36,    70,
      56,    70,     4,     5,    70,     4,    56,     9,    10,    11,
       4,    70,    70,    70,    61,    70,    56,     4,    70,    21,
      61,    29,   171,    -1,    26,    70,    28,    70,    30,   235,
     170,    -1,   224,    -1,    -1,    37,    38,    39,    -1,    -1,
      42,    -1,   234,    -1,    -1,    -1,    -1,    49,    50,    51,
      -1,    -1,    -1,    -1,    -1,   247,    58,    59,    60,    61,
      -1,    -1,    -1,    65,    66,    67,    68,    69,    70,     4,
       5,    -1,    -1,    -1,     9,    10,    11,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    26,    -1,    28,    29,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    37,    38,    39,    -1,    -1,    42,    -1,    -1,
      -1,    -1,    -1,    -1,    49,    50,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    58,    59,    60,    61,    -1,    -1,    -1,
      65,    66,    67,    68,    69,     4,     5,    -1,    -1,    -1,
       9,    10,    11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    26,    -1,    28,
      29,    30,    -1,    -1,    -1,    -1,    -1,    -1,    37,    38,
      39,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,
      49,    50,    51,    -1,    -1,    -1,    -1,    -1,    -1,    58,
      59,    60,    61,    -1,    -1,    -1,    65,    66,    67,    68,
      69,     4,     5,    -1,    -1,    -1,     9,    10,    11,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    26,    -1,    28,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    37,    38,    39,    -1,    -1,    42,
      -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    58,    59,    60,    61,    -1,
      -1,    -1,    65,    66,    67,    68,    69,     4,     5,    -1,
      -1,    -1,     9,    10,    11,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    26,
      27,    28,    -1,    30,    -1,    22,    23,    -1,    -1,    26,
      37,    28,    39,    30,    -1,    42,    33,    -1,    35,    36,
      -1,    -1,    49,    50,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    58,    59,    60,    61,    -1,    -1,    -1,    65,    -1,
      67,    68,    69,     4,     5,    -1,    -1,    -1,     9,    10,
      11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    26,    -1,    28,    -1,    30,
      31,    -1,    -1,    -1,    -1,    -1,    37,    -1,    39,    -1,
      -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,    59,    60,
      61,    -1,    -1,    -1,    65,    -1,    67,    68,    69,     4,
       5,    -1,    -1,    -1,     9,    10,    11,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    26,    -1,    28,    -1,    30,    31,    -1,    -1,    -1,
      -1,    -1,    37,    -1,    39,     4,     5,    42,    -1,    -1,
       9,    10,    11,    -1,    49,    50,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    58,    59,    60,    61,    26,    -1,    28,
      65,    30,    67,    68,    69,    -1,    -1,    -1,    37,    -1,
      39,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,
      49,    50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,
      59,    60,    61,    -1,    -1,    -1,    65,    -1,    67,    68,
      69,     6,     7,     8,    -1,    -1,    -1,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    -1,
      -1,    26,    -1,    28,    -1,    30,    -1,    -1,    33,    -1,
      35,    36,     6,     7,     8,    -1,    -1,    -1,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      -1,    -1,    26,    -1,    28,    -1,    30,    -1,    -1,    33,
      -1,    35,    36,    -1,    -1,    70,    -1,    -1,     6,     7,
       8,    -1,    -1,    -1,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    -1,    -1,    26,    -1,
      28,    -1,    30,    31,    -1,    33,    70,    35,    36,     6,
       7,     8,    -1,    -1,    -1,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    -1,    -1,    26,
      27,    28,    -1,    30,    -1,    -1,    33,    -1,    35,    36,
       6,     7,     8,    -1,    -1,    -1,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    -1,    -1,
      26,    -1,    28,    -1,    30,    -1,    -1,    33,    -1,    35,
      36,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    -1,    -1,    26,    -1,    28,    -1,    30,
      -1,    -1,    33,    -1,    35,    36,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    -1,    -1,    26,    -1,
      28,    -1,    30,    -1,    -1,    33,    -1,    35,    36
  };

  const signed char
  Parser::yystos_[] =
  {
       0,     4,     5,     9,    10,    11,    21,    26,    28,    30,
      37,    38,    39,    42,    49,    50,    51,    58,    59,    60,
      61,    65,    66,    67,    68,    69,    72,    74,    75,    76,
      77,    80,    81,    82,    85,    86,    87,    90,    91,    96,
      98,    99,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   116,     6,    30,   107,   107,   107,   107,   107,    27,
      95,   107,    70,    74,   107,     0,    70,    73,    70,    70,
       4,     4,   107,     4,    67,     0,    75,    70,    73,    30,
      70,   107,    70,    56,   107,     6,     7,     8,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      26,    28,    30,    33,    35,    36,    73,   107,    31,    95,
      27,    55,    74,    29,    31,     3,    92,    40,    28,    70,
       6,    73,     4,    31,    83,    84,    28,    70,    80,   107,
      70,   107,   107,   107,   107,   107,   107,   107,   107,   107,
     107,   107,   107,   107,   107,   107,   107,     4,    29,   115,
      31,    95,     4,    30,    31,   107,    29,    56,     3,     4,
      70,    93,    94,    70,    28,   107,    56,    70,    80,    31,
      55,    70,    80,    28,    27,    56,    29,    55,    31,    95,
      93,    56,    41,     4,    55,    70,     4,    88,    89,    70,
       4,    79,    80,    70,    80,    84,    29,    88,    57,    97,
     100,    70,   107,     4,    31,    73,    93,    93,    93,    41,
      56,    29,    89,    70,    78,    79,    56,    80,    29,    70,
       4,   101,   103,   105,    56,    73,    93,    79,    70,    29,
      79,    70,     4,    80,    61,    70,   107,    70,    56,    80,
     107,    29,    62,   102,   103,   104,     4,    61,    70,    80,
     107,    29
  };

  const signed char
  Parser::yyr1_[] =
  {
       0,    71,    72,    73,    73,    74,    74,    75,    75,    75,
      75,    76,    76,    76,    76,    77,    78,    78,    79,    79,
      79,    80,    80,    81,    81,    81,    81,    82,    83,    83,
      84,    84,    85,    85,    86,    87,    88,    88,    89,    89,
      90,    90,    91,    91,    92,    92,    93,    93,    94,    94,
      95,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     103,   104,   104,   105,   105,   106,   107,   107,   107,   107,
     107,   107,   107,   107,   107,   107,   107,   107,   107,   107,
     107,   107,   107,   107,   107,   107,   107,   107,   107,   107,
     107,   107,   107,   107,   107,   107,   107,   107,   107,   107,
     107,   107,   108,   108,   109,   110,   110,   111,   112,   112,
     112,   112,   112,   112,   112,   112,   113,   113,   113,   113,
     114,   114,   115,   115,   116
  };

  const signed char
  Parser::yyr2_[] =
  {
       0,     2,     1,     1,     1,     1,     2,     3,     2,     2,
       1,     1,     1,     1,     1,     7,     2,     3,     1,     3,
       5,     3,     4,     5,     6,     4,     5,     1,     1,     3,
       1,     3,     6,     5,     1,     6,     2,     3,     1,     3,
       2,     3,     7,     6,     4,     5,     1,     2,     1,     3,
       1,     3,     1,     1,     3,     2,     3,     1,     1,     3,
       3,     3,     3,     1,     3,     1,     1,     1,     1,     2,
       5,     4,     3,     8,    10,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     4,     3,     5,     2,     2,     2,     1,     1,     1,
       2,     3,     2,     2,     4,     3,     3,     2,     1,     1,
       1,     1,     1,     2,     3,     1,     4,     3,     4,     3,
       4,     3,     3,     5,     1
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const Parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "MEMBER_IDENTIFIER",
  "IDENTIFIER", "RETURN", "EQUAL", "LEFT_ARROW", "BOTH_SIDE_ARROW",
  "VARIOUS", "REFER", "ACCESS", "OP_AND", "OP_OR", "OP_EQUAL",
  "OP_NOT_EQUAL", "OP_MORE_EQUAL", "OP_LESS_EQUAL", "OP_MORE", "OP_LESS",
  "PLUS", "MINUS", "ASTERISK", "SLASH", "UMINUS", "SEMICOLON",
  "LEFT_BRACKET", "RIGHT_BRACKET", "LEFT_CURLY_BRACE", "RIGHT_CURLY_BRACE",
  "LEFT_PARENTHESIS", "RIGHT_PARENTHESIS", "LEFT_PARENTHESIS_AND_EOL",
  "DOUBLE_DOT", "TRIPLE_DOT", "DOT_IDENTIFIER", "RIGHT_ARROW_IDENTIFIER",
  "UNION_CONSTRUCTOR", "C_TYPE_DECLARATION", "C_FUNCTION_DECLARATION",
  "C_FUNCTION_ARGUMENT", "C_FUNCTION_RETURN", "FUNCTION_DEFINITION",
  "METHOD_DEFINITION", "CONSTRUCTOR_DEFINITION", "DESTRUCTOR_DEFINITION",
  "SETTER_DEFINITION", "GETTER_DEFINITION", "CLASS_DEFINITION",
  "STRUCTURE_DEFINITION", "UNION_DEFINITION", "REQUIREMENT",
  "SINGLE_QUOTATION", "USE", "LAZY", "COMMA", "COLON", "ELSE", "IF",
  "REPEAT", "MATCH", "ARM", "DEFAULT", "IN", "WHILE", "GLOBAL", "IMPORT",
  "STRING_LITERAL", "INT_LITERAL", "FLOAT_LITERAL", "EOL", "$accept",
  "program", "EOL_OR_EOF", "lines", "line", "definition",
  "requirement_definition", "requirements", "requirement_clause", "block",
  "function_definition", "function_start", "parameters", "parameter",
  "structure_definition", "structure_start", "union_definition",
  "elements", "element", "C_type_declaration", "C_function_declaration",
  "C_members_descriptions", "C_type_descriptor", "C_arguments",
  "expressions", "if_start", "else_start", "repeat_start",
  "repeat_first_start", "else_body", "pattern_start",
  "default_pattern_start", "pattern", "default_pattern", "patterns",
  "match_start", "expression", "access", "global_variable_definition",
  "assign_variable", "return_value", "monomial", "call", "construction",
  "named_arguments", "named_value", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  Parser::yyrline_[] =
  {
       0,   198,   198,   204,   204,   207,   211,   217,   221,   225,
     229,   235,   239,   243,   247,   252,   257,   261,   267,   272,
     277,   283,   287,   292,   298,   304,   310,   317,   345,   349,
     355,   359,   364,   370,   376,   382,   406,   410,   416,   420,
     426,   430,   435,   439,   444,   448,   454,   458,   464,   468,
     475,   479,   485,   490,   496,   502,   513,   519,   525,   530,
     537,   543,   550,   556,   560,   565,   567,   571,   575,   580,
     584,   588,   594,   599,   603,   607,   611,   615,   620,   625,
     629,   633,   637,   641,   645,   649,   653,   657,   661,   665,
     669,   673,   677,   681,   686,   690,   694,   698,   702,   706,
     710,   714,   720,   724,   729,   734,   746,   751,   756,   760,
     764,   768,   772,   776,   780,   784,   790,   802,   814,   818,
     823,   827,   833,   837,   844
  };

  void
  Parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  Parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  Parser::symbol_kind_type
  Parser::yytranslate_ (int t) YY_NOEXCEPT
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70
    };
    // Last valid token kind.
    const int code_max = 325;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

#line 5 "./src/lib/compiler/parser/parser.yy"
} } // clawn::compiler
#line 3131 "/Users/ueharanaoto/Desktop/Clawn/build/parser.cpp"

#line 848 "./src/lib/compiler/parser/parser.yy"


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
