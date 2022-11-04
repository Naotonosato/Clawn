#pragma once

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif
#include <string>

#include "location.hh"
#include "driver.hpp"

namespace clawn::compiler
{
class Driver;

struct DeclarationInfo;

class PreLexer : public yyFlexLexer
{
    public:
    Driver &driver;
    PreLexer(std::istream *in, Driver &driver)
        : yyFlexLexer(in), driver(driver){};

    using yyFlexLexer::yylex;

    virtual int prelex();
};
}  // namespace clawn::compiler
