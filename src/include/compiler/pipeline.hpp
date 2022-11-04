#pragma once
#include <memory>
#include <vector>
#include "include/compiler/error.hpp"
#include "llvm/IR/IRBuilder.h"

namespace clawn::ast
{
class AST;
}

namespace clawn::hir
{
class HIR;
}

namespace clawn::requirement
{
class TypeEnvironment;
}

namespace clawn::mir
{
class Value;
class Context;
}  // namespace clawn::mir

namespace llvm
{
class Module;
class LLVMContext;
}  // namespace llvm

namespace clawn::compiler
{
class Module;
}

namespace clawn::compiler::pipeline
{
void create_binary(const std::string& name, llvm::Module* llvm_module);
std::unique_ptr<hir::HIR> ast_to_hir(
    const ast::AST& node, const std::shared_ptr<compiler::Module> clawn_module);

std::vector<std::unique_ptr<hir::HIR>> monomorphizer(
    hir::HIR& hir,
    std::shared_ptr<requirement::TypeEnvironment> type_environment);

std::optional<Error> verify_hir(const hir::HIR& hir,
                                const std::shared_ptr<Module> clawn_module);

std::shared_ptr<mir::Value> hir_to_mir(const hir::HIR&,
                                       std::shared_ptr<mir::Context>);

void mir_to_llvm_ir(mir::Context& mir_context, llvm::LLVMContext& llvm_context,
                    llvm::Module& llvm_module, llvm::IRBuilder<>& ir_builder);
}  // namespace clawn::compiler::pipeline