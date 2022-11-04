// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton interface for Bison LALR(1) parsers in C++

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


/**
 ** \file /Users/ueharanaoto/Desktop/Clawn/build/parser.hpp
 ** Define the clawn::compiler::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.

#ifndef YY_YY_USERS_UEHARANAOTO_DESKTOP_CLAWN_BUILD_PARSER_HPP_INCLUDED
# define YY_YY_USERS_UEHARANAOTO_DESKTOP_CLAWN_BUILD_PARSER_HPP_INCLUDED
// "%code requires" blocks.
#line 14 "./src/lib/compiler/parser/parser.yy"

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

#line 87 "/Users/ueharanaoto/Desktop/Clawn/build/parser.hpp"

# include <cassert>
# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>

#if defined __cplusplus
# define YY_CPLUSPLUS __cplusplus
#else
# define YY_CPLUSPLUS 199711L
#endif

// Support move semantics when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_MOVE           std::move
# define YY_MOVE_OR_COPY   move
# define YY_MOVE_REF(Type) Type&&
# define YY_RVREF(Type)    Type&&
# define YY_COPY(Type)     Type
#else
# define YY_MOVE
# define YY_MOVE_OR_COPY   copy
# define YY_MOVE_REF(Type) Type&
# define YY_RVREF(Type)    const Type&
# define YY_COPY(Type)     const Type&
#endif

// Support noexcept when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_NOEXCEPT noexcept
# define YY_NOTHROW
#else
# define YY_NOEXCEPT
# define YY_NOTHROW throw ()
#endif

// Support constexpr when possible.
#if 201703 <= YY_CPLUSPLUS
# define YY_CONSTEXPR constexpr
#else
# define YY_CONSTEXPR
#endif
# include "location.hh"
#include <typeinfo>
#ifndef YY_ASSERT
# include <cassert>
# define YY_ASSERT assert
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

#line 5 "./src/lib/compiler/parser/parser.yy"
namespace clawn { namespace compiler {
#line 228 "/Users/ueharanaoto/Desktop/Clawn/build/parser.hpp"




  /// A Bison parser.
  class Parser
  {
  public:
#ifdef YYSTYPE
# ifdef __GNUC__
#  pragma GCC message "bison: do not #define YYSTYPE in C++, use %define api.value.type"
# endif
    typedef YYSTYPE value_type;
#else
  /// A buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current parser state.
  class value_type
  {
  public:
    /// Type of *this.
    typedef value_type self_type;

    /// Empty construction.
    value_type () YY_NOEXCEPT
      : yyraw_ ()
      , yytypeid_ (YY_NULLPTR)
    {}

    /// Construct and fill.
    template <typename T>
    value_type (YY_RVREF (T) t)
      : yytypeid_ (&typeid (T))
    {
      YY_ASSERT (sizeof (T) <= size);
      new (yyas_<T> ()) T (YY_MOVE (t));
    }

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    value_type (const self_type&) = delete;
    /// Non copyable.
    self_type& operator= (const self_type&) = delete;
#endif

    /// Destruction, allowed only if empty.
    ~value_type () YY_NOEXCEPT
    {
      YY_ASSERT (!yytypeid_);
    }

# if 201103L <= YY_CPLUSPLUS
    /// Instantiate a \a T in here from \a t.
    template <typename T, typename... U>
    T&
    emplace (U&&... u)
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (std::forward <U>(u)...);
    }
# else
    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    emplace ()
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T ();
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    emplace (const T& t)
    {
      YY_ASSERT (!yytypeid_);
      YY_ASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (std::move((T&)t));
    }
# endif

    /// Instantiate an empty \a T in here.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build ()
    {
      return emplace<T> ();
    }

    /// Instantiate a \a T in here from \a t.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build (const T& t)
    {
      return emplace<T> (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as () YY_NOEXCEPT
    {
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == typeid (T));
      YY_ASSERT (sizeof (T) <= size);
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const YY_NOEXCEPT
    {
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == typeid (T));
      YY_ASSERT (sizeof (T) <= size);
      return *yyas_<T> ();
    }

    /// Swap the content with \a that, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsibility.
    /// Swapping between built and (possibly) non-built is done with
    /// self_type::move ().
    template <typename T>
    void
    swap (self_type& that) YY_NOEXCEPT
    {
      YY_ASSERT (yytypeid_);
      YY_ASSERT (*yytypeid_ == *that.yytypeid_);
      std::swap (as<T> (), that.as<T> ());
    }

    /// Move the content of \a that to this.
    ///
    /// Destroys \a that.
    template <typename T>
    void
    move (self_type& that)
    {
# if 201103L <= YY_CPLUSPLUS
      emplace<T> (std::move (that.as<T> ()));
# else
      emplace<T> ();
      swap<T> (that);
# endif
      that.destroy<T> ();
    }

# if 201103L <= YY_CPLUSPLUS
    /// Move the content of \a that to this.
    template <typename T>
    void
    move (self_type&& that)
    {
      emplace<T> (std::move (that.as<T> ()));
      that.destroy<T> ();
    }
#endif

    /// Copy the content of \a that to this.
    template <typename T>
    void
    copy (const self_type& that)
    {
      emplace<T> (that.as<T> ());
    }

    /// Destroy the stored \a T.
    template <typename T>
    void
    destroy ()
    {
      as<T> ().~T ();
      yytypeid_ = YY_NULLPTR;
    }

  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    value_type (const self_type&);
    /// Non copyable.
    self_type& operator= (const self_type&);
#endif

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ () YY_NOEXCEPT
    {
      void *yyp = yyraw_;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const YY_NOEXCEPT
    {
      const void *yyp = yyraw_;
      return static_cast<const T*> (yyp);
     }

    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // FLOAT_LITERAL
      char dummy1[sizeof (double)];

      // INT_LITERAL
      char dummy2[sizeof (long long)];

      // parameter
      // element
      char dummy3[sizeof (std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>>)];

      // repeat_first_start
      // pattern
      char dummy4[sizeof (std::pair<std::string,std::unique_ptr<ast::AST>>)];

      // requirement_clause
      char dummy5[sizeof (std::shared_ptr<requirement::Requirement>)];

      // MEMBER_IDENTIFIER
      // IDENTIFIER
      // DOT_IDENTIFIER
      // RIGHT_ARROW_IDENTIFIER
      // C_TYPE_DECLARATION
      // C_FUNCTION_DECLARATION
      // FUNCTION_DEFINITION
      // METHOD_DEFINITION
      // STRUCTURE_DEFINITION
      // UNION_DEFINITION
      // STRING_LITERAL
      // function_start
      // structure_start
      // pattern_start
      char dummy6[sizeof (std::string)];

      // repeat_start
      char dummy7[sizeof (std::unique_ptr<RepetationInfo>)];

      // program
      // line
      // definition
      // block
      // function_definition
      // structure_definition
      // union_definition
      // C_type_declaration
      // C_function_declaration
      // else_body
      // default_pattern
      // expression
      // access
      // global_variable_definition
      // assign_variable
      // return_value
      // monomial
      // call
      // construction
      // named_value
      char dummy8[sizeof (std::unique_ptr<ast::AST>)];

      // UNION_CONSTRUCTOR
      char dummy9[sizeof (std::unique_ptr<ast::UnionConstructorInfo>)];

      // CONSTRUCTOR_DEFINITION
      // DESTRUCTOR_DEFINITION
      // SETTER_DEFINITION
      // GETTER_DEFINITION
      // CLASS_DEFINITION
      char dummy10[sizeof (std::unique_ptr<clawn::compiler::DeclarationInfo>)];

      // C_type_descriptor
      char dummy11[sizeof (std::vector<const std::string>)];

      // C_members_descriptions
      char dummy12[sizeof (std::vector<std::pair<const std::string,std::vector<const std::string>>>)];

      // parameters
      // elements
      char dummy13[sizeof (std::vector<std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>>>)];

      // patterns
      // named_arguments
      char dummy14[sizeof (std::vector<std::pair<std::string,std::unique_ptr<ast::AST>>>)];

      // requirements
      char dummy15[sizeof (std::vector<std::shared_ptr<requirement::Requirement>>)];

      // lines
      // expressions
      char dummy16[sizeof (std::vector<std::unique_ptr<ast::AST>>)];

      // C_arguments
      char dummy17[sizeof (std::vector<std::vector<const std::string>>)];
    };

    /// The size of the largest semantic type.
    enum { size = sizeof (union_type) };

    /// A buffer to store semantic values.
    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me_;
      /// A buffer large enough to store any of the semantic values.
      char yyraw_[size];
    };

    /// Whether the content is built: if defined, the name of the stored type.
    const std::type_info *yytypeid_;
  };

#endif
    /// Backward compatibility (Bison 3.8).
    typedef value_type semantic_type;

    /// Symbol locations.
    typedef location location_type;

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const location_type& l, const std::string& m)
        : std::runtime_error (m)
        , location (l)
      {}

      syntax_error (const syntax_error& s)
        : std::runtime_error (s.what ())
        , location (s.location)
      {}

      ~syntax_error () YY_NOEXCEPT YY_NOTHROW;

      location_type location;
    };

    /// Token kinds.
    struct token
    {
      enum token_kind_type
      {
        YYEMPTY = -2,
    END = 0,                       // "end of file"
    YYerror = 256,                 // error
    YYUNDEF = 257,                 // "invalid token"
    MEMBER_IDENTIFIER = 258,       // MEMBER_IDENTIFIER
    IDENTIFIER = 259,              // IDENTIFIER
    RETURN = 260,                  // RETURN
    EQUAL = 261,                   // EQUAL
    LEFT_ARROW = 262,              // LEFT_ARROW
    BOTH_SIDE_ARROW = 263,         // BOTH_SIDE_ARROW
    VARIOUS = 264,                 // VARIOUS
    REFER = 265,                   // REFER
    ACCESS = 266,                  // ACCESS
    SEMICOLON = 267,               // SEMICOLON
    LEFT_BRACKET = 268,            // LEFT_BRACKET
    RIGHT_BRACKET = 269,           // RIGHT_BRACKET
    LEFT_CURLY_BRACE = 270,        // LEFT_CURLY_BRACE
    RIGHT_CURLY_BRACE = 271,       // RIGHT_CURLY_BRACE
    LEFT_PARENTHESIS = 272,        // LEFT_PARENTHESIS
    RIGHT_PARENTHESIS = 273,       // RIGHT_PARENTHESIS
    LEFT_PARENTHESIS_AND_EOL = 274, // LEFT_PARENTHESIS_AND_EOL
    OP_AND = 275,                  // OP_AND
    OP_OR = 276,                   // OP_OR
    OP_EQUAL = 277,                // OP_EQUAL
    OP_NOT_EQUAL = 278,            // OP_NOT_EQUAL
    OP_MORE_EQUAL = 279,           // OP_MORE_EQUAL
    OP_LESS_EQUAL = 280,           // OP_LESS_EQUAL
    OP_MORE = 281,                 // OP_MORE
    OP_LESS = 282,                 // OP_LESS
    PLUS = 283,                    // PLUS
    MINUS = 284,                   // MINUS
    ASTERISK = 285,                // ASTERISK
    SLASH = 286,                   // SLASH
    UMINUS = 287,                  // UMINUS
    DOUBLE_DOT = 288,              // DOUBLE_DOT
    TRIPLE_DOT = 289,              // TRIPLE_DOT
    DOT_IDENTIFIER = 290,          // DOT_IDENTIFIER
    RIGHT_ARROW_IDENTIFIER = 291,  // RIGHT_ARROW_IDENTIFIER
    UNION_CONSTRUCTOR = 292,       // UNION_CONSTRUCTOR
    C_TYPE_DECLARATION = 293,      // C_TYPE_DECLARATION
    C_FUNCTION_DECLARATION = 294,  // C_FUNCTION_DECLARATION
    C_FUNCTION_ARGUMENT = 295,     // C_FUNCTION_ARGUMENT
    C_FUNCTION_RETURN = 296,       // C_FUNCTION_RETURN
    FUNCTION_DEFINITION = 297,     // FUNCTION_DEFINITION
    METHOD_DEFINITION = 298,       // METHOD_DEFINITION
    CONSTRUCTOR_DEFINITION = 299,  // CONSTRUCTOR_DEFINITION
    DESTRUCTOR_DEFINITION = 300,   // DESTRUCTOR_DEFINITION
    SETTER_DEFINITION = 301,       // SETTER_DEFINITION
    GETTER_DEFINITION = 302,       // GETTER_DEFINITION
    CLASS_DEFINITION = 303,        // CLASS_DEFINITION
    STRUCTURE_DEFINITION = 304,    // STRUCTURE_DEFINITION
    UNION_DEFINITION = 305,        // UNION_DEFINITION
    REQUIREMENT = 306,             // REQUIREMENT
    SINGLE_QUOTATION = 307,        // SINGLE_QUOTATION
    USE = 308,                     // USE
    LAZY = 309,                    // LAZY
    COMMA = 310,                   // COMMA
    COLON = 311,                   // COLON
    ELSE = 312,                    // ELSE
    IF = 313,                      // IF
    REPEAT = 314,                  // REPEAT
    MATCH = 315,                   // MATCH
    ARM = 316,                     // ARM
    DEFAULT = 317,                 // DEFAULT
    IN = 318,                      // IN
    WHILE = 319,                   // WHILE
    GLOBAL = 320,                  // GLOBAL
    IMPORT = 321,                  // IMPORT
    STRING_LITERAL = 322,          // STRING_LITERAL
    INT_LITERAL = 323,             // INT_LITERAL
    FLOAT_LITERAL = 324,           // FLOAT_LITERAL
    EOL = 325                      // EOL
      };
      /// Backward compatibility alias (Bison 3.6).
      typedef token_kind_type yytokentype;
    };

    /// Token kind, as returned by yylex.
    typedef token::token_kind_type token_kind_type;

    /// Backward compatibility alias (Bison 3.6).
    typedef token_kind_type token_type;

    /// Symbol kinds.
    struct symbol_kind
    {
      enum symbol_kind_type
      {
        YYNTOKENS = 71, ///< Number of tokens.
        S_YYEMPTY = -2,
        S_YYEOF = 0,                             // "end of file"
        S_YYerror = 1,                           // error
        S_YYUNDEF = 2,                           // "invalid token"
        S_MEMBER_IDENTIFIER = 3,                 // MEMBER_IDENTIFIER
        S_IDENTIFIER = 4,                        // IDENTIFIER
        S_RETURN = 5,                            // RETURN
        S_EQUAL = 6,                             // EQUAL
        S_LEFT_ARROW = 7,                        // LEFT_ARROW
        S_BOTH_SIDE_ARROW = 8,                   // BOTH_SIDE_ARROW
        S_VARIOUS = 9,                           // VARIOUS
        S_REFER = 10,                            // REFER
        S_ACCESS = 11,                           // ACCESS
        S_SEMICOLON = 12,                        // SEMICOLON
        S_LEFT_BRACKET = 13,                     // LEFT_BRACKET
        S_RIGHT_BRACKET = 14,                    // RIGHT_BRACKET
        S_LEFT_CURLY_BRACE = 15,                 // LEFT_CURLY_BRACE
        S_RIGHT_CURLY_BRACE = 16,                // RIGHT_CURLY_BRACE
        S_LEFT_PARENTHESIS = 17,                 // LEFT_PARENTHESIS
        S_RIGHT_PARENTHESIS = 18,                // RIGHT_PARENTHESIS
        S_LEFT_PARENTHESIS_AND_EOL = 19,         // LEFT_PARENTHESIS_AND_EOL
        S_OP_AND = 20,                           // OP_AND
        S_OP_OR = 21,                            // OP_OR
        S_OP_EQUAL = 22,                         // OP_EQUAL
        S_OP_NOT_EQUAL = 23,                     // OP_NOT_EQUAL
        S_OP_MORE_EQUAL = 24,                    // OP_MORE_EQUAL
        S_OP_LESS_EQUAL = 25,                    // OP_LESS_EQUAL
        S_OP_MORE = 26,                          // OP_MORE
        S_OP_LESS = 27,                          // OP_LESS
        S_PLUS = 28,                             // PLUS
        S_MINUS = 29,                            // MINUS
        S_ASTERISK = 30,                         // ASTERISK
        S_SLASH = 31,                            // SLASH
        S_UMINUS = 32,                           // UMINUS
        S_DOUBLE_DOT = 33,                       // DOUBLE_DOT
        S_TRIPLE_DOT = 34,                       // TRIPLE_DOT
        S_DOT_IDENTIFIER = 35,                   // DOT_IDENTIFIER
        S_RIGHT_ARROW_IDENTIFIER = 36,           // RIGHT_ARROW_IDENTIFIER
        S_UNION_CONSTRUCTOR = 37,                // UNION_CONSTRUCTOR
        S_C_TYPE_DECLARATION = 38,               // C_TYPE_DECLARATION
        S_C_FUNCTION_DECLARATION = 39,           // C_FUNCTION_DECLARATION
        S_C_FUNCTION_ARGUMENT = 40,              // C_FUNCTION_ARGUMENT
        S_C_FUNCTION_RETURN = 41,                // C_FUNCTION_RETURN
        S_FUNCTION_DEFINITION = 42,              // FUNCTION_DEFINITION
        S_METHOD_DEFINITION = 43,                // METHOD_DEFINITION
        S_CONSTRUCTOR_DEFINITION = 44,           // CONSTRUCTOR_DEFINITION
        S_DESTRUCTOR_DEFINITION = 45,            // DESTRUCTOR_DEFINITION
        S_SETTER_DEFINITION = 46,                // SETTER_DEFINITION
        S_GETTER_DEFINITION = 47,                // GETTER_DEFINITION
        S_CLASS_DEFINITION = 48,                 // CLASS_DEFINITION
        S_STRUCTURE_DEFINITION = 49,             // STRUCTURE_DEFINITION
        S_UNION_DEFINITION = 50,                 // UNION_DEFINITION
        S_REQUIREMENT = 51,                      // REQUIREMENT
        S_SINGLE_QUOTATION = 52,                 // SINGLE_QUOTATION
        S_USE = 53,                              // USE
        S_LAZY = 54,                             // LAZY
        S_COMMA = 55,                            // COMMA
        S_COLON = 56,                            // COLON
        S_ELSE = 57,                             // ELSE
        S_IF = 58,                               // IF
        S_REPEAT = 59,                           // REPEAT
        S_MATCH = 60,                            // MATCH
        S_ARM = 61,                              // ARM
        S_DEFAULT = 62,                          // DEFAULT
        S_IN = 63,                               // IN
        S_WHILE = 64,                            // WHILE
        S_GLOBAL = 65,                           // GLOBAL
        S_IMPORT = 66,                           // IMPORT
        S_STRING_LITERAL = 67,                   // STRING_LITERAL
        S_INT_LITERAL = 68,                      // INT_LITERAL
        S_FLOAT_LITERAL = 69,                    // FLOAT_LITERAL
        S_EOL = 70,                              // EOL
        S_YYACCEPT = 71,                         // $accept
        S_program = 72,                          // program
        S_EOL_OR_EOF = 73,                       // EOL_OR_EOF
        S_lines = 74,                            // lines
        S_line = 75,                             // line
        S_definition = 76,                       // definition
        S_requirement_definition = 77,           // requirement_definition
        S_requirements = 78,                     // requirements
        S_requirement_clause = 79,               // requirement_clause
        S_block = 80,                            // block
        S_function_definition = 81,              // function_definition
        S_function_start = 82,                   // function_start
        S_parameters = 83,                       // parameters
        S_parameter = 84,                        // parameter
        S_structure_definition = 85,             // structure_definition
        S_structure_start = 86,                  // structure_start
        S_union_definition = 87,                 // union_definition
        S_elements = 88,                         // elements
        S_element = 89,                          // element
        S_C_type_declaration = 90,               // C_type_declaration
        S_C_function_declaration = 91,           // C_function_declaration
        S_C_members_descriptions = 92,           // C_members_descriptions
        S_C_type_descriptor = 93,                // C_type_descriptor
        S_C_arguments = 94,                      // C_arguments
        S_expressions = 95,                      // expressions
        S_if_start = 96,                         // if_start
        S_else_start = 97,                       // else_start
        S_repeat_start = 98,                     // repeat_start
        S_repeat_first_start = 99,               // repeat_first_start
        S_else_body = 100,                       // else_body
        S_pattern_start = 101,                   // pattern_start
        S_default_pattern_start = 102,           // default_pattern_start
        S_pattern = 103,                         // pattern
        S_default_pattern = 104,                 // default_pattern
        S_patterns = 105,                        // patterns
        S_match_start = 106,                     // match_start
        S_expression = 107,                      // expression
        S_access = 108,                          // access
        S_global_variable_definition = 109,      // global_variable_definition
        S_assign_variable = 110,                 // assign_variable
        S_return_value = 111,                    // return_value
        S_monomial = 112,                        // monomial
        S_call = 113,                            // call
        S_construction = 114,                    // construction
        S_named_arguments = 115,                 // named_arguments
        S_named_value = 116                      // named_value
      };
    };

    /// (Internal) symbol kind.
    typedef symbol_kind::symbol_kind_type symbol_kind_type;

    /// The number of tokens.
    static const symbol_kind_type YYNTOKENS = symbol_kind::YYNTOKENS;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol kind
    /// via kind ().
    ///
    /// Provide access to semantic value and location.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol () YY_NOEXCEPT
        : value ()
        , location ()
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      basic_symbol (basic_symbol&& that)
        : Base (std::move (that))
        , value ()
        , location (std::move (that.location))
      {
        switch (this->kind ())
    {
      case symbol_kind::S_FLOAT_LITERAL: // FLOAT_LITERAL
        value.move< double > (std::move (that.value));
        break;

      case symbol_kind::S_INT_LITERAL: // INT_LITERAL
        value.move< long long > (std::move (that.value));
        break;

      case symbol_kind::S_parameter: // parameter
      case symbol_kind::S_element: // element
        value.move< std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>> > (std::move (that.value));
        break;

      case symbol_kind::S_repeat_first_start: // repeat_first_start
      case symbol_kind::S_pattern: // pattern
        value.move< std::pair<std::string,std::unique_ptr<ast::AST>> > (std::move (that.value));
        break;

      case symbol_kind::S_requirement_clause: // requirement_clause
        value.move< std::shared_ptr<requirement::Requirement> > (std::move (that.value));
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
        value.move< std::string > (std::move (that.value));
        break;

      case symbol_kind::S_repeat_start: // repeat_start
        value.move< std::unique_ptr<RepetationInfo> > (std::move (that.value));
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
        value.move< std::unique_ptr<ast::AST> > (std::move (that.value));
        break;

      case symbol_kind::S_UNION_CONSTRUCTOR: // UNION_CONSTRUCTOR
        value.move< std::unique_ptr<ast::UnionConstructorInfo> > (std::move (that.value));
        break;

      case symbol_kind::S_CONSTRUCTOR_DEFINITION: // CONSTRUCTOR_DEFINITION
      case symbol_kind::S_DESTRUCTOR_DEFINITION: // DESTRUCTOR_DEFINITION
      case symbol_kind::S_SETTER_DEFINITION: // SETTER_DEFINITION
      case symbol_kind::S_GETTER_DEFINITION: // GETTER_DEFINITION
      case symbol_kind::S_CLASS_DEFINITION: // CLASS_DEFINITION
        value.move< std::unique_ptr<clawn::compiler::DeclarationInfo> > (std::move (that.value));
        break;

      case symbol_kind::S_C_type_descriptor: // C_type_descriptor
        value.move< std::vector<const std::string> > (std::move (that.value));
        break;

      case symbol_kind::S_C_members_descriptions: // C_members_descriptions
        value.move< std::vector<std::pair<const std::string,std::vector<const std::string>>> > (std::move (that.value));
        break;

      case symbol_kind::S_parameters: // parameters
      case symbol_kind::S_elements: // elements
        value.move< std::vector<std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>>> > (std::move (that.value));
        break;

      case symbol_kind::S_patterns: // patterns
      case symbol_kind::S_named_arguments: // named_arguments
        value.move< std::vector<std::pair<std::string,std::unique_ptr<ast::AST>>> > (std::move (that.value));
        break;

      case symbol_kind::S_requirements: // requirements
        value.move< std::vector<std::shared_ptr<requirement::Requirement>> > (std::move (that.value));
        break;

      case symbol_kind::S_lines: // lines
      case symbol_kind::S_expressions: // expressions
        value.move< std::vector<std::unique_ptr<ast::AST>> > (std::move (that.value));
        break;

      case symbol_kind::S_C_arguments: // C_arguments
        value.move< std::vector<std::vector<const std::string>> > (std::move (that.value));
        break;

      default:
        break;
    }

      }
#endif

      /// Copy constructor.
      basic_symbol (const basic_symbol& that);

      /// Constructors for typed symbols.
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, location_type&& l)
        : Base (t)
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const location_type& l)
        : Base (t)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, double&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const double& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, long long&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const long long& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::pair<std::string,std::unique_ptr<ast::AST>>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::pair<std::string,std::unique_ptr<ast::AST>>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::shared_ptr<requirement::Requirement>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::shared_ptr<requirement::Requirement>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::string&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::string& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::unique_ptr<RepetationInfo>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::unique_ptr<RepetationInfo>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::unique_ptr<ast::AST>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::unique_ptr<ast::AST>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::unique_ptr<ast::UnionConstructorInfo>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::unique_ptr<ast::UnionConstructorInfo>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::unique_ptr<clawn::compiler::DeclarationInfo>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::unique_ptr<clawn::compiler::DeclarationInfo>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::vector<const std::string>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::vector<const std::string>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::vector<std::pair<const std::string,std::vector<const std::string>>>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::vector<std::pair<const std::string,std::vector<const std::string>>>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::vector<std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>>>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::vector<std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>>>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::vector<std::pair<std::string,std::unique_ptr<ast::AST>>>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::vector<std::pair<std::string,std::unique_ptr<ast::AST>>>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::vector<std::shared_ptr<requirement::Requirement>>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::vector<std::shared_ptr<requirement::Requirement>>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::vector<std::unique_ptr<ast::AST>>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::vector<std::unique_ptr<ast::AST>>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::vector<std::vector<const std::string>>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::vector<std::vector<const std::string>>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

      /// Destroy the symbol.
      ~basic_symbol ()
      {
        clear ();
      }



      /// Destroy contents, and record that is empty.
      void clear () YY_NOEXCEPT
      {
        // User destructor.
        symbol_kind_type yykind = this->kind ();
        basic_symbol<Base>& yysym = *this;
        (void) yysym;
        switch (yykind)
        {
       default:
          break;
        }

        // Value type destructor.
switch (yykind)
    {
      case symbol_kind::S_FLOAT_LITERAL: // FLOAT_LITERAL
        value.template destroy< double > ();
        break;

      case symbol_kind::S_INT_LITERAL: // INT_LITERAL
        value.template destroy< long long > ();
        break;

      case symbol_kind::S_parameter: // parameter
      case symbol_kind::S_element: // element
        value.template destroy< std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>> > ();
        break;

      case symbol_kind::S_repeat_first_start: // repeat_first_start
      case symbol_kind::S_pattern: // pattern
        value.template destroy< std::pair<std::string,std::unique_ptr<ast::AST>> > ();
        break;

      case symbol_kind::S_requirement_clause: // requirement_clause
        value.template destroy< std::shared_ptr<requirement::Requirement> > ();
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
        value.template destroy< std::string > ();
        break;

      case symbol_kind::S_repeat_start: // repeat_start
        value.template destroy< std::unique_ptr<RepetationInfo> > ();
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
        value.template destroy< std::unique_ptr<ast::AST> > ();
        break;

      case symbol_kind::S_UNION_CONSTRUCTOR: // UNION_CONSTRUCTOR
        value.template destroy< std::unique_ptr<ast::UnionConstructorInfo> > ();
        break;

      case symbol_kind::S_CONSTRUCTOR_DEFINITION: // CONSTRUCTOR_DEFINITION
      case symbol_kind::S_DESTRUCTOR_DEFINITION: // DESTRUCTOR_DEFINITION
      case symbol_kind::S_SETTER_DEFINITION: // SETTER_DEFINITION
      case symbol_kind::S_GETTER_DEFINITION: // GETTER_DEFINITION
      case symbol_kind::S_CLASS_DEFINITION: // CLASS_DEFINITION
        value.template destroy< std::unique_ptr<clawn::compiler::DeclarationInfo> > ();
        break;

      case symbol_kind::S_C_type_descriptor: // C_type_descriptor
        value.template destroy< std::vector<const std::string> > ();
        break;

      case symbol_kind::S_C_members_descriptions: // C_members_descriptions
        value.template destroy< std::vector<std::pair<const std::string,std::vector<const std::string>>> > ();
        break;

      case symbol_kind::S_parameters: // parameters
      case symbol_kind::S_elements: // elements
        value.template destroy< std::vector<std::pair<std::string,std::optional<std::shared_ptr<requirement::Requirement>>>> > ();
        break;

      case symbol_kind::S_patterns: // patterns
      case symbol_kind::S_named_arguments: // named_arguments
        value.template destroy< std::vector<std::pair<std::string,std::unique_ptr<ast::AST>>> > ();
        break;

      case symbol_kind::S_requirements: // requirements
        value.template destroy< std::vector<std::shared_ptr<requirement::Requirement>> > ();
        break;

      case symbol_kind::S_lines: // lines
      case symbol_kind::S_expressions: // expressions
        value.template destroy< std::vector<std::unique_ptr<ast::AST>> > ();
        break;

      case symbol_kind::S_C_arguments: // C_arguments
        value.template destroy< std::vector<std::vector<const std::string>> > ();
        break;

      default:
        break;
    }

        Base::clear ();
      }

#if YYDEBUG || 0
      /// The user-facing name of this symbol.
      const char *name () const YY_NOEXCEPT
      {
        return Parser::symbol_name (this->kind ());
      }
#endif // #if YYDEBUG || 0


      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// Whether empty.
      bool empty () const YY_NOEXCEPT;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      value_type value;

      /// The location.
      location_type location;

    private:
#if YY_CPLUSPLUS < 201103L
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& that);
#endif
    };

    /// Type access provider for token (enum) based symbols.
    struct by_kind
    {
      /// The symbol kind as needed by the constructor.
      typedef token_kind_type kind_type;

      /// Default constructor.
      by_kind () YY_NOEXCEPT;

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      by_kind (by_kind&& that) YY_NOEXCEPT;
#endif

      /// Copy constructor.
      by_kind (const by_kind& that) YY_NOEXCEPT;

      /// Constructor from (external) token numbers.
      by_kind (kind_type t) YY_NOEXCEPT;



      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_kind& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// The symbol kind.
      /// \a S_YYEMPTY when empty.
      symbol_kind_type kind_;
    };

    /// Backward compatibility for a private implementation detail (Bison 3.6).
    typedef by_kind by_type;

    /// "External" symbols: returned by the scanner.
    struct symbol_type : basic_symbol<by_kind>
    {
      /// Superclass.
      typedef basic_symbol<by_kind> super_type;

      /// Empty symbol.
      symbol_type () YY_NOEXCEPT {}

      /// Constructor for valueless symbols, and symbols from each type.
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, location_type l)
        : super_type (token_kind_type (tok), std::move (l))
#else
      symbol_type (int tok, const location_type& l)
        : super_type (token_kind_type (tok), l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT (tok == token::END
                   || (token::YYerror <= tok && tok <= token::YYUNDEF)
                   || (token::RETURN <= tok && tok <= token::TRIPLE_DOT)
                   || (token::C_FUNCTION_ARGUMENT <= tok && tok <= token::C_FUNCTION_RETURN)
                   || (token::REQUIREMENT <= tok && tok <= token::IMPORT)
                   || tok == token::EOL);
#endif
      }
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, double v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const double& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT (tok == token::FLOAT_LITERAL);
#endif
      }
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, long long v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const long long& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT (tok == token::INT_LITERAL);
#endif
      }
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, std::string v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const std::string& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT ((token::MEMBER_IDENTIFIER <= tok && tok <= token::IDENTIFIER)
                   || (token::DOT_IDENTIFIER <= tok && tok <= token::RIGHT_ARROW_IDENTIFIER)
                   || (token::C_TYPE_DECLARATION <= tok && tok <= token::C_FUNCTION_DECLARATION)
                   || (token::FUNCTION_DEFINITION <= tok && tok <= token::METHOD_DEFINITION)
                   || (token::STRUCTURE_DEFINITION <= tok && tok <= token::UNION_DEFINITION)
                   || tok == token::STRING_LITERAL);
#endif
      }
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, std::unique_ptr<ast::UnionConstructorInfo> v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const std::unique_ptr<ast::UnionConstructorInfo>& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT (tok == token::UNION_CONSTRUCTOR);
#endif
      }
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, std::unique_ptr<clawn::compiler::DeclarationInfo> v, location_type l)
        : super_type (token_kind_type (tok), std::move (v), std::move (l))
#else
      symbol_type (int tok, const std::unique_ptr<clawn::compiler::DeclarationInfo>& v, const location_type& l)
        : super_type (token_kind_type (tok), v, l)
#endif
      {
#if !defined _MSC_VER || defined __clang__
        YY_ASSERT ((token::CONSTRUCTOR_DEFINITION <= tok && tok <= token::CLASS_DEFINITION));
#endif
      }
    };

    /// Build a parser object.
    Parser (Lexer  &lexer_yyarg, Driver  &driver_yyarg);
    virtual ~Parser ();

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    Parser (const Parser&) = delete;
    /// Non copyable.
    Parser& operator= (const Parser&) = delete;
#endif

    /// Parse.  An alias for parse ().
    /// \returns  0 iff parsing succeeded.
    int operator() ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

#if YYDEBUG || 0
    /// The user-facing name of the symbol whose (internal) number is
    /// YYSYMBOL.  No bounds checking.
    static const char *symbol_name (symbol_kind_type yysymbol);
#endif // #if YYDEBUG || 0


    // Implementation of make_symbol for each token kind.
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_END (location_type l)
      {
        return symbol_type (token::END, std::move (l));
      }
#else
      static
      symbol_type
      make_END (const location_type& l)
      {
        return symbol_type (token::END, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYerror (location_type l)
      {
        return symbol_type (token::YYerror, std::move (l));
      }
#else
      static
      symbol_type
      make_YYerror (const location_type& l)
      {
        return symbol_type (token::YYerror, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYUNDEF (location_type l)
      {
        return symbol_type (token::YYUNDEF, std::move (l));
      }
#else
      static
      symbol_type
      make_YYUNDEF (const location_type& l)
      {
        return symbol_type (token::YYUNDEF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MEMBER_IDENTIFIER (std::string v, location_type l)
      {
        return symbol_type (token::MEMBER_IDENTIFIER, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_MEMBER_IDENTIFIER (const std::string& v, const location_type& l)
      {
        return symbol_type (token::MEMBER_IDENTIFIER, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IDENTIFIER (std::string v, location_type l)
      {
        return symbol_type (token::IDENTIFIER, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_IDENTIFIER (const std::string& v, const location_type& l)
      {
        return symbol_type (token::IDENTIFIER, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RETURN (location_type l)
      {
        return symbol_type (token::RETURN, std::move (l));
      }
#else
      static
      symbol_type
      make_RETURN (const location_type& l)
      {
        return symbol_type (token::RETURN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EQUAL (location_type l)
      {
        return symbol_type (token::EQUAL, std::move (l));
      }
#else
      static
      symbol_type
      make_EQUAL (const location_type& l)
      {
        return symbol_type (token::EQUAL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LEFT_ARROW (location_type l)
      {
        return symbol_type (token::LEFT_ARROW, std::move (l));
      }
#else
      static
      symbol_type
      make_LEFT_ARROW (const location_type& l)
      {
        return symbol_type (token::LEFT_ARROW, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BOTH_SIDE_ARROW (location_type l)
      {
        return symbol_type (token::BOTH_SIDE_ARROW, std::move (l));
      }
#else
      static
      symbol_type
      make_BOTH_SIDE_ARROW (const location_type& l)
      {
        return symbol_type (token::BOTH_SIDE_ARROW, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_VARIOUS (location_type l)
      {
        return symbol_type (token::VARIOUS, std::move (l));
      }
#else
      static
      symbol_type
      make_VARIOUS (const location_type& l)
      {
        return symbol_type (token::VARIOUS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_REFER (location_type l)
      {
        return symbol_type (token::REFER, std::move (l));
      }
#else
      static
      symbol_type
      make_REFER (const location_type& l)
      {
        return symbol_type (token::REFER, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ACCESS (location_type l)
      {
        return symbol_type (token::ACCESS, std::move (l));
      }
#else
      static
      symbol_type
      make_ACCESS (const location_type& l)
      {
        return symbol_type (token::ACCESS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SEMICOLON (location_type l)
      {
        return symbol_type (token::SEMICOLON, std::move (l));
      }
#else
      static
      symbol_type
      make_SEMICOLON (const location_type& l)
      {
        return symbol_type (token::SEMICOLON, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LEFT_BRACKET (location_type l)
      {
        return symbol_type (token::LEFT_BRACKET, std::move (l));
      }
#else
      static
      symbol_type
      make_LEFT_BRACKET (const location_type& l)
      {
        return symbol_type (token::LEFT_BRACKET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RIGHT_BRACKET (location_type l)
      {
        return symbol_type (token::RIGHT_BRACKET, std::move (l));
      }
#else
      static
      symbol_type
      make_RIGHT_BRACKET (const location_type& l)
      {
        return symbol_type (token::RIGHT_BRACKET, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LEFT_CURLY_BRACE (location_type l)
      {
        return symbol_type (token::LEFT_CURLY_BRACE, std::move (l));
      }
#else
      static
      symbol_type
      make_LEFT_CURLY_BRACE (const location_type& l)
      {
        return symbol_type (token::LEFT_CURLY_BRACE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RIGHT_CURLY_BRACE (location_type l)
      {
        return symbol_type (token::RIGHT_CURLY_BRACE, std::move (l));
      }
#else
      static
      symbol_type
      make_RIGHT_CURLY_BRACE (const location_type& l)
      {
        return symbol_type (token::RIGHT_CURLY_BRACE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LEFT_PARENTHESIS (location_type l)
      {
        return symbol_type (token::LEFT_PARENTHESIS, std::move (l));
      }
#else
      static
      symbol_type
      make_LEFT_PARENTHESIS (const location_type& l)
      {
        return symbol_type (token::LEFT_PARENTHESIS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RIGHT_PARENTHESIS (location_type l)
      {
        return symbol_type (token::RIGHT_PARENTHESIS, std::move (l));
      }
#else
      static
      symbol_type
      make_RIGHT_PARENTHESIS (const location_type& l)
      {
        return symbol_type (token::RIGHT_PARENTHESIS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LEFT_PARENTHESIS_AND_EOL (location_type l)
      {
        return symbol_type (token::LEFT_PARENTHESIS_AND_EOL, std::move (l));
      }
#else
      static
      symbol_type
      make_LEFT_PARENTHESIS_AND_EOL (const location_type& l)
      {
        return symbol_type (token::LEFT_PARENTHESIS_AND_EOL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OP_AND (location_type l)
      {
        return symbol_type (token::OP_AND, std::move (l));
      }
#else
      static
      symbol_type
      make_OP_AND (const location_type& l)
      {
        return symbol_type (token::OP_AND, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OP_OR (location_type l)
      {
        return symbol_type (token::OP_OR, std::move (l));
      }
#else
      static
      symbol_type
      make_OP_OR (const location_type& l)
      {
        return symbol_type (token::OP_OR, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OP_EQUAL (location_type l)
      {
        return symbol_type (token::OP_EQUAL, std::move (l));
      }
#else
      static
      symbol_type
      make_OP_EQUAL (const location_type& l)
      {
        return symbol_type (token::OP_EQUAL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OP_NOT_EQUAL (location_type l)
      {
        return symbol_type (token::OP_NOT_EQUAL, std::move (l));
      }
#else
      static
      symbol_type
      make_OP_NOT_EQUAL (const location_type& l)
      {
        return symbol_type (token::OP_NOT_EQUAL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OP_MORE_EQUAL (location_type l)
      {
        return symbol_type (token::OP_MORE_EQUAL, std::move (l));
      }
#else
      static
      symbol_type
      make_OP_MORE_EQUAL (const location_type& l)
      {
        return symbol_type (token::OP_MORE_EQUAL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OP_LESS_EQUAL (location_type l)
      {
        return symbol_type (token::OP_LESS_EQUAL, std::move (l));
      }
#else
      static
      symbol_type
      make_OP_LESS_EQUAL (const location_type& l)
      {
        return symbol_type (token::OP_LESS_EQUAL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OP_MORE (location_type l)
      {
        return symbol_type (token::OP_MORE, std::move (l));
      }
#else
      static
      symbol_type
      make_OP_MORE (const location_type& l)
      {
        return symbol_type (token::OP_MORE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OP_LESS (location_type l)
      {
        return symbol_type (token::OP_LESS, std::move (l));
      }
#else
      static
      symbol_type
      make_OP_LESS (const location_type& l)
      {
        return symbol_type (token::OP_LESS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PLUS (location_type l)
      {
        return symbol_type (token::PLUS, std::move (l));
      }
#else
      static
      symbol_type
      make_PLUS (const location_type& l)
      {
        return symbol_type (token::PLUS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MINUS (location_type l)
      {
        return symbol_type (token::MINUS, std::move (l));
      }
#else
      static
      symbol_type
      make_MINUS (const location_type& l)
      {
        return symbol_type (token::MINUS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ASTERISK (location_type l)
      {
        return symbol_type (token::ASTERISK, std::move (l));
      }
#else
      static
      symbol_type
      make_ASTERISK (const location_type& l)
      {
        return symbol_type (token::ASTERISK, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SLASH (location_type l)
      {
        return symbol_type (token::SLASH, std::move (l));
      }
#else
      static
      symbol_type
      make_SLASH (const location_type& l)
      {
        return symbol_type (token::SLASH, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_UMINUS (location_type l)
      {
        return symbol_type (token::UMINUS, std::move (l));
      }
#else
      static
      symbol_type
      make_UMINUS (const location_type& l)
      {
        return symbol_type (token::UMINUS, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DOUBLE_DOT (location_type l)
      {
        return symbol_type (token::DOUBLE_DOT, std::move (l));
      }
#else
      static
      symbol_type
      make_DOUBLE_DOT (const location_type& l)
      {
        return symbol_type (token::DOUBLE_DOT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TRIPLE_DOT (location_type l)
      {
        return symbol_type (token::TRIPLE_DOT, std::move (l));
      }
#else
      static
      symbol_type
      make_TRIPLE_DOT (const location_type& l)
      {
        return symbol_type (token::TRIPLE_DOT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DOT_IDENTIFIER (std::string v, location_type l)
      {
        return symbol_type (token::DOT_IDENTIFIER, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_DOT_IDENTIFIER (const std::string& v, const location_type& l)
      {
        return symbol_type (token::DOT_IDENTIFIER, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RIGHT_ARROW_IDENTIFIER (std::string v, location_type l)
      {
        return symbol_type (token::RIGHT_ARROW_IDENTIFIER, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_RIGHT_ARROW_IDENTIFIER (const std::string& v, const location_type& l)
      {
        return symbol_type (token::RIGHT_ARROW_IDENTIFIER, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_UNION_CONSTRUCTOR (std::unique_ptr<ast::UnionConstructorInfo> v, location_type l)
      {
        return symbol_type (token::UNION_CONSTRUCTOR, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_UNION_CONSTRUCTOR (const std::unique_ptr<ast::UnionConstructorInfo>& v, const location_type& l)
      {
        return symbol_type (token::UNION_CONSTRUCTOR, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_C_TYPE_DECLARATION (std::string v, location_type l)
      {
        return symbol_type (token::C_TYPE_DECLARATION, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_C_TYPE_DECLARATION (const std::string& v, const location_type& l)
      {
        return symbol_type (token::C_TYPE_DECLARATION, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_C_FUNCTION_DECLARATION (std::string v, location_type l)
      {
        return symbol_type (token::C_FUNCTION_DECLARATION, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_C_FUNCTION_DECLARATION (const std::string& v, const location_type& l)
      {
        return symbol_type (token::C_FUNCTION_DECLARATION, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_C_FUNCTION_ARGUMENT (location_type l)
      {
        return symbol_type (token::C_FUNCTION_ARGUMENT, std::move (l));
      }
#else
      static
      symbol_type
      make_C_FUNCTION_ARGUMENT (const location_type& l)
      {
        return symbol_type (token::C_FUNCTION_ARGUMENT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_C_FUNCTION_RETURN (location_type l)
      {
        return symbol_type (token::C_FUNCTION_RETURN, std::move (l));
      }
#else
      static
      symbol_type
      make_C_FUNCTION_RETURN (const location_type& l)
      {
        return symbol_type (token::C_FUNCTION_RETURN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FUNCTION_DEFINITION (std::string v, location_type l)
      {
        return symbol_type (token::FUNCTION_DEFINITION, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FUNCTION_DEFINITION (const std::string& v, const location_type& l)
      {
        return symbol_type (token::FUNCTION_DEFINITION, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_METHOD_DEFINITION (std::string v, location_type l)
      {
        return symbol_type (token::METHOD_DEFINITION, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_METHOD_DEFINITION (const std::string& v, const location_type& l)
      {
        return symbol_type (token::METHOD_DEFINITION, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CONSTRUCTOR_DEFINITION (std::unique_ptr<clawn::compiler::DeclarationInfo> v, location_type l)
      {
        return symbol_type (token::CONSTRUCTOR_DEFINITION, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_CONSTRUCTOR_DEFINITION (const std::unique_ptr<clawn::compiler::DeclarationInfo>& v, const location_type& l)
      {
        return symbol_type (token::CONSTRUCTOR_DEFINITION, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DESTRUCTOR_DEFINITION (std::unique_ptr<clawn::compiler::DeclarationInfo> v, location_type l)
      {
        return symbol_type (token::DESTRUCTOR_DEFINITION, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_DESTRUCTOR_DEFINITION (const std::unique_ptr<clawn::compiler::DeclarationInfo>& v, const location_type& l)
      {
        return symbol_type (token::DESTRUCTOR_DEFINITION, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SETTER_DEFINITION (std::unique_ptr<clawn::compiler::DeclarationInfo> v, location_type l)
      {
        return symbol_type (token::SETTER_DEFINITION, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_SETTER_DEFINITION (const std::unique_ptr<clawn::compiler::DeclarationInfo>& v, const location_type& l)
      {
        return symbol_type (token::SETTER_DEFINITION, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_GETTER_DEFINITION (std::unique_ptr<clawn::compiler::DeclarationInfo> v, location_type l)
      {
        return symbol_type (token::GETTER_DEFINITION, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_GETTER_DEFINITION (const std::unique_ptr<clawn::compiler::DeclarationInfo>& v, const location_type& l)
      {
        return symbol_type (token::GETTER_DEFINITION, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CLASS_DEFINITION (std::unique_ptr<clawn::compiler::DeclarationInfo> v, location_type l)
      {
        return symbol_type (token::CLASS_DEFINITION, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_CLASS_DEFINITION (const std::unique_ptr<clawn::compiler::DeclarationInfo>& v, const location_type& l)
      {
        return symbol_type (token::CLASS_DEFINITION, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_STRUCTURE_DEFINITION (std::string v, location_type l)
      {
        return symbol_type (token::STRUCTURE_DEFINITION, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_STRUCTURE_DEFINITION (const std::string& v, const location_type& l)
      {
        return symbol_type (token::STRUCTURE_DEFINITION, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_UNION_DEFINITION (std::string v, location_type l)
      {
        return symbol_type (token::UNION_DEFINITION, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_UNION_DEFINITION (const std::string& v, const location_type& l)
      {
        return symbol_type (token::UNION_DEFINITION, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_REQUIREMENT (location_type l)
      {
        return symbol_type (token::REQUIREMENT, std::move (l));
      }
#else
      static
      symbol_type
      make_REQUIREMENT (const location_type& l)
      {
        return symbol_type (token::REQUIREMENT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SINGLE_QUOTATION (location_type l)
      {
        return symbol_type (token::SINGLE_QUOTATION, std::move (l));
      }
#else
      static
      symbol_type
      make_SINGLE_QUOTATION (const location_type& l)
      {
        return symbol_type (token::SINGLE_QUOTATION, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_USE (location_type l)
      {
        return symbol_type (token::USE, std::move (l));
      }
#else
      static
      symbol_type
      make_USE (const location_type& l)
      {
        return symbol_type (token::USE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LAZY (location_type l)
      {
        return symbol_type (token::LAZY, std::move (l));
      }
#else
      static
      symbol_type
      make_LAZY (const location_type& l)
      {
        return symbol_type (token::LAZY, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_COMMA (location_type l)
      {
        return symbol_type (token::COMMA, std::move (l));
      }
#else
      static
      symbol_type
      make_COMMA (const location_type& l)
      {
        return symbol_type (token::COMMA, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_COLON (location_type l)
      {
        return symbol_type (token::COLON, std::move (l));
      }
#else
      static
      symbol_type
      make_COLON (const location_type& l)
      {
        return symbol_type (token::COLON, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ELSE (location_type l)
      {
        return symbol_type (token::ELSE, std::move (l));
      }
#else
      static
      symbol_type
      make_ELSE (const location_type& l)
      {
        return symbol_type (token::ELSE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IF (location_type l)
      {
        return symbol_type (token::IF, std::move (l));
      }
#else
      static
      symbol_type
      make_IF (const location_type& l)
      {
        return symbol_type (token::IF, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_REPEAT (location_type l)
      {
        return symbol_type (token::REPEAT, std::move (l));
      }
#else
      static
      symbol_type
      make_REPEAT (const location_type& l)
      {
        return symbol_type (token::REPEAT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MATCH (location_type l)
      {
        return symbol_type (token::MATCH, std::move (l));
      }
#else
      static
      symbol_type
      make_MATCH (const location_type& l)
      {
        return symbol_type (token::MATCH, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ARM (location_type l)
      {
        return symbol_type (token::ARM, std::move (l));
      }
#else
      static
      symbol_type
      make_ARM (const location_type& l)
      {
        return symbol_type (token::ARM, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DEFAULT (location_type l)
      {
        return symbol_type (token::DEFAULT, std::move (l));
      }
#else
      static
      symbol_type
      make_DEFAULT (const location_type& l)
      {
        return symbol_type (token::DEFAULT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IN (location_type l)
      {
        return symbol_type (token::IN, std::move (l));
      }
#else
      static
      symbol_type
      make_IN (const location_type& l)
      {
        return symbol_type (token::IN, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_WHILE (location_type l)
      {
        return symbol_type (token::WHILE, std::move (l));
      }
#else
      static
      symbol_type
      make_WHILE (const location_type& l)
      {
        return symbol_type (token::WHILE, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_GLOBAL (location_type l)
      {
        return symbol_type (token::GLOBAL, std::move (l));
      }
#else
      static
      symbol_type
      make_GLOBAL (const location_type& l)
      {
        return symbol_type (token::GLOBAL, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IMPORT (location_type l)
      {
        return symbol_type (token::IMPORT, std::move (l));
      }
#else
      static
      symbol_type
      make_IMPORT (const location_type& l)
      {
        return symbol_type (token::IMPORT, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_STRING_LITERAL (std::string v, location_type l)
      {
        return symbol_type (token::STRING_LITERAL, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_STRING_LITERAL (const std::string& v, const location_type& l)
      {
        return symbol_type (token::STRING_LITERAL, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INT_LITERAL (long long v, location_type l)
      {
        return symbol_type (token::INT_LITERAL, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_INT_LITERAL (const long long& v, const location_type& l)
      {
        return symbol_type (token::INT_LITERAL, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FLOAT_LITERAL (double v, location_type l)
      {
        return symbol_type (token::FLOAT_LITERAL, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FLOAT_LITERAL (const double& v, const location_type& l)
      {
        return symbol_type (token::FLOAT_LITERAL, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EOL (location_type l)
      {
        return symbol_type (token::EOL, std::move (l));
      }
#else
      static
      symbol_type
      make_EOL (const location_type& l)
      {
        return symbol_type (token::EOL, l);
      }
#endif


  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    Parser (const Parser&);
    /// Non copyable.
    Parser& operator= (const Parser&);
#endif


    /// Stored state numbers (used for stacks).
    typedef unsigned char state_type;

    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    static state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT;

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT;

    static const short yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token kind \a t to a symbol kind.
    /// In theory \a t should be a token_kind_type, but character literals
    /// are valid, yet not members of the token_kind_type enum.
    static symbol_kind_type yytranslate_ (int t) YY_NOEXCEPT;

#if YYDEBUG || 0
    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#endif // #if YYDEBUG || 0


    // Tables.
    // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
    // STATE-NUM.
    static const short yypact_[];

    // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
    // Performed when YYTABLE does not specify something else to do.  Zero
    // means the default is an error.
    static const signed char yydefact_[];

    // YYPGOTO[NTERM-NUM].
    static const short yypgoto_[];

    // YYDEFGOTO[NTERM-NUM].
    static const unsigned char yydefgoto_[];

    // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
    // positive, shift that token.  If negative, reduce the rule whose
    // number is the opposite.  If YYTABLE_NINF, syntax error.
    static const short yytable_[];

    static const short yycheck_[];

    // YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
    // state STATE-NUM.
    static const signed char yystos_[];

    // YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.
    static const signed char yyr1_[];

    // YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.
    static const signed char yyr2_[];


#if YYDEBUG
    // YYRLINE[YYN] -- Source line where rule number YYN was defined.
    static const short yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r) const;
    /// Print the state stack on the debug stream.
    virtual void yy_stack_print_ () const;

    /// Debugging level.
    int yydebug_;
    /// Debug stream.
    std::ostream* yycdebug_;

    /// \brief Display a symbol kind, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state () YY_NOEXCEPT;

      /// The symbol kind as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s) YY_NOEXCEPT;

      /// Copy constructor.
      by_state (const by_state& that) YY_NOEXCEPT;

      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_state& that);

      /// The symbol kind (corresponding to \a state).
      /// \a symbol_kind::S_YYEMPTY when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// The state number used to denote an empty symbol.
      /// We use the initial state, as it does not have a value.
      enum { empty_state = 0 };

      /// The state.
      /// \a empty when empty.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Move or copy construction.
      stack_symbol_type (YY_RVREF (stack_symbol_type) that);
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) sym);
#if YY_CPLUSPLUS < 201103L
      /// Assignment, needed by push_back by some old implementations.
      /// Moves the contents of that.
      stack_symbol_type& operator= (stack_symbol_type& that);

      /// Assignment, needed by push_back by other implementations.
      /// Needed by some other old implementations.
      stack_symbol_type& operator= (const stack_symbol_type& that);
#endif
    };

    /// A stack with random access from its top.
    template <typename T, typename S = std::vector<T> >
    class stack
    {
    public:
      // Hide our reversed order.
      typedef typename S::iterator iterator;
      typedef typename S::const_iterator const_iterator;
      typedef typename S::size_type size_type;
      typedef typename std::ptrdiff_t index_type;

      stack (size_type n = 200) YY_NOEXCEPT
        : seq_ (n)
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Non copyable.
      stack (const stack&) = delete;
      /// Non copyable.
      stack& operator= (const stack&) = delete;
#endif

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      const T&
      operator[] (index_type i) const
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      T&
      operator[] (index_type i)
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Steal the contents of \a t.
      ///
      /// Close to move-semantics.
      void
      push (YY_MOVE_REF (T) t)
      {
        seq_.push_back (T ());
        operator[] (0).move (t);
      }

      /// Pop elements from the stack.
      void
      pop (std::ptrdiff_t n = 1) YY_NOEXCEPT
      {
        for (; 0 < n; --n)
          seq_.pop_back ();
      }

      /// Pop all elements from the stack.
      void
      clear () YY_NOEXCEPT
      {
        seq_.clear ();
      }

      /// Number of elements on the stack.
      index_type
      size () const YY_NOEXCEPT
      {
        return index_type (seq_.size ());
      }

      /// Iterator on top of the stack (going downwards).
      const_iterator
      begin () const YY_NOEXCEPT
      {
        return seq_.begin ();
      }

      /// Bottom of the stack.
      const_iterator
      end () const YY_NOEXCEPT
      {
        return seq_.end ();
      }

      /// Present a slice of the top of a stack.
      class slice
      {
      public:
        slice (const stack& stack, index_type range) YY_NOEXCEPT
          : stack_ (stack)
          , range_ (range)
        {}

        const T&
        operator[] (index_type i) const
        {
          return stack_[range_ - i];
        }

      private:
        const stack& stack_;
        index_type range_;
      };

    private:
#if YY_CPLUSPLUS < 201103L
      /// Non copyable.
      stack (const stack&);
      /// Non copyable.
      stack& operator= (const stack&);
#endif
      /// The wrapped container.
      S seq_;
    };


    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param sym  the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a sym.value is stolen.
    void yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym);

    /// Pop \a n symbols from the stack.
    void yypop_ (int n = 1) YY_NOEXCEPT;

    /// Constants.
    enum
    {
      yylast_ = 869,     ///< Last index in yytable_.
      yynnts_ = 46,  ///< Number of nonterminal symbols.
      yyfinal_ = 75 ///< Termination state number.
    };


    // User arguments.
    Lexer  &lexer;
    Driver  &driver;

  };


#line 5 "./src/lib/compiler/parser/parser.yy"
} } // clawn::compiler
#line 2940 "/Users/ueharanaoto/Desktop/Clawn/build/parser.hpp"




#endif // !YY_YY_USERS_UEHARANAOTO_DESKTOP_CLAWN_BUILD_PARSER_HPP_INCLUDED
