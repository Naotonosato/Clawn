#include <variant>
#include <type_traits>
#include <algorithm>
#include "include/ast/ast.hpp"
#include "include/ast/visitor.hpp"
#include "include/hierarchy/hierarchical_collector.hpp"
#include "include/ast/builder.hpp"
#include "include/location/location.hpp"
#include "include/hierarchy/mangler.hpp"
#include "include/utils/pattern_match.hpp"
#include "include/utils/transform.hpp"

namespace clawn::ast
{
Builder::Builder(std::string filename, unsigned int various_tags_limit,
                 unsigned int current_line_number)
    : filename(filename),
      various_tags_limit(various_tags_limit),
      current_line_number(current_line_number),
      current_token_index(0),
      current_token(""),
      various_expressions_count(0),
      ast_container(hierarchy::HierarchicalCollector<NodeExistsState>(
          hierarchy::Hierarchy::get_root_name())),
      current_scope_id(ast_container.get_hierarchy_id())
{
    builtins = create_builtins();
}

std::shared_ptr<location::Location> Builder::create_current_debug_info()
{
    auto token_info = location::TokenInfo(current_line_number,
                                          current_token_index, current_token);
    return std::make_shared<location::Location>(filename, token_info,
                                                current_scope_id);
}

void Builder::count_line_number() { current_line_number += 1; }

void Builder::count_token_index(int index) { current_token_index += index; }

void Builder::set_token(std::string token) { current_token = token; }

void Builder::into_named_scope(std::string name)
{
    ast_container.into_named_hierarchy(name);
}

void Builder::into_anonymous_scope()
{
    ast_container.into_anonymous_hierarchy();
}

void Builder::break_out_of_scope() { ast_container.break_out_of_namespace(); }

void Builder::register_function_declaration(std::string name)
{
    bool is_already_apperared = ast_container.get(name).element.has_value();
    if (is_already_apperared)
    {
        throw std::runtime_error(
            name + " is already defined in " +
            ast_container.get(name).hierarchy_id.to_string());
    }
    auto mangled_name = hierarchy::mangle(name, current_scope_id);

    auto declaration = AST::create<GenericFunctionDeclaration>(
        create_current_debug_info(), mangled_name);
    auto tmp_function = AST::create<VariableDefinition>(
        create_current_debug_info(), mangled_name, std::move(declaration));
    ast_container.add(name, NO_DEFINITION);
    declarations.insert(std::make_pair(mangled_name, std::move(tmp_function)));
}

void Builder::register_method_declaration(std::string name,
                                          const std::string& belong_to)
{
    auto scope = hierarchy::Hierarchy::create(hierarchy::Hierarchy::get_root(),
                                              belong_to);
    bool is_already_apperared =
        ast_container.get(name, scope).element.has_value();
    if (is_already_apperared)
    {
        throw std::runtime_error(
            name + " is already defined in " +
            ast_container.get(name, scope).hierarchy_id.to_string());
    }
    auto mangled_name = hierarchy::mangle(name, scope);

    auto declaration = AST::create<GenericFunctionDeclaration>(
        create_current_debug_info(), mangled_name);
    auto tmp_function = AST::create<VariableDefinition>(
        create_current_debug_info(), mangled_name, std::move(declaration));
    ast_container.add(name, NO_DEFINITION, scope);
    declarations.insert(std::make_pair(mangled_name, std::move(tmp_function)));
}

void Builder::register_structure_declaration(std::string name)
{
    auto constructor_name = "constructor@" + name;
    bool is_already_apperared =
        ast_container.get(constructor_name).element.has_value();
    if (is_already_apperared)
    {
        throw std::runtime_error(
            name + " is already defined in " +
            ast_container.get(constructor_name).hierarchy_id.to_string());
    }
    auto mangled_name = hierarchy::mangle(name, current_scope_id);
    auto mangled_constructor_name =
        hierarchy::mangle(constructor_name, current_scope_id);

    auto declaration = AST::create<GenericStructureDeclaration>(
        create_current_debug_info(), mangled_name);

    ast_container.add(constructor_name, NO_DEFINITION);
    declarations.insert(
        std::make_pair(mangled_constructor_name, std::move(declaration)));
}

void Builder::register_union_constructor_declaration(UnionConstructorInfo info)
{
    bool is_already_registered =
        ast_container.get(info.name).element.has_value();
    if (is_already_registered)
    {
        return;
    }
    auto mangled_name = hierarchy::mangle(info.name, current_scope_id);
    auto mangled_union_name =
        hierarchy::mangle(info.union_name, current_scope_id);

    auto declaration = AST::create<UnionConstructorDeclaration>(
        create_current_debug_info(), mangled_union_name, info.tag_name,
        mangled_name);

    ast_container.add(info.name, NO_DEFINITION);
    declarations.insert(std::make_pair(mangled_name, std::move(declaration)));
}

std::unique_ptr<AST> Builder::create_assignment_or_variable_definition(
    std::string name, std::unique_ptr<AST>&& right_node)
{
    if (ast_container.get(name).element.has_value())
    {
        auto left_hand_side = create_named_node(name);
        return AST::create<Assignment>(create_current_debug_info(),
                                       std::move(left_hand_side),
                                       std::move(right_node));
    }
    auto variable = AST::create<VariableDefinition>(
        create_current_debug_info(), hierarchy::mangle(name, current_scope_id),
        std::move(right_node));
    ast_container.add(name, EXISTS);
    return variable;
}

std::unique_ptr<AST> Builder::create_integer(int num)
{
    return AST::create<Integer>(create_current_debug_info(), num);
}

std::unique_ptr<AST> Builder::create_float(double num)
{
    return AST::create<Float>(create_current_debug_info(), num);
}

std::unique_ptr<AST> Builder::create_minus(std::unique_ptr<AST>&& expr)
{
    auto zero = AST::create<Integer>(create_current_debug_info(), 0);
    return create_binary_expression(std::move(zero), std::move(expr),
                                    OperatorKind::Subtraction);
}

std::unique_ptr<AST> Builder::create_string(std::string str)
{
    return AST::create<String>(create_current_debug_info(), str);
}

std::unique_ptr<AST> Builder::create_void() { return 0; }

std::unique_ptr<AST> Builder::create_set_result(
    std::unique_ptr<AST>&& expression)
{
    return AST::create<SetResult>(create_current_debug_info(),
                                  std::move(expression));
}

std::unique_ptr<AST> Builder::create_reference(std::unique_ptr<AST>&& refer_to)
{
    return AST::create<Reference>(create_current_debug_info(),
                                  std::move(refer_to));
}

std::unique_ptr<AST> Builder::create_dereference(std::unique_ptr<AST>&& target)
{
    return AST::create<Dereference>(create_current_debug_info(),
                                    std::move(target));
}

std::unique_ptr<AST> Builder::create_various(std::unique_ptr<AST>&& expression)
{
    auto constructor = create_union_constructor(ast::UnionConstructorInfo(
        "Various", std::to_string(various_expressions_count)));
    various_expressions_count += 1;
    if (various_expressions_count > various_tags_limit)
    {
        throw std::runtime_error("Too many 'various' expressions.");
    }
    std::vector<std::unique_ptr<AST>> arguments;
    arguments.push_back(std::move(expression));
    return create_call(std::move(constructor), std::move(arguments));
}

std::unique_ptr<AST> Builder::create_list(
    std::vector<std::unique_ptr<AST>>&& elements)
{
    return AST::create<List>(create_current_debug_info(), std::move(elements));
}

std::unique_ptr<AST> Builder::create_list()
{
    return AST::create<List>(create_current_debug_info());
}

std::unique_ptr<AST> Builder::create_binary_expression(
    std::unique_ptr<AST>&& node, std::unique_ptr<AST>&& other,
    const OperatorKind& operator_kind)
{
    return AST::create<BinaryExpression>(create_current_debug_info(),
                                         operator_kind, std::move(node),
                                         std::move(other));
}

std::unique_ptr<AST> Builder::create_member_variable_definition(
    std::string name, std::unique_ptr<AST>&& node)
{
    auto variable = AST::create<Variable>(
        create_current_debug_info(), hierarchy::mangle(name, current_scope_id));
    ast_container.add(name, EXISTS);
    return variable;
}

std::unique_ptr<AST> Builder::create_global_variable_definition(
    std::string name, std::unique_ptr<AST>&& node)
{
    auto variable = AST::create<GlobalVariable>(
        create_current_debug_info(), hierarchy::mangle(name, current_scope_id),
        std::move(node));
    ast_container.add(name, EXISTS, hierarchy::Hierarchy::get_root());
    return variable;
}

std::string Builder::register_argument(std::string name)
{
    auto mangled_name = hierarchy::mangle(name, current_scope_id);
    ast_container.add(name, NO_DEFINITION);
    return mangled_name;
}

std::unique_ptr<AST> Builder::create_assignment(std::unique_ptr<AST>&& left,
                                                std::unique_ptr<AST>&& right,
                                                bool is_both_reference)
{
    return AST::create<Assignment>(create_current_debug_info(), std::move(left),
                                   std::move(right), is_both_reference);
}

std::unique_ptr<AST> Builder::create_deep_copy(std::unique_ptr<AST>&& pointer,
                                               std::unique_ptr<AST>&& object)
{
    return AST::create<DeepCopy>(create_current_debug_info(),
                                 std::move(pointer), std::move(object));
}

std::unique_ptr<AST> Builder::create_block(
    std::vector<std::unique_ptr<AST>>&& expressions)
{
    return AST::create<Block>(create_current_debug_info(),
                              std::move(expressions));
}

std::unique_ptr<AST> Builder::create_return(std::unique_ptr<AST>&& expressions)
{
    return AST::create<Return>(create_current_debug_info(),
                               std::move(expressions));
}

std::unique_ptr<AST> Builder::create_call(
    std::unique_ptr<AST>&& function,
    std::vector<std::unique_ptr<AST>>&& arguments)
{
    return AST::create<FunctionCall>(create_current_debug_info(),
                                     std::move(function), std::move(arguments));
}

std::unique_ptr<AST> Builder::create_call(
    const std::string& function_name,
    std::vector<std::unique_ptr<AST>>&& arguments)
{
    auto mangled_name = hierarchy::mangle(function_name, current_scope_id);
    auto location = create_current_debug_info();
    auto function = AST::create<Variable>(location, mangled_name);

    auto calling = AST::create<FunctionCall>(location, std::move(function),
                                             std::move(arguments));

    unsolved_functions.insert(
        std::make_pair(calling->get_basic_info().get_id(), mangled_name));

    return calling;
}

std::unique_ptr<AST> Builder::create_common_access(
    std::unique_ptr<AST> expression, const std::string& element_name)
{
    auto temporary_variable_name = std::to_string(
        utils::get_unique_number());  // number is not permitted in the syntax
                                      // so using number.
    auto temporary_variable = create_assignment_or_variable_definition(
        temporary_variable_name, std::move(expression));
    std::vector<std::pair<std::string, std::unique_ptr<AST>>> patterns;
    for (unsigned int index = 0; index < various_tags_limit; index += 1)
    {
        auto tag_name = std::to_string(index);
        auto unboxed = create_access(
            create_access(create_named_node(temporary_variable_name), tag_name),
            element_name);
        patterns.push_back(std::make_pair(tag_name, std::move(unboxed)));
    }
    return create_match(std::move(temporary_variable), std::move(patterns));
}

std::unique_ptr<AST> Builder::create_common_call(
    std::unique_ptr<AST> expression,
    std::vector<std::unique_ptr<AST>>&& arguments)
{
    auto temporary_variable_name = std::to_string(
        utils::get_unique_number());  // number is not permitted in the syntax
                                      // so using number.
    auto temporary_variable = create_assignment_or_variable_definition(
        temporary_variable_name, std::move(expression));
    std::vector<std::pair<std::string, std::unique_ptr<AST>>>
        temporary_arguments_variables;
    for (auto& argument : arguments)
    {
        auto name = std::to_string(utils::get_unique_number());
        temporary_arguments_variables.push_back(
            std::make_pair(name, create_assignment_or_variable_definition(
                                     name, std::move(argument))));
    }

    std::vector<std::pair<std::string, std::unique_ptr<AST>>> patterns;
    for (unsigned int index = 0; index < various_tags_limit; index += 1)
    {
        auto tag_name = std::to_string(index);
        auto unboxed =
            create_access(create_named_node(temporary_variable_name), tag_name);
        auto arguments_variables = utils::get_transformed(
            temporary_arguments_variables, [this](const auto& argument) {
                return create_named_node(argument.first);
            });
        auto calling =
            create_call(std::move(unboxed), std::move(arguments_variables));
        patterns.push_back(std::make_pair(tag_name, std::move(calling)));
    }
    auto matching =
        create_match(std::move(temporary_variable), std::move(patterns));
    std::vector<std::unique_ptr<AST>> all_expressions;
    for (auto&& argument : temporary_arguments_variables)
    {
        all_expressions.push_back(std::move(argument.second));
    }
    all_expressions.push_back(create_set_result(std::move(matching)));
    return create_block(std::move(all_expressions));
}

std::unique_ptr<AST> Builder::create_union_constructor(
    UnionConstructorInfo union_constructor_info)
{
    return create_named_node(union_constructor_info.name);
}

std::unique_ptr<AST> Builder::create_construction(
    std::unique_ptr<AST>&& constructor,
    std::vector<std::pair<std::string, std::unique_ptr<AST>>>&& arguments)
{
    return AST::create<Construction>(create_current_debug_info(),
                                     std::move(constructor),
                                     std::move(arguments));
}

std::unique_ptr<AST> Builder::create_construction(
    const std::string& constructor_name,
    std::vector<std::pair<std::string, std::unique_ptr<AST>>>&& named_arguments)
{
    auto mangled_name =
        hierarchy::mangle("constructor@" + constructor_name, current_scope_id);

    auto location = create_current_debug_info();
    auto constructor = AST::create<Variable>(location, mangled_name);
    auto calling = AST::create<Construction>(location, std::move(constructor),
                                             std::move(named_arguments));

    unsolved_functions.insert(
        std::make_pair(calling->get_basic_info().get_id(), mangled_name));

    return calling;  // AST::create<Integer>(location,0);
}

std::unique_ptr<AST> Builder::create_match(std::unique_ptr<AST>&& target,
                                           std::vector<Pattern>&& patterns)
{
    return AST::create<Match>(create_current_debug_info(), std::move(target),
                              std::move(patterns));
}

std::unique_ptr<AST> Builder::create_match(std::unique_ptr<AST>&& target,
                                           std::vector<Pattern>&& patterns,
                                           std::unique_ptr<AST>&& default_case)
{
    return AST::create<Match>(create_current_debug_info(), std::move(target),
                              std::move(patterns), std::move(default_case));
}

std::unique_ptr<AST> Builder::create_if(std::unique_ptr<AST>&& cond,
                                        std::unique_ptr<AST>&& if_body,
                                        std::unique_ptr<AST>&& else_body)
{
    return AST::create<If>(create_current_debug_info(), std::move(cond),
                           std::move(if_body), std::move(else_body));
}

std::unique_ptr<AST> Builder::create_repeat(const std::string& counter_name,
                                            std::unique_ptr<AST>&& counter,
                                            std::unique_ptr<AST>&& condition,
                                            std::unique_ptr<AST>&& body)
{
    auto mangled_counter_name =
        hierarchy::mangle(counter_name, current_scope_id);
    std::vector<std::unique_ptr<AST>> expressions;
    expressions.push_back(std::move(counter));
    expressions.push_back(create_set_result(
        AST::create<Repeat>(create_current_debug_info(), mangled_counter_name,
                            std::move(condition), std::move(body))));
    return create_block(std::move(expressions));
}

std::unique_ptr<AST> Builder::create_access(std::unique_ptr<AST>&& left,
                                            std::string right)
{
    return AST::create<AccessElement>(create_current_debug_info(),
                                      std::move(left), std::move(right));
}

std::unique_ptr<AST> Builder::create_access_list(std::unique_ptr<AST>&& list,
                                                 std::unique_ptr<AST>&& index)
{
    return AST::create<AccessList>(create_current_debug_info(), std::move(list),
                                   std::move(index));
}

std::unique_ptr<AST> Builder::create_generic_function_definition(
    const std::string& name,
    std::vector<
        std::pair<std::string,
                  std::optional<std::shared_ptr<requirement::Requirement>>>>&&
        parameters,
    std::unique_ptr<AST>&& body)
{
    auto previous_scope_id = current_scope_id.get_poped_back();

    auto definition = AST::create<GenericFunction>(
        create_current_debug_info(), hierarchy::mangle(name, previous_scope_id),
        parameters, std::move(body));
    return definition;
}

std::unique_ptr<AST> Builder::create_generic_union_definition(
    const std::string& name,
    const std::vector<std::pair<
        std::string, std::optional<std::shared_ptr<requirement::Requirement>>>>&
        tags)
{
    return AST::create<GenericUnion>(create_current_debug_info(),
                                     hierarchy::mangle(name, current_scope_id),
                                     tags);
}

std::unique_ptr<AST> Builder::create_generic_structure_definition(
    const std::string& name,
    const std::vector<std::pair<
        std::string, std::optional<std::shared_ptr<requirement::Requirement>>>>&
        elements)
{
    auto previous_scope_id = current_scope_id.get_poped_back();
    return AST::create<GenericStructure>(
        create_current_debug_info(), hierarchy::mangle(name, previous_scope_id),
        elements);
}

std::unique_ptr<AST> Builder::create_generic_class_definition(
    const std::string& name, std::vector<std::string>&& arguments,
    std::vector<std::unique_ptr<AST>>&& members,
    std::vector<std::unique_ptr<AST>>&& methods)
{
    auto definition = AST::create<GenericClass>(
        create_current_debug_info(), hierarchy::mangle(name, current_scope_id),
        std::move(arguments), std::move(members), std::move(methods));

    return definition;
}

std::unique_ptr<AST> Builder::create_C_type_declaration(
    const std::string& name,
    const std::vector<std::pair<std::string,
                                std::vector<std::string>>>& member_types)
{
    return AST::create<CTypeDeclaration>(create_current_debug_info(), name,
                                         member_types);
}

std::unique_ptr<AST> Builder::create_C_function_declaration(
    const std::string& name,
    const std::vector<std::vector<std::string>>& argument_types,
    const std::vector<std::string>& return_type)
{
    auto declaration = AST::create<CFunctionDeclaration>(
        create_current_debug_info(), name, argument_types, return_type);

    // C functions shouldn't be mangled but unmangled functions are not
    // callable, so returns variable that is assigned the function pointer.
    return create_assignment_or_variable_definition(name,
                                                    std::move(declaration));
}

bool Builder::exist_named_expression(std::string name)
{
    return (ast_container.get(name).element != std::nullopt);
}

std::unique_ptr<AST> Builder::create_named_node(std::string name)
{
    auto named_element = ast_container.get(name);
    auto node = named_element.element;
    auto scope_id = named_element.hierarchy_id;

    if (!node.has_value())
    {
        auto constructor_name = "constructor@" + name;
        named_element = ast_container.get(constructor_name);
        node = named_element.element;
        scope_id = named_element.hierarchy_id;
        if (node.has_value())
        {
            return AST::create<Variable>(
                create_current_debug_info(),
                hierarchy::mangle(constructor_name, scope_id));
        }
        // print error message
        throw std::runtime_error(name + " was not found. in " +
                                 current_scope_id.to_string());
    }
    return AST::create<Variable>(create_current_debug_info(),
                                 hierarchy::mangle(name, scope_id));
}

std::unique_ptr<AST> Builder::resolve_name(const FunctionCall& calling)
{
    auto& function = calling.get_function();
    return utils::Match{function.get_variant()}(
        utils::Type<Variable>(),
        // all of functions definitions are definition of variable.
        // function f()... means f = (function object). f is variable.
        [this](const Variable& function_obj) {
            auto mangled_name =
                hierarchy::Hierarchy::create(function_obj.get_name());
            auto demangled_name = mangled_name.back();
            auto found = ast_container.get(demangled_name, mangled_name);
            if (found.element.has_value())  // && found.element.value() ==
                                            // NodeExistsState::EXISTS)
            {
                auto found_function_name =

                    hierarchy::Hierarchy::create(
                        hierarchy::HierarchyPool::get(found.hierarchy_id),
                        demangled_name)
                        .to_string();
                return AST::create<Variable>(function_obj.get_location(),
                                             found_function_name);
            }
            else
            {
                throw std::runtime_error("function " + demangled_name +
                                         " was not found");
                return std::unique_ptr<AST>();
            }
        },
        utils::Default(),
        [](const auto& var) {
            throw std::runtime_error("calling something not function");
            return std::unique_ptr<AST>();
        });
}

void Builder::resolve_names(std::vector<std::unique_ptr<AST>>& programs)
{
    std::vector<std::reference_wrapper<const AST>> flatten;
    for (auto&& program : programs)
    {
        auto flat = Flattener().visit(*program);
        flatten.insert(flatten.end(), flat.begin(), flat.end());
    }

    for (int idx = 0; idx < flatten.size(); idx += 1)
    {
        auto& node = flatten[idx].get();
        auto id = node.get_basic_info().get_id();
        if (unsolved_functions.count(id))
        {
            // Bad code but it cannot be helped because rebuilding AST every
            // unsolved function call costs too much.
            auto& mutable_node = const_cast<AST&>(node);
            auto& calling =
                std::get<FunctionCall>(mutable_node.get_mutable_variant());
            auto new_function = resolve_name(calling);
            calling.set_function(std::move(new_function));
        }
    }
}

std::unique_ptr<AST> Builder::create_builtins()
{
    std::vector<std::pair<std::string, std::optional<std::shared_ptr<
                                           clawn::requirement::Requirement>>>>
        tags;
    for (unsigned int index = 0; index < various_tags_limit; index += 1)
    {
        auto tag_name = std::to_string(index);
        register_union_constructor_declaration(
            UnionConstructorInfo("Various", tag_name));
        tags.push_back(std::make_pair(tag_name, std::nullopt));
    }

    return create_generic_union_definition("Various", tags);
}

void Builder::create_root(std::vector<std::unique_ptr<AST>>&& programs)
{
    std::vector<std::unique_ptr<AST>> all_programs;
    for (auto&& pair : declarations)
    {
        all_programs.push_back(std::move(pair.second));
    }

    resolve_names(programs);
    for (auto&& program : programs)
    {
        all_programs.push_back(std::move(program));
    }
    if (builtins.has_value())
    {
        all_programs.push_back(std::move(builtins.value()));
        builtins = std::nullopt;
    }
    root =
        AST::create<Root>(create_current_debug_info(), std::move(all_programs));
}

std::shared_ptr<AST> Builder::build() const { return root; }

}  // namespace clawn::ast