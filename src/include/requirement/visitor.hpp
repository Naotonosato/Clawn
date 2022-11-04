

namespace clawn::requirement
{
template <typename ReturnType>
class Visitor
{
    protected:
    public:
    Visitor() = default;

    virtual ReturnType visit(const type::VoidType& node) const = 0;
    virtual ReturnType visit(const type::CType& node) const = 0;
    virtual ReturnType visit(const type::BooleanType& node) const = 0;
    virtual ReturnType visit(const type::IntegerType& node) const = 0;
    virtual ReturnType visit(const type::RealNumberType& node) const = 0;
    virtual ReturnType visit(const type::ReferenceType& node) const = 0;
    virtual ReturnType visit(const type::ListType& node) const = 0;
    virtual ReturnType visit(const type::VariantType& node) const = 0;
    virtual ReturnType visit(const type::StructureType& node) const = 0;
    virtual ReturnType visit(const type::FunctionType& node) const = 0;
    virtual ReturnType visit(const type::UnsolvedType& node) const = 0;

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

}  // namespace clawn::requirement