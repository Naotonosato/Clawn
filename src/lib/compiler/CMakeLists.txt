cmake_minimum_required(VERSION 2.8)
add_library(Compiler STATIC 
    compiler.cpp
    parser/driver.cpp
    pipeline/ast_to_hir.cpp
    pipeline/hir_verifier.cpp
    pipeline/hir_to_mir.cpp
    pipeline/mir_to_llvm_ir.cpp
    pipeline/llvm_to_binary.cpp
    pipeline/monomorphizer.cpp
    module/module.cpp
    error.cpp
    )

