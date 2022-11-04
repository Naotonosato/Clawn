#pragma once
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "include/hierarchy/hierarchical_collector.hpp"
#include "include/location/location.hpp"
#include "include/requirement/requirement.hpp"

namespace clawn::ast
{
struct UnionConstructorInfo;

using NodeID = unsigned long;
class Parameters;
class NodeBase;
class Integer;
class Float;
class List;
class String;
class Variable;
class GlobalVariable;
class Argument;
class Assignment;
class BinaryExpression;
class FunctionCall;
class AccessElement;
class DeepCopy;
class Block;
class If;
class Repeat;
class GenericFunctionDeclaration;
class GenericFunction;
class GenericClass;
class AST;
class Root;
using Pattern = std::pair<std::string, std::unique_ptr<AST>>;
enum class OperatorKind;

}  // namespace clawn::ast

namespace clawn::ast
{
enum NodeExistsState
{
    EXISTS,
    NOT_EXISTS,
    NO_DEFINITION
};

class Builder
{
    private:
    const std::string filename;
    const unsigned int various_tags_limit;
    unsigned long current_line_number;
    unsigned long current_token_index;
    std::string current_token;
    unsigned int various_expressions_count;
    hierarchy::HierarchicalCollector<NodeExistsState> ast_container;
    hierarchy::HierarchyID& current_scope_id;
    std::map<NodeID, std::string> unsolved_functions;
    std::map<std::string, std::unique_ptr<AST>> declarations;
    std::optional<std::unique_ptr<AST>> builtins;
    std::shared_ptr<AST> root;

    std::unique_ptr<AST> resolve_name(const FunctionCall& calling);
    void resolve_names(std::vector<std::unique_ptr<AST>>& programs);
    std::unique_ptr<AST> create_builtins();

    public:
    Builder(std::string filename, unsigned int various_tags_limit,
            unsigned int current_line_number);

    void count_line_number();

    void count_token_index(int index);

    void set_token(std::string token);

    std::shared_ptr<location::Location> create_current_debug_info();

    void into_named_scope(std::string name);

    void into_anonymous_scope();

    void break_out_of_scope();

    void register_function_declaration(std::string name);

    void register_method_declaration(std::string name,
                                     const std::string& belong_to);

    void register_structure_declaration(std::string name);

    void register_union_constructor_declaration(UnionConstructorInfo);

    void register_class_declaration(std::string name,
                                    std::vector<std::string> argument_names);

    std::unique_ptr<AST> create_integer(int num);

    std::unique_ptr<AST> create_float(double num);

    std::unique_ptr<AST> create_minus(std::unique_ptr<AST>&& expr);

    std::unique_ptr<AST> create_string(std::string str);

    std::unique_ptr<AST> create_void();

    std::unique_ptr<AST> create_set_result(std::unique_ptr<AST>&& expression);

    std::unique_ptr<AST> create_reference(std::unique_ptr<AST>&& refer_to);

    std::unique_ptr<AST> create_dereference(std::unique_ptr<AST>&& target);

    std::unique_ptr<AST> create_various(std::unique_ptr<AST>&& epression);

    std::unique_ptr<AST> create_list(
        std::vector<std::unique_ptr<AST>>&& elements);

    std::unique_ptr<AST> create_list();

    std::unique_ptr<AST> create_binary_expression(
        std::unique_ptr<AST>&& node, std::unique_ptr<AST>&& other,
        const OperatorKind& operator_kind);

    std::unique_ptr<AST> create_assignment_or_variable_definition(
        std::string name, std::unique_ptr<AST>&& node);

    std::unique_ptr<AST> create_member_variable_definition(
        std::string name, std::unique_ptr<AST>&& node);

    std::unique_ptr<AST> create_global_variable_definition(
        std::string name, std::unique_ptr<AST>&&);

    std::string register_argument(std::string name);

    std::unique_ptr<AST> create_assignment(std::unique_ptr<AST>&& left,
                                           std::unique_ptr<AST>&& right,
                                           bool is_both_reference = false);

    std::unique_ptr<AST> create_deep_copy(std::unique_ptr<AST>&& pointer,
                                          std::unique_ptr<AST>&& object);

    std::unique_ptr<AST> create_return(std::unique_ptr<AST>&& expressions);

    std::unique_ptr<AST> create_C_type_declaration(
        const std::string& name,
        const std::vector<
            std::pair<const std::string, std::vector<const std::string>>>&
            member_types);

    std::unique_ptr<AST> create_C_function_declaration(
        const std::string& name,
        const std::vector<std::vector<const std::string>>& argument_types,
        const std::vector<const std::string>& return_type);

    std::unique_ptr<AST> create_block(
        std::vector<std::unique_ptr<AST>>&& expressions);

    std::unique_ptr<AST> create_call(
        std::unique_ptr<AST>&&, std::vector<std::unique_ptr<AST>>&& arguments);

    std::unique_ptr<AST> create_call(
        const std::string& name, std::vector<std::unique_ptr<AST>>&& arguments);

    std::unique_ptr<AST> create_common_access(std::unique_ptr<AST> expression,
                                              const std::string& element_name);

    std::unique_ptr<AST> create_common_call(
        std::unique_ptr<AST> expression,
        std::vector<std::unique_ptr<AST>>&& arguments);

    std::unique_ptr<AST> create_union_constructor(UnionConstructorInfo);

    std::unique_ptr<AST> create_construction(
        std::unique_ptr<AST>&&,
        std::vector<std::pair<std::string, std::unique_ptr<AST>>>&& arguments);

    std::unique_ptr<AST> create_construction(
        const std::string& name,
        std::vector<std::pair<std::string, std::unique_ptr<AST>>>&& arguments);

    std::unique_ptr<AST> create_match(std::unique_ptr<AST>&& target,
                                      std::vector<Pattern>&& patterns);
    std::unique_ptr<AST> create_match(std::unique_ptr<AST>&& target,
                                      std::vector<Pattern>&& patterns,
                                      std::unique_ptr<AST>&& default_case);

    std::unique_ptr<AST> create_if(std::unique_ptr<AST>&& cond,
                                   std::unique_ptr<AST>&& if_body,
                                   std::unique_ptr<AST>&& else_body);

    std::unique_ptr<AST> create_repeat(const std::string& counter_name,
                                       std::unique_ptr<AST>&& counter,
                                       std::unique_ptr<AST>&& condition,
                                       std::unique_ptr<AST>&& body);

    std::unique_ptr<AST> create_access(std::unique_ptr<AST>&& left,
                                       std::string right);

    std::unique_ptr<AST> create_access_list(std::unique_ptr<AST>&& list,
                                            std::unique_ptr<AST>&& index);

    std::unique_ptr<AST> create_generic_function_definition(
        const std::string& name,
        std::vector<std::pair<std::string, std::optional<std::shared_ptr<
                                               requirement::Requirement>>>>&&
            parameters,
        std::unique_ptr<AST>&& body);

    std::unique_ptr<AST> create_generic_union_definition(
        const std::string& name,
        const std::vector<std::pair<
            std::string,
            std::optional<std::shared_ptr<requirement::Requirement>>>>& tags);

    std::unique_ptr<AST> create_generic_structure_definition(
        const std::string& name,
        const std::vector<std::pair<
            std::string,
            std::optional<std::shared_ptr<requirement::Requirement>>>>&
            elements);

    std::unique_ptr<AST> create_generic_class_definition(
        const std::string& name, std::vector<std::string>&& arguments,
        std::vector<std::unique_ptr<AST>>&& members_definition,
        std::vector<std::unique_ptr<AST>>&& methods);

    bool exist_named_expression(std::string name);

    std::unique_ptr<AST> create_named_node(std::string name);

    void create_root(std::vector<std::unique_ptr<AST>>&& program);

    std::shared_ptr<AST> build() const;
};

}  // namespace clawn::ast