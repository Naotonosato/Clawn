#pragma once
#include <vector>
#include <memory>
#include <string>
#include <type_traits>
#include "include/utils/variant_wrapper.hpp"
#include "include/utils/move_constructible.hpp"
#include "include/location/location.hpp"
#include "include/requirement/type.hpp"
#include "include/utils/pattern_match.hpp"
#include "include/algorithm/tree.hpp"

namespace clawn::hir
{
class HIR;
}  // namespace clawn::hir
// forward declarations

namespace clawn::hir
{
class HIRId
{
    private:
    size_t number;

    public:
    bool operator==(const HIRId& rhs) const { return number == rhs.number; }
    bool operator!=(const HIRId& rhs) const { return number != rhs.number; }
    bool operator<(const HIRId& rhs) const { return number < rhs.number; }
    bool operator>(const HIRId& rhs) const { return number > rhs.number; }
};

using HIRID = size_t;

class HIRBase : public algorithm::Tree<HIR>
{
    private:
    HIRID id;
    std::shared_ptr<requirement::Type> type;
    const std::shared_ptr<location::Location> location;

    protected:
    HIRBase(std::shared_ptr<requirement::Type> type,
            const std::shared_ptr<location::Location> location)
        : id(utils::get_unique_number()), type(type), location(location)
    {
    }
    HIRBase(HIRBase&&) = default;
    const std::vector<std::reference_wrapper<const HIR>> get_children() const;

    public:
    HIRID get_id() const;
    const std::shared_ptr<requirement::Type> get_type(
        requirement::TypeSolver& solver) const;
    const std::shared_ptr<requirement::Type> get_type() const;
    const std::shared_ptr<requirement::Type> get_type_without_solving() const;
    const std::shared_ptr<location::Location> get_location() const;
};

class Root : public HIRBase
{
    private:
    std::vector<std::unique_ptr<HIR>> program;

    protected:
    Root(const std::shared_ptr<requirement::Type> type,
         const std::shared_ptr<location::Location> location,
         std::vector<std::unique_ptr<HIR>>&& program);

    public:
    const std::vector<std::reference_wrapper<const HIR>> get_children()
        const override;
    void insert(std::vector<std::unique_ptr<HIR>>&& new_hirs);
    std::vector<std::unique_ptr<HIR>>& get_hirs();
};

class Integer : public HIRBase
{
    private:
    const int64_t initial_value;

    protected:
    Integer(const std::shared_ptr<requirement::Type> type,
            const std::shared_ptr<location::Location> location,
            int64_t initial_value);

    public:
    const int64_t get_initial_value() const;
    const std::vector<std::reference_wrapper<const HIR>> get_children()
        const override;
};

class Float : public HIRBase
{
    private:
    const long double initial_value;

    protected:
    Float(const std::shared_ptr<requirement::Type> type,
          const std::shared_ptr<location::Location> location,
          long double initial_value);

    public:
    const long double get_initial_value() const;
    const std::vector<std::reference_wrapper<const HIR>> get_children()
        const override;
};

class String : public HIRBase
{
    private:
    const std::string initial_value;

    protected:
    String(const std::shared_ptr<requirement::Type> type,
           const std::shared_ptr<location::Location> location,
           const std::string& initial_value);

    public:
    const std::string& get_initial_value() const;
    const std::vector<std::reference_wrapper<const HIR>> get_children()
        const override;
};

class Reference : public HIRBase
{
    private:
    std::unique_ptr<HIR> refer_to;

    public:
    Reference(const std::shared_ptr<requirement::Type> type,
              std::shared_ptr<location::Location> location,
              std::unique_ptr<HIR>&& refer_to)
        : HIRBase(type, location), refer_to(std::move(refer_to))
    {
    }
    const HIR& get_refer_to() const;
    const std::vector<std::reference_wrapper<const HIR>> get_children()
        const override;
};

class Dereference : public HIRBase
{
    private:
    std::unique_ptr<HIR> target;

    public:
    Dereference(const std::shared_ptr<requirement::Type> type,
                std::shared_ptr<location::Location> location,
                std::unique_ptr<HIR>&& target)
        : HIRBase(type, location), target(std::move(target))
    {
    }
    const HIR& get_target() const;
    const std::vector<std::reference_wrapper<const HIR>> get_children()
        const override;
};

class List : public HIRBase
{
    std::vector<std::unique_ptr<HIR>> initial_values;

    protected:
    List(const std::shared_ptr<requirement::Type> type,
         const std::shared_ptr<location::Location> location,
         std::vector<std::unique_ptr<HIR>>&& initial_values);

    public:
    const std::vector<std::reference_wrapper<const HIR>> get_children()
        const override;
    const std::vector<std::reference_wrapper<const HIR>> get_initial_values()
        const;
};

class Function : public HIRBase
{
    private:
    std::unique_ptr<HIR> body;
    const std::string name;
    bool _is_declaration_only;

    protected:
    Function(const std::shared_ptr<requirement::Type> type,
             const std::shared_ptr<location::Location> location,
             const std::string& name, std::unique_ptr<HIR>&& body,
             bool is_declaration_only = false);

    public:
    const std::string& get_name() const;
    const HIR& get_body() const;
    const std::vector<std::reference_wrapper<const HIR>> get_children()
        const override;
    bool is_declaration_only() const;
};

class Assignment : public HIRBase
{
    private:
    std::unique_ptr<HIR> right_hand_side;
    std::unique_ptr<HIR> left_hand_side;
    bool _is_both_reference;

    protected:
    Assignment(const std::shared_ptr<requirement::Type> type,
               const std::shared_ptr<location::Location> location,
               std::unique_ptr<HIR>&& right_hand_side,
               std::unique_ptr<HIR>&& left_hand_side,
               bool _is_both_reference = false);

    public:
    const HIR& get_right_hand_side() const;
    const HIR& get_left_hand_side() const;
    const std::vector<std::reference_wrapper<const HIR>> get_children()
        const override;
    const bool is_both_reference() const;
};

class Variable : public HIRBase
{
    private:
    const std::string name;

    protected:
    Variable(const std::shared_ptr<requirement::Type> type,
             const std::string& name,
             const std::shared_ptr<location::Location> location);

    public:
    const std::string& get_name() const;
    const std::vector<std::reference_wrapper<const HIR>> get_children()
        const override;
    bool is_global() const;
};

class GlobalVariable
{
    protected:
    GlobalVariable(const std::shared_ptr<requirement::Type> type,
                   const std::shared_ptr<location::Location> location);

    public:
    // const std::vector<std::reference_wrapper<const HIR>> get_children() const
    // override;
};

class FunctionCall : public HIRBase
{
    private:
    std::unique_ptr<HIR> function_obj;  // when this is none, name is not
                                        // solved.
    std::vector<std::unique_ptr<HIR>> arguments;

    protected:
    FunctionCall(const std::shared_ptr<requirement::Type> type,
                 const std::shared_ptr<location::Location> location,
                 std::unique_ptr<HIR>&& function_obj,
                 std::vector<std::unique_ptr<HIR>>&& arguments);

    public:
    const HIR& get_function_obj() const;
    const std::vector<std::reference_wrapper<const HIR>> get_arguments() const;
    const std::vector<std::reference_wrapper<const HIR>> get_children()
        const override;
};

class Argument : public HIRBase
{
    private:
    const std::string name;

    protected:
    Argument(const std::shared_ptr<requirement::Type> type,
             const std::shared_ptr<location::Location> location,
             const std::string name);

    public:
    const std::vector<std::reference_wrapper<const HIR>> get_children()
        const override;
};

class AccessElement : public HIRBase
{
    private:
    std::unique_ptr<HIR> left_hand_side;
    const std::string element_name;

    protected:
    AccessElement(const std::shared_ptr<requirement::Type> type,
                  const std::shared_ptr<location::Location> location,
                  std::unique_ptr<HIR>&& left_hand_side,
                  const std::string element_name);

    public:
    const HIR& get_left_hand_side() const;
    const std::string& get_element_name() const;
    const std::vector<std::reference_wrapper<const HIR>> get_children()
        const override;
    // std::shared_ptr<requirement::Type> get_type() const;
};

class AccessList : public HIRBase
{
    private:
    std::unique_ptr<HIR> list;
    std::unique_ptr<HIR> index;

    public:
    AccessList(const std::shared_ptr<requirement::Type> type,
               std::shared_ptr<location::Location> location,
               std::unique_ptr<HIR>&& list, std::unique_ptr<HIR>&& index)
        : HIRBase(type, location),
          list(std::move(list)),
          index(std::move(index))
    {
    }

    const HIR& get_list() const;
    const HIR& get_index() const;
    const std::vector<std::reference_wrapper<const HIR>> get_children()
        const override;
};

class Construction : public HIRBase
{
    private:
    std::vector<std::pair<std::string, std::unique_ptr<HIR>>> arguments;

    protected:
    Construction(
        const std::shared_ptr<requirement::Type> type,
        const std::shared_ptr<location::Location> location,
        std::vector<std::pair<std::string, std::unique_ptr<HIR>>>&& arguments);

    public:
    const std::vector<std::reference_wrapper<const HIR>> get_children()
        const override;
    const std::vector<std::pair<std::string, std::reference_wrapper<const HIR>>>
    get_arguments() const;
};

class UnionConstruction : public HIRBase
{
    private:
    std::pair<std::string, std::unique_ptr<HIR>> argument;

    protected:
    UnionConstruction(const std::shared_ptr<requirement::Type> type,
                      const std::shared_ptr<location::Location> location,
                      std::pair<std::string, std::unique_ptr<HIR>>&& argument);

    public:
    const std::vector<std::reference_wrapper<const HIR>> get_children()
        const override;
    const std::pair<std::string, std::reference_wrapper<const HIR>>
    get_argument() const;
};

class Cast
{
};

class Block : public HIRBase
{
    private:
    std::vector<std::unique_ptr<HIR>> hir_list;

    protected:
    Block(const std::shared_ptr<requirement::Type> type,
          const std::shared_ptr<location::Location> location,
          std::vector<std::unique_ptr<HIR>>&& hir_list);

    public:
    std::vector<std::unique_ptr<HIR>>& get_mutable_list();
    const std::vector<std::reference_wrapper<const HIR>> get_list() const;
    const std::vector<std::reference_wrapper<const HIR>> get_children()
        const override;
};

class Match : public HIRBase
{
    public:
    using Pattern = std::pair<std::string, std::unique_ptr<HIR>>;

    private:
    std::unique_ptr<HIR> target;
    std::vector<Pattern> patterns;
    std::optional<std::unique_ptr<HIR>> default_case;

    public:
    Match(const std::shared_ptr<requirement::Type> type,
          std::shared_ptr<location::Location> location,
          std::unique_ptr<HIR>&& target, std::vector<Pattern>&& patterns)
        : HIRBase(type, location),
          target(std::move(target)),
          patterns(std::move(patterns)),
          default_case(std::nullopt)
    {
    }
    Match(const std::shared_ptr<requirement::Type> type,
          std::shared_ptr<location::Location> location,
          std::unique_ptr<HIR>&& target, std::vector<Pattern>&& patterns,
          std::unique_ptr<HIR>&& default_case)
        : HIRBase(type, location),
          target(std::move(target)),
          patterns(std::move(patterns)),
          default_case(std::move(default_case))
    {
    }

    const HIR& get_target() const;
    const std::vector<std::pair<std::string, std::reference_wrapper<const HIR>>>
    get_patterns() const;
    std::optional<std::reference_wrapper<const HIR>> get_default_case() const;
    const std::vector<std::reference_wrapper<const HIR>> get_children()
        const override;
};

class If : public HIRBase
{
    private:
    std::unique_ptr<HIR> condition;
    std::unique_ptr<HIR> body;
    std::optional<std::unique_ptr<HIR>> else_body;

    protected:
    If(const std::shared_ptr<requirement::Type> type,
       const std::shared_ptr<location::Location> location,
       std::unique_ptr<HIR>&& condition, std::unique_ptr<HIR>&& body,
       std::unique_ptr<HIR>&& else_body);
    If(const std::shared_ptr<requirement::Type> type,
       const std::shared_ptr<location::Location> location,
       std::unique_ptr<HIR>&& condition, std::unique_ptr<HIR>&& body);

    public:
    const HIR& get_condition() const;
    const HIR& get_body() const;
    const std::optional<std::reference_wrapper<const HIR>> get_else_body()
        const;
    const std::vector<std::reference_wrapper<const HIR>> get_children()
        const override;
};

class Loop : public HIRBase
{
    private:
    const std::string counter_name;
    std::unique_ptr<HIR> condition;
    std::unique_ptr<HIR> body;

    public:
    Loop(const std::shared_ptr<requirement::Type> type,
         const std::shared_ptr<location::Location> location,
         std::unique_ptr<HIR>&& condition, std::unique_ptr<HIR>&& body)
        : HIRBase(type, location),
          condition(std::move(condition)),
          body(std::move(body))
    {
    }

    const HIR& get_condition() const;
    const HIR& get_body() const;
    const std::vector<std::reference_wrapper<const HIR>> get_children()
        const override;
};

enum class OperatorKind
{
    Addition,
    Subtraction,
    Multiplication,
    Division,
    GreaterThan,
    GreaterThanOrEqual,
    LessThan,
    LessThanOrEqual,
    Equal,
    NotEqual,
    And,
    Or
};

class BinaryExpression : public HIRBase
{
    private:
    std::pair<std::unique_ptr<HIR>, std::unique_ptr<HIR>> targets;
    const OperatorKind kind;

    public:
    BinaryExpression(
        const std::shared_ptr<requirement::Type> type,
        const std::shared_ptr<location::Location> location,
        std::pair<std::unique_ptr<HIR>, std::unique_ptr<HIR>>&& targets,
        const OperatorKind& kind);
    const OperatorKind get_kind() const;
    std::pair<std::reference_wrapper<const HIR>,
              std::reference_wrapper<const HIR>>
    get_targets() const;
    const std::vector<std::reference_wrapper<const HIR>> get_children()
        const override;
};

class SetResult : public HIRBase
{
    private:
    std::unique_ptr<HIR> expression;

    public:
    SetResult(const std::shared_ptr<requirement::Type> type,
              const std::shared_ptr<location::Location> location,
              std::unique_ptr<HIR>&& expression);
    const HIR& get_expression() const;
    const std::vector<std::reference_wrapper<const HIR>> get_children()
        const override;
};

class Return : public HIRBase
{
    private:
    std::unique_ptr<HIR> expression;

    public:
    Return(const std::shared_ptr<requirement::Type> type,
           const std::shared_ptr<location::Location> location,
           std::unique_ptr<HIR>&& expression);
    const HIR& get_expression() const;
    const std::vector<std::reference_wrapper<const HIR>> get_children()
        const override;
};

class HIR : public utils::VariantWrapper<
                Root, Integer, Float, String, List, Variable, Reference,
                Dereference, Function, Assignment, FunctionCall, AccessElement,
                AccessList, Construction, UnionConstruction, Block, Match, If,
                Loop, BinaryExpression, Return, SetResult>
{
    private:
    using parent_type =
        utils::VariantWrapper<Root, Integer, Float, String, List, Variable,
                              Reference, Dereference, Function, Assignment,
                              FunctionCall, AccessElement, AccessList,
                              Construction, UnionConstruction, Block, Match, If,
                              Loop, BinaryExpression, Return, SetResult>;

    protected:
    template <class T>
    HIR(T&& initial_content) : parent_type(std::move(initial_content))
    {
    }

    public:
    template <typename HIRType, typename... Args>
    static std::unique_ptr<HIR> create(Args&&... args)
    {
        static_assert(std::is_move_constructible<HIRType>(),
                      "Content of HIR must be move-constructible.");
        static_assert(std::is_base_of<HIRBase, HIRType>(),
                      "Content of HIR must inherit `HIRBase`.");
        std::unique_ptr<CreateHelper<HIR>> hir =
            std::make_unique<CreateHelper<HIR>>(
                std::move(CreateHelper<HIRType>(std::forward<Args>(args)...)));
        return std::move(hir);
    }

    std::unique_ptr<HIR> clone(requirement::TypeEnvironment&,
                               requirement::ClonedTypeMap) const;

    std::string to_string() const;

    const std::shared_ptr<requirement::Type> get_type() const
    {
        return std::visit([](const auto& hir) { return hir.get_type(); },
                          content);
    }

    const std::shared_ptr<requirement::Type> get_type_without_solving() const
    {
        return std::visit(
            [](const auto& hir) { return hir.get_type_without_solving(); },
            content);
    }

    const HIRBase& get_basic_info() const
    {
        return std::visit([](const auto& hir) -> const HIRBase& { return hir; },
                          content);
    }

    template <typename Functor>
    void walk(Functor functor) const
    {
        functor(*this);
        auto children = std::visit(
            [](const auto& unboxed_hir) { return unboxed_hir.get_children(); },
            content);
        for (auto child : children)
        {
            functor(child.get());
            child.get().walk(functor);
        }
        return;
    }
};

}  // namespace clawn::hir

/*
AST->HIR->solve type
HIR->LLVM IR
*/