#pragma once
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include <optional>
#include "include/utils/variant_wrapper.hpp"
#include <iostream>
#include "include/utils/unique_number.hpp"
#include "include/location/location.hpp"
#include "include/requirement/requirement.hpp"

namespace clawn::ast
{
class AST;
using NodeID = unsigned long;

class Parameters
    : public std::vector<
          std::pair<std::string,
                    std::optional<std::shared_ptr<requirement::Requirement>>>>
{
    public:
    Parameters(
        const std::vector<std::pair<
            std::string,
            std::optional<std::shared_ptr<requirement::Requirement>>>>& data)
        : std::vector<std::pair<
              std::string,
              std::optional<std::shared_ptr<requirement::Requirement>>>>(data)
    {
    }
};

using CTypeDescriptor = std::vector<const std::string>;

class NodeBase
{
    private:
    NodeID id;
    std::shared_ptr<location::Location> location;

    public:
    NodeBase(std::shared_ptr<location::Location> location)
        : id(utils::get_unique_number()), location(location)
    {
    }
    NodeBase() = delete;
    NodeBase(const NodeBase&) = delete;
    NodeBase(NodeBase&&) = default;

    public:
    template <typename... Args>
    void* operator new(size_t, Args&&...)
    {
        // this code forces programmer to initialize class that inherits from
        // NodeBase with AST::create. Please don't mind.
        constexpr bool operator_new_is_not_used = [] {
            return false;
        }();  // delay evaluation until instantiation
        static_assert(operator_new_is_not_used,
                      "Class that inherits from 'NodeBase' cannot be "
                      "initialized with operator new.");
    }
    const unsigned long get_id() const;
    const std::shared_ptr<location::Location> get_location() const;
};

class Block;

class Root : public NodeBase
{
    private:
    std::vector<std::unique_ptr<AST>> children;

    public:
    Root(std::shared_ptr<location::Location> location,
         std::vector<std::unique_ptr<AST>>&& children)
        : NodeBase(location), children(std::move(children))
    {
    }

    const std::vector<std::reference_wrapper<const AST>> get_children() const;
};

class Integer : public NodeBase
{
    private:
    int64_t initial_value;

    public:
    Integer(std::shared_ptr<location::Location> location, int64_t initial_value)
        : NodeBase(location), initial_value(initial_value)
    {
    }
    const int64_t get_initial_value() const;
};

class Float : public NodeBase
{
    private:
    double initial_value;

    public:
    Float(std::shared_ptr<location::Location> location, double initial_value)
        : NodeBase(location), initial_value(initial_value)
    {
    }
    const double get_initial_value() const;
};

class List : public NodeBase
{
    private:
    std::vector<std::unique_ptr<AST>> initial_values;

    public:
    List(std::shared_ptr<location::Location> location) : NodeBase(location) {}
    List(std::shared_ptr<location::Location> location,
         std::vector<std::unique_ptr<AST>> initial_values)
        : NodeBase(location), initial_values(std::move(initial_values))
    {
    }

    const std::vector<std::reference_wrapper<const AST>> get_initial_values()
        const;
};

class String : public NodeBase
{
    private:
    std::string initial_value;

    public:
    String(std::shared_ptr<location::Location> location,
           const std::string initial_value)
        : NodeBase(location), initial_value(initial_value)
    {
    }
    const std::string& get_initial_value() const;
};

class Reference : public NodeBase
{
    private:
    std::unique_ptr<AST> refer_to;

    public:
    Reference(std::shared_ptr<location::Location> location,
              std::unique_ptr<AST>&& refer_to)
        : NodeBase(location), refer_to(std::move(refer_to))
    {
    }
    const AST& get_refer_to() const;
};

class Dereference : public NodeBase
{
    private:
    std::unique_ptr<AST> target;

    public:
    Dereference(std::shared_ptr<location::Location> location,
                std::unique_ptr<AST>&& target)
        : NodeBase(location), target(std::move(target))
    {
    }
    const AST& get_target() const;
};

class SetResult : public NodeBase
{
    std::unique_ptr<AST> expression;

    public:
    SetResult(std::shared_ptr<location::Location> location,
              std::unique_ptr<AST>&& expression)
        : NodeBase(location), expression(std::move(expression))
    {
    }
    const AST& get_expression() const;
};

class Variable : public NodeBase
{
    private:
    std::string name;

    public:
    Variable(std::shared_ptr<location::Location> location, std::string name)
        : NodeBase(location), name(name)
    {
    }
    const std::string& get_name() const;
};

class VariableDefinition : public NodeBase
{
    private:
    std::string name;
    std::unique_ptr<AST> initial_value;

    public:
    VariableDefinition(std::shared_ptr<location::Location> location,
                       std::string name, std::unique_ptr<AST>&& initial_value)
        : NodeBase(location),
          name(name),
          initial_value(std::move(initial_value))
    {
    }

    const std::string& get_name() const;
    const AST& get_initial_value() const;
};

class GlobalVariable : public NodeBase
{
    private:
    std::string name;
    std::unique_ptr<AST> initial_value;

    public:
    GlobalVariable(std::shared_ptr<location::Location> location,
                   std::string name, std::unique_ptr<AST>&& initial_value)
        : NodeBase(location),
          name(name),
          initial_value(std::move(initial_value))
    {
    }

    const std::string get_name() const;
    const AST& get_initial_value() const;
};

class Assignment : public NodeBase
{
    private:
    std::unique_ptr<AST> left_hand_side;
    std::unique_ptr<AST> right_hand_side;
    bool _is_both_reference;

    public:
    Assignment(std::shared_ptr<location::Location> location,
               std::unique_ptr<AST>&& left_hand_side,
               std::unique_ptr<AST>&& right_hand_side,
               bool _is_both_reference = false)
        : NodeBase(location),
          left_hand_side(std::move(left_hand_side)),
          right_hand_side(std::move(right_hand_side)),
          _is_both_reference(_is_both_reference)
    {
    }

    const AST& get_left_hand_side() const;
    const AST& get_right_hand_side() const;
    bool is_both_reference() const;
};

class Operator
{
    private:
    const std::string name;

    public:
    Operator(const std::string& name) : name(name) {}
    std::string get_name() const;
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

class BinaryExpression : public NodeBase
{
    private:
    std::unique_ptr<AST> left_hand_side;
    std::unique_ptr<AST> right_hand_side;
    const OperatorKind operator_kind;

    public:
    BinaryExpression(std::shared_ptr<location::Location> location,
                     const OperatorKind& operator_kind,
                     std::unique_ptr<AST>&& left_hand_side,
                     std::unique_ptr<AST>&& right_hand_side)
        : NodeBase(location),
          left_hand_side(std::move(left_hand_side)),
          right_hand_side(std::move(right_hand_side)),
          operator_kind(operator_kind)
    {
    }

    const OperatorKind get_operator() const;
    const AST& get_left_hand_side() const;
    const AST& get_right_hand_side() const;
};

class GenericFunctionDeclaration;

class FunctionCall : public NodeBase
{
    private:
    std::unique_ptr<AST> function;
    std::vector<std::unique_ptr<AST>> old_functions;
    std::vector<std::unique_ptr<AST>> arguments;

    public:
    FunctionCall(std::shared_ptr<location::Location> location,
                 std::unique_ptr<AST>&& function,
                 std::vector<std::unique_ptr<AST>>&& arguments)
        : NodeBase(location),
          function(std::move(function)),
          arguments(std::move(arguments))
    {
    }

    const AST& get_function() const;
    void set_function(std::unique_ptr<AST>&& new_function);
    const std::vector<std::reference_wrapper<const AST>> get_arguments() const;
};

class AccessElement : public NodeBase
{
    private:
    std::unique_ptr<AST> left_hand_side;
    std::string element_name;

    public:
    AccessElement(std::shared_ptr<location::Location> location,
                  std::unique_ptr<AST>&& left_hand_side,
                  std::string element_name)
        : NodeBase(location),
          left_hand_side(std::move(left_hand_side)),
          element_name(element_name)
    {
    }

    const AST& get_left_hand_side() const;
    const std::string& get_element_name() const;
};

class AccessList : public NodeBase
{
    private:
    std::unique_ptr<AST> list;
    std::unique_ptr<AST> index;

    public:
    AccessList(std::shared_ptr<location::Location> location,
               std::unique_ptr<AST>&& list, std::unique_ptr<AST>&& index)
        : NodeBase(location), list(std::move(list)), index(std::move(index))
    {
    }

    const AST& get_list() const;
    const AST& get_index() const;
};

class DeepCopy : public NodeBase
{
    private:
    std::unique_ptr<AST> left_hand_side;
    std::unique_ptr<AST> right_hand_side;

    public:
    DeepCopy(std::shared_ptr<location::Location> location,
             std::unique_ptr<AST>&& left_hand_side,
             std::unique_ptr<AST>&& right_hand_side)
        : NodeBase(location),
          left_hand_side(std::move(left_hand_side)),
          right_hand_side(std::move(right_hand_side))
    {
    }

    const AST& get_left_hand_side() const;
    const AST& get_right_hand_side() const;
};

class Block : public NodeBase
{
    private:
    std::vector<std::unique_ptr<AST>> expressions;

    public:
    Block(std::shared_ptr<location::Location> location,
          std::vector<std::unique_ptr<AST>>&& expressions)
        : NodeBase(location), expressions(std::move(expressions))
    {
    }

    const std::vector<std::reference_wrapper<const AST>> get_expressions()
        const;
};

class Match : public NodeBase
{
    public:
    using Pattern = std::pair<std::string, std::unique_ptr<AST>>;

    private:
    std::unique_ptr<AST> target;
    std::vector<Pattern> patterns;
    std::optional<std::unique_ptr<AST>> default_case;

    public:
    Match(std::shared_ptr<location::Location> location,
          std::unique_ptr<AST>&& target, std::vector<Pattern>&& patterns)
        : NodeBase(location),
          target(std::move(target)),
          patterns(std::move(patterns)),
          default_case(std::nullopt)
    {
    }
    Match(std::shared_ptr<location::Location> location,
          std::unique_ptr<AST>&& target, std::vector<Pattern>&& patterns,
          std::unique_ptr<AST>&& default_case)
        : NodeBase(location),
          target(std::move(target)),
          patterns(std::move(patterns)),
          default_case(std::move(default_case))
    {
    }

    const AST& get_target() const;
    const std::vector<std::pair<std::string, std::reference_wrapper<const AST>>>
    get_patterns() const;
    std::optional<std::reference_wrapper<const AST>> get_default_case() const;
};

class If : public NodeBase
{
    private:
    std::unique_ptr<AST> condition;
    std::unique_ptr<AST> body;
    std::optional<std::unique_ptr<AST>> else_body;

    public:
    If(std::shared_ptr<location::Location> location,
       std::unique_ptr<AST>&& condition, std::unique_ptr<AST>&& body)
        : NodeBase(location),
          condition(std::move(condition)),
          body(std::move(body)),
          else_body(std::nullopt)
    {
    }
    If(std::shared_ptr<location::Location> location,
       std::unique_ptr<AST>&& condition, std::unique_ptr<AST>&& body,
       std::unique_ptr<AST>&& else_body)
        : NodeBase(location),
          condition(std::move(condition)),
          body(std::move(body)),
          else_body(std::move(else_body))
    {
    }

    const AST& get_condition() const;
    const AST& get_body() const;
    const std::optional<std::reference_wrapper<const AST>> get_else_body()
        const;
};

class Repeat : public NodeBase
{
    private:
    const std::string counter_name;
    std::unique_ptr<AST> condition;
    std::unique_ptr<AST> body;

    public:
    Repeat(std::shared_ptr<location::Location> location,
           const std::string& counter_name, std::unique_ptr<AST>&& condition,
           std::unique_ptr<AST>&& body)
        : NodeBase(location),
          counter_name(counter_name),
          condition(std::move(condition)),
          body(std::move(body))
    {
    }

    const std::string& get_counter_name() const;
    const AST& get_condition() const;
    const AST& get_body() const;
};

class GenericFunction;

class GenericFunctionDeclaration : public NodeBase
{
    private:
    std::string name;

    public:
    GenericFunctionDeclaration(std::shared_ptr<location::Location> location,
                               std::string name)
        : NodeBase(location), name(name)
    {
    }
    const std::string& get_name() const;
};

class GenericFunction : public NodeBase
{
    private:
    const std::string name;
    std::unique_ptr<AST> body;
    Parameters parameters;

    public:
    GenericFunction(std::shared_ptr<location::Location> location,
                    const std::string& name, const Parameters& parameters,
                    std::unique_ptr<AST>&& body)
        : NodeBase(location),
          name(name),
          body(std::move(body)),
          parameters(parameters)
    {
    }

    const std::string& get_name() const;
    const AST& get_body() const;
    const std::vector<std::string> get_parameter_names() const;
    const Parameters& get_parameters() const;
    const std::optional<std::shared_ptr<requirement::Requirement>>
    get_requirement(const std::string& parameter_name) const;
};

struct UnionConstructorInfo
{
    const std::string union_name;
    const std::string tag_name;
    const std::string name;
    UnionConstructorInfo() = default;
    UnionConstructorInfo(const std::string& union_name,
                         const std::string& tag_name)
        : union_name(union_name),
          tag_name(tag_name),
          name(union_name + "#" + tag_name)
    {
    }
};

UnionConstructorInfo extract_union_constructor_info(const std::string& text);

class UnionConstructorDeclaration : public NodeBase
{
    private:
    const std::string union_name;
    const std::string tag_name;
    const std::string constructor_name;

    public:
    UnionConstructorDeclaration(std::shared_ptr<location::Location> location,
                                const std::string& union_name,
                                const std::string& tag_name,
                                const std::string& constructor_name)
        : NodeBase(location),
          union_name(union_name),
          tag_name(tag_name),
          constructor_name(constructor_name)
    {
    }

    const std::string get_union_name() const;
    const std::string get_tag_name() const;
    const std::string get_constructor_name() const;
};

class GenericUnion : public NodeBase
{
    private:
    const std::string name;
    const Parameters tags;

    public:
    GenericUnion(std::shared_ptr<location::Location> location,
                 const std::string& name, const Parameters& tags)
        : NodeBase(location), name(name), tags(tags)
    {
    }

    const std::string get_name() const;
    const std::vector<std::string> get_tag_names() const;
    const std::optional<std::shared_ptr<requirement::Requirement>>
    get_requirement(const std::string& tag_name) const;
    const Parameters& get_tags() const;
    const std::map<std::string, std::string> get_constructor_names() const;
};

class GenericStructureDeclaration : public NodeBase
{
    private:
    const std::string name;

    public:
    GenericStructureDeclaration(std::shared_ptr<location::Location> location,
                                const std::string& name)
        : NodeBase(location), name(name)
    {
    }

    const std::string& get_name() const;
    const std::string get_constructor_name() const;
};

class GenericStructure : public NodeBase
{
    private:
    const std::string name;
    const Parameters elements;

    public:
    GenericStructure(std::shared_ptr<location::Location> location,
                     const std::string& name, const Parameters& elements)
        : NodeBase(location), name(name), elements(elements)
    {
    }

    const std::string& get_name() const;
    const std::vector<std::string> get_element_names() const;
    const std::optional<std::shared_ptr<requirement::Requirement>>
    get_requirement(const std::string& element_name) const;
    const Parameters& get_elements() const;
    const std::string get_constructor_name() const;
};

class Construction : public NodeBase
{
    private:
    std::unique_ptr<AST> constructor;
    std::vector<std::pair<std::string, std::unique_ptr<AST>>> arguments;

    public:
    Construction(
        const std::shared_ptr<location::Location> location,
        std::unique_ptr<AST>&& constructor,
        std::vector<std::pair<std::string, std::unique_ptr<AST>>>&& arguments)
        : NodeBase(location),
          constructor(std::move(constructor)),
          arguments(std::move(arguments))
    {
    }

    const AST& get_constructor() const;
    const std::vector<std::pair<std::string, std::reference_wrapper<const AST>>>
    get_arguments() const;
};

class GenericClass : public NodeBase
{
    private:
    std::string name;
    std::vector<std::string> arguments;
    std::vector<std::unique_ptr<AST>> member_variables;
    std::vector<std::unique_ptr<AST>> member_functions;

    public:
    GenericClass(std::shared_ptr<location::Location> location,
                 const std::string& name,
                 const std::vector<std::string>& arguments,
                 std::vector<std::unique_ptr<AST>>&& member_variables,
                 std::vector<std::unique_ptr<AST>>&& member_functions)
        : NodeBase(location),
          name(name),
          arguments(arguments),
          member_variables(std::move(member_variables)),
          member_functions(std::move(member_functions))
    {
    }

    const std::string& get_name() const;
    const std::vector<std::string>& get_argument_names() const;
    const std::vector<std::reference_wrapper<const AST>> get_member_variables()
        const;
    const std::vector<std::reference_wrapper<const AST>> get_member_functions()
        const;
};

class TypeID : public NodeBase
{
    private:
    std::unique_ptr<AST> expression;

    public:
    TypeID(std::shared_ptr<location::Location> location,
           std::unique_ptr<AST>&& expression)
        : NodeBase(location), expression(std::move(expression))
    {
    }

    const AST& get_expression() const;
};

class Return : public NodeBase
{
    private:
    std::unique_ptr<AST> expression;

    public:
    Return(std::shared_ptr<location::Location> location,
           std::unique_ptr<AST>&& expression)
        : NodeBase(location), expression(std::move(expression))
    {
    }

    const AST& get_expression() const;
};

class CTypeDeclaration : public NodeBase
{
    const std::string name;
    const std::vector<std::pair<const std::string, CTypeDescriptor>>
        member_types;

    public:
    CTypeDeclaration(
        std::shared_ptr<location::Location> location, const std::string& name,
        const std::vector<std::pair<const std::string, CTypeDescriptor>>&
            member_types)
        : NodeBase(location), name(name), member_types(member_types)
    {
    }

    const std::string& get_name() const;
    const std::vector<std::pair<const std::string, CTypeDescriptor>>&
    get_member_types() const;
};

class CFunctionDeclaration : public NodeBase
{
    const std::string name;
    const std::vector<CTypeDescriptor> argument_types;
    const CTypeDescriptor return_type;

    public:
    CFunctionDeclaration(std::shared_ptr<location::Location> location,
                         const std::string& name,
                         const std::vector<CTypeDescriptor>& argument_types,
                         const CTypeDescriptor& return_type)
        : NodeBase(location),
          name(name),
          argument_types(argument_types),
          return_type(return_type)
    {
    }

    const std::string& get_name() const;
    const std::vector<CTypeDescriptor>& get_argument_types() const;
    const CTypeDescriptor& get_return_type() const;
};

class AST
    : public utils::VariantWrapper<
          Root, Integer, Float, List, String, Reference, Dereference, SetResult,
          Variable, VariableDefinition, GlobalVariable, Assignment,
          BinaryExpression, FunctionCall, AccessElement, AccessList, DeepCopy,
          Block, Match, If, Repeat, GenericFunctionDeclaration, GenericFunction,
          UnionConstructorDeclaration, GenericUnion, Construction,
          GenericStructureDeclaration, GenericStructure, GenericClass, TypeID,
          Return, CTypeDeclaration, CFunctionDeclaration>
{
    private:
    using parent_type = utils::VariantWrapper<
        Root, Integer, Float, List, String, Reference, Dereference, SetResult,
        Variable, VariableDefinition, GlobalVariable, Assignment,
        BinaryExpression, FunctionCall, AccessElement, AccessList, DeepCopy,
        Block, Match, If, Repeat, GenericFunctionDeclaration, GenericFunction,
        UnionConstructorDeclaration, GenericUnion, Construction,
        GenericStructureDeclaration, GenericStructure, GenericClass, TypeID,
        Return, CTypeDeclaration, CFunctionDeclaration>;

    protected:
    template <typename T>
    AST(T&& initial_content) : parent_type(std::move(initial_content))
    {
    }

    public:
    template <typename NodeType, typename... Args>
    static std::unique_ptr<AST> create(Args&&... args)
    {
        auto node = std::make_unique<CreateHelper<AST>>(
            NodeType(std::forward<Args>(args)...));
        return node;
    }

    template <typename ContentType>
    AST& operator=(ContentType&& new_content)
    {
        content = std::move(new_content);
        return *this;
    }

    const NodeBase& get_basic_info() const
    {
        return std::visit(
            [](const auto& node) -> const NodeBase& { return node; }, content);
    }
    /*
    template <typename ContentType>
    Node& operator=(ContentType&& new_content) {
        assign(new_content);
        return *this;
    }
    */
};

template <class Visitor>
using ASTVisitor = utils::VisitorWrapper<Visitor, AST>;
}  // namespace clawn::ast