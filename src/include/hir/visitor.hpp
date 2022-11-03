#pragma once
#include "include/requirement/type.hpp"
#include "include/hir/hir.hpp"

// forward declarations

namespace clawn::hir
{
template <class VisitorClass>
using HIRVisitor = utils::VisitorWrapper<VisitorClass, hir::HIR>;

// Visitor class forces derived class to implement visitor function takes
// each kind of hir::HIR as an argument.
template <typename ReturnType>
class Visitor
{
    protected:
    public:
    Visitor() = default;

    virtual ReturnType visit(const hir::Root& node) const = 0;
    virtual ReturnType visit(const hir::Integer& node) const = 0;
    virtual ReturnType visit(const hir::Float& node) const = 0;
    virtual ReturnType visit(const hir::String& node) const = 0;
    virtual ReturnType visit(const hir::Reference& node) const = 0;
    virtual ReturnType visit(const hir::Dereference& node) const = 0;
    virtual ReturnType visit(const hir::List& node) const = 0;
    virtual ReturnType visit(const hir::Variable& node) const = 0;
    virtual ReturnType visit(const hir::Function& node) const = 0;
    virtual ReturnType visit(const hir::Assignment& node) const = 0;
    virtual ReturnType visit(const hir::FunctionCall& node) const = 0;
    virtual ReturnType visit(const hir::AccessElement& node) const = 0;
    virtual ReturnType visit(const hir::AccessList& node) const = 0;
    virtual ReturnType visit(const hir::Construction& node) const = 0;
    virtual ReturnType visit(const hir::UnionConstruction& node) const = 0;
    virtual ReturnType visit(const hir::Block& node) const = 0;
    virtual ReturnType visit(const hir::Match& node) const = 0;
    virtual ReturnType visit(const hir::If& node) const = 0;
    virtual ReturnType visit(const hir::Loop& node) const = 0;
    virtual ReturnType visit(const hir::BinaryExpression& node) const = 0;
    virtual ReturnType visit(const hir::SetResult& node) const = 0;
    virtual ReturnType visit(const hir::Return& node) const = 0;

    ReturnType visit(const hir::HIR& node)
    {
        auto& variant = node.get_variant();
        return std::visit([this](const auto& arg) { return visit(arg); },
                          variant);
    }

    template <typename HIRType>
    ReturnType visit(const HIRType& hir)
    {
        return std::visit([this](const auto& arg) { return visit(arg); },
                          hir.get_variant());
    }

    template <typename ArgumentType>
    ReturnType operator()(ArgumentType& arg) const
    {
        return visit(arg);
    }
};

class Cloner : public Visitor<std::unique_ptr<HIR>>
{
    private:
    requirement::TypeEnvironment& type_environment;
    requirement::ClonedTypeMap& cloned_type_map;

    public:
    Cloner(requirement::TypeEnvironment& type_environment,
           requirement::ClonedTypeMap& cloned_type_map)
        : type_environment(type_environment), cloned_type_map(cloned_type_map)
    {
    }
    std::unique_ptr<HIR> visit(const hir::Root& node) const override;
    std::unique_ptr<HIR> visit(const hir::Integer& node) const override;
    std::unique_ptr<HIR> visit(const hir::Float& node) const override;
    std::unique_ptr<HIR> visit(const hir::String& node) const override;
    std::unique_ptr<HIR> visit(const hir::Reference& node) const override;
    std::unique_ptr<HIR> visit(const hir::Dereference& node) const override;
    std::unique_ptr<HIR> visit(const hir::List& node) const override;
    std::unique_ptr<HIR> visit(const hir::Variable& node) const override;
    std::unique_ptr<HIR> visit(const hir::Function& node) const override;
    std::unique_ptr<HIR> visit(const hir::Assignment& node) const override;
    std::unique_ptr<HIR> visit(const hir::FunctionCall& node) const override;
    std::unique_ptr<HIR> visit(const hir::AccessElement& node) const override;
    std::unique_ptr<HIR> visit(const hir::AccessList& node) const override;
    std::unique_ptr<HIR> visit(const hir::Construction& node) const override;
    std::unique_ptr<HIR> visit(
        const hir::UnionConstruction& node) const override;
    std::unique_ptr<HIR> visit(const hir::Block& node) const override;
    std::unique_ptr<HIR> visit(const hir::Match& node) const override;
    std::unique_ptr<HIR> visit(const hir::If& node) const override;
    std::unique_ptr<HIR> visit(const hir::Loop& node) const override;
    std::unique_ptr<HIR> visit(const hir::BinaryExpression&) const override;
    std::unique_ptr<HIR> visit(const hir::SetResult& node) const override;
    std::unique_ptr<HIR> visit(const hir::Return& node) const override;
};

class Printer : public Visitor<std::string>
{
    public:
    Printer() = default;
    std::string visit(const hir::Root& node) const override;
    std::string visit(const hir::Integer& node) const override;
    std::string visit(const hir::Float& node) const override;
    std::string visit(const hir::String& node) const override;
    std::string visit(const hir::Reference& node) const override;
    std::string visit(const hir::Dereference& node) const override;
    std::string visit(const hir::List& node) const override;
    std::string visit(const hir::Variable& node) const override;
    std::string visit(const hir::Function& node) const override;
    std::string visit(const hir::Assignment& node) const override;
    std::string visit(const hir::FunctionCall& node) const override;
    std::string visit(const hir::AccessElement& node) const override;
    std::string visit(const hir::AccessList& node) const override;
    std::string visit(const hir::Construction& node) const override;
    std::string visit(const hir::UnionConstruction& node) const override;
    std::string visit(const hir::Block& node) const override;
    std::string visit(const hir::Match& node) const override;
    std::string visit(const hir::If& node) const override;
    std::string visit(const hir::Loop& node) const override;
    std::string visit(const hir::BinaryExpression& node) const override;
    std::string visit(const hir::SetResult& node) const override;
    std::string visit(const hir::Return& node) const override;
};

class Flattener : public HIRVisitor<Flattener>
{
};

// Flattener f;

}  // namespace clawn::hir