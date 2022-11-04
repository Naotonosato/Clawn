#pragma once

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif
#include <string>
#include "driver.hpp"
#include "location.hh"
#include "parser.hpp"

namespace clawn::compiler
{
class Lexer : public yyFlexLexer
{
    public:
    Driver &driver;
    Lexer(std::istream *in, Driver &driver) : yyFlexLexer(in), driver(driver){};

    using FlexLexer::yylex;

    virtual int yylex(compiler::Parser::semantic_type *const lval,
                      compiler::Parser::location_type *location);

    private:
    compiler::Parser::semantic_type *yylval = nullptr;
};

}  // namespace clawn::compiler
