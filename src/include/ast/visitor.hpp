#pragma once
#include "include/ast/ast.hpp"
#include "include/utils/variant_wrapper.hpp"

namespace clawn::ast
{
// Visitor class forces derived class to implement visitor function takes
// each kind of ast::AST as an argument.

// using ASTVisitor = utils::Visitor

template <typename ReturnType>
class Visitor
{
    public:
    Visitor() = default;

    virtual ReturnType visit(const ast::Root& node) const = 0;
    virtual ReturnType visit(const ast::Integer& node) const = 0;
    virtual ReturnType visit(const ast::Float& node) const = 0;
    virtual ReturnType visit(const ast::List& node) const = 0;
    virtual ReturnType visit(const ast::String& node) const = 0;
    virtual ReturnType visit(const ast::Reference& node) const = 0;
    virtual ReturnType visit(const ast::Dereference& node) const = 0;
    virtual ReturnType visit(const ast::Variable& node) const = 0;
    virtual ReturnType visit(const ast::VariableDefinition& node) const = 0;
    virtual ReturnType visit(const ast::GlobalVariable& node) const = 0;
    virtual ReturnType visit(const ast::Assignment& node) const = 0;
    virtual ReturnType visit(const ast::BinaryExpression& node) const = 0;
    virtual ReturnType visit(const ast::FunctionCall& node) const = 0;
    virtual ReturnType visit(const ast::AccessElement& node) const = 0;
    virtual ReturnType visit(const ast::AccessList& node) const = 0;
    virtual ReturnType visit(const ast::DeepCopy& node) const = 0;
    virtual ReturnType visit(const ast::Block& node) const = 0;
    virtual ReturnType visit(const ast::Match& node) const = 0;
    virtual ReturnType visit(const ast::If& node) const = 0;
    virtual ReturnType visit(const ast::Repeat& node) const = 0;
    virtual ReturnType visit(
        const ast::GenericFunctionDeclaration& node) const = 0;
    virtual ReturnType visit(const ast::GenericFunction& node) const = 0;
    virtual ReturnType visit(
        const ast::UnionConstructorDeclaration& node) const = 0;
    virtual ReturnType visit(const ast::GenericUnion& node) const = 0;
    virtual ReturnType visit(const ast::Construction& node) const = 0;
    virtual ReturnType visit(
        const ast::GenericStructureDeclaration& node) const = 0;
    virtual ReturnType visit(const ast::GenericStructure& node) const = 0;
    virtual ReturnType visit(const ast::GenericClass& node) const = 0;
    virtual ReturnType visit(const ast::TypeID& node) const = 0;
    virtual ReturnType visit(const ast::Return& node) const = 0;
    virtual ReturnType visit(const ast::SetResult& node) const = 0;
    virtual ReturnType visit(const ast::CTypeDeclaration& node) const = 0;
    virtual ReturnType visit(const ast::CFunctionDeclaration& node) const = 0;

    ReturnType visit(const ast::AST& node)
    {
        auto& variant = node.get_variant();
        return std::visit([this](const auto& arg) { return visit(arg); },
                          variant);
    }

    template <typename ArgumentType>
    ReturnType operator()(ArgumentType& arg) const
    {
        return visit(arg);
    }
};

class Flattener
{
    std::unique_ptr<class FlattenerImpl> impl;

    public:
    Flattener();
    ~Flattener();
    std::vector<std::reference_wrapper<const AST>> visit(const AST& node);
};

class Cloner
{
    std::unique_ptr<class ClonerImpl> impl;

    public:
    Cloner();
    ~Cloner();
    std::unique_ptr<AST> visit(const AST& node) const;
};

}  // namespace clawn::ast