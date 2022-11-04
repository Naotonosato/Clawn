#pragma once

namespace clawn::ast
{
class Builder;
}
namespace clawn::hir
{
class Builder;
}

namespace clawn::compiler
{
int compile(int argc, char** argv);
class CompilerOptions;
class Compiler
{
    private:
    CompilerOptions& options;

    public:
    Compiler(CompilerOptions& options) : options(options) {}
    Compiler() = delete;
    Compiler(const Compiler&) = delete;
    Compiler(Compiler&&) = delete;
};
}  // namespace clawn::compiler