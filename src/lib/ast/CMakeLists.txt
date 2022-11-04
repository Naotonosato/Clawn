cmake_minimum_required(VERSION 2.8)
add_library(Ast STATIC 
    builder.cpp
    ast.cpp
    visitor.cpp
    )
    