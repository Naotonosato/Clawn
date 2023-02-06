#include <optional>
#include <unordered_map>
#include <algorithm>
#include "include/requirement/type.hpp"
#include "include/hir/hir.hpp"
#include "include/hir/visitor.hpp"
#include "include/compiler/error.hpp"
#include "include/compiler/module/module.hpp"
#include "include/compiler/pipeline.hpp"
#include "include/hir/visitor.hpp"
#include "include/resource/resource.hpp"
#include "include/utils/pattern_match.hpp"
#include "include/utils/transform.hpp"
#include "include/compiler/builtins.hpp"
#include "include/compiler/error.hpp"

namespace clawn::compiler
{
/*
This is how clawn compiler monomorphizes generics.

for this code,

==========================
function original(x,y)
{
    a = x
    b = y
    return a + b
}

f = original
result1 = f(1,0.5)
result2 = f("aaa","bbb")
==========================

compilier will

1. collect specialization info when HIR was built from AST and change signature.
    function original(x,y) to function original(int|string x,float|string y)
2. copy relationship between types (a is equal to x,b is equal to y)
3. generate helper function that speialized with passed arguments.

    original_int_int(int x,float y) -> float {a=x b=y return a+b}
    original_string_string(string x,string y) -> string {a=x b=y return a+b}

4. edit original function and calling.

    original_int_int(int x,float y) -> float {a=x b=y return a+b}

    original_string_string(string x,string y) -> string {a=x b=y return a+b}

    function original({0:int,1:string} x, {0:float,1:string} y)->float|string
    {
        if (x.'index' == 'int' and y.'index' == 'float')
        {
            return original_int_int(x as int,y as float)
        }
        if (x.'index' == 'string' and y.'index' == 'string')
        {
            return original_string_string(x as string,y as string)
        }
    }
    f = original
    result1 = f(1,0.5).get<float>()
    result2 = f("aaa","bbb").get<string>()

5. Optimize functions. (This step is not yet implemented)
    Functions at step 4 contains essencially useless processes(e.g. if
expressions, variants, etc) which is needed by the type system. Compiler should
optimize it.

These processes enable compiler to treat generic functions as first class
objects.

*/

std::vector<std::shared_ptr<requirement::Type>> replace_arguments_types(
    const hir::Function& function, const requirement::FunctionType& type,
    requirement::TypeEnvironment& type_environment)
{
    auto type_solver = type_environment.get_solver();
    auto arguments_types = type.get_argument_types();
    std::map<std::shared_ptr<requirement::Type>,
             std::vector<std::shared_ptr<requirement::Type>>>
        argument_candidates_map;
    auto candidates = type_environment.get_instantiations(function.get_type());
    // if(candidates.size() <= 1){return {};}
    requirement::TypeSet result_type_candidates_no_duplicates;

    if(candidates.size() > 0 && candidates[0].size() == 0) //if no argument function
    {
        auto got_result_type = type_environment.get_return_type(
            function.get_type(),
            {});
        if(got_result_type.has_value())
        {
            type_solver->bind(
                type.get_return_type(),
                got_result_type.value(),
                function.get_location()
            );
        }
        return {};
    }

    for (auto& candidate : candidates)
    {
        if (type.get_argument_names().size() != candidate.size())
        {
            throw Error::invalid_arguments_number(
                function.get_location(), type.get_argument_names().size(),
                candidate.size());
        }
        if (arguments_types.size() == 0)
        {
            break;
        }
        auto result_type = type_environment.get_return_type(
            function.get_type(),
            candidate  // utils::get_transformed(candidate,[type_solver](const
                       // auto& t){return type_solver->solve(t);})
        );

        if (result_type.has_value())
        {
            result_type_candidates_no_duplicates.insert(result_type.value());
        }
        else
        {
        }
        for (size_t i = 0; i < arguments_types.size(); i += 1)
        {
            argument_candidates_map[arguments_types[i]].push_back(candidate[i]);
        }
    }

    for (auto& argument_candidates_pair : argument_candidates_map)
    {
        auto argument_type = argument_candidates_pair.first;
        auto candidates = argument_candidates_pair.second;
        int index = 0;
        std::map<std::string, std::shared_ptr<requirement::Type>> tags;
        requirement::TypeSet candidates_no_duplicates;
        for (auto& candidate : candidates)
        {
            candidates_no_duplicates.insert(candidate->get_solved());
        }

        for (auto& candidate_ : candidates_no_duplicates.get_data())
        {
            // std::cout << function.get_name() << " takes(tag) " <<
            // std::to_string(index)<< candidate_->get_solved()->to_string() <<
            // std::endl;
            tags.insert(std::make_pair(std::to_string(index), candidate_));
            index += 1;
        }

        size_t argument_index =
            std::distance(arguments_types.begin(),
                          std::find(arguments_types.begin(),
                                    arguments_types.end(), argument_type));
        auto argument_type_to_replace =
            requirement::Type::create<requirement::UnionType>(
                type_solver,
                "@argument_" + std::to_string(argument_index) +
                    "_of_function_" + type.get_name(),
                tags, type_solver, true);
        type_solver->bind(argument_type, argument_type_to_replace,
                          function.get_location(), false);
        // std::cout << function.get_name() << " takes " <<
        // argument_type_to_replace->to_string() << std::endl;
    }
    // auto replaced_argument_types = type.get_argument_types();
    std::map<std::string, std::shared_ptr<requirement::Type>> result_type_tags;
    unsigned int index = 0;

    for (auto& result_type : result_type_candidates_no_duplicates.get_data())
    {
        result_type_tags.insert(
            std::make_pair(std::to_string(index), result_type));
        index += 1;
    }
    auto result_type_to_replace =
        requirement::Type::create<requirement::UnionType>(
            type_solver, "@result_of_function_" + type.get_name(),
            result_type_tags, type_solver, true);
    type_solver->bind(type.get_return_type(), result_type_to_replace,
                      function.get_location());

    return {};  // replaced_argument_types;
}

std::unique_ptr<hir::HIR> create_root_function(
    const hir::Function& function,
    const requirement::FunctionType& function_type,
    requirement::TypeEnvironment& type_environment,
    const requirement::SearchableByTypeVector<
        std::shared_ptr<requirement::Type>>&
        specialized_functions)  ////This function is complex and shold be split.
{
    auto location = function.get_location();
    auto argument_types = function_type.get_argument_types();
    auto argument_pattern =
        type_environment.get_instantiations(function.get_type());

    size_t argument_index = 0;

    std::vector<std::unique_ptr<hir::HIR>> root_function_body;

    if(argument_pattern.size() > 0 && argument_pattern[0].size() == 0) //if no argument function
    {
        auto function_type_to_call =
            specialized_functions.at({});
        auto function_to_call = hir::HIR::create<hir::Variable>(
            function_type_to_call, function_type_to_call->get_name(),
            function.get_location());
        auto return_type =
            type_environment
                .get_return_type(function.get_type(), {})
                .value();
        auto returning = hir::HIR::create<hir::Return>(
            return_type, function.get_location(),
            hir::HIR::create<hir::FunctionCall>(
                return_type, function.get_location(),
                std::move(function_to_call), std::vector<std::unique_ptr<hir::HIR>>()
            )
        );
        root_function_body.push_back(std::move(returning));
        auto root_function = hir::HIR::create<hir::Function>(
        function.get_type(), location, function.get_name(),
        hir::HIR::create<hir::Block>(function_type.get_return_type(), location,
                                     std::move(root_function_body)));
        return root_function;
    }

    for (auto& passed_argument_types : argument_pattern)
    {
        std::optional<std::unique_ptr<hir::HIR>> last_matching;
        std::vector<std::unique_ptr<hir::HIR>> unboxed_arguments;
        std::vector<unsigned int> matching_tag_indexes;
        std::vector<std::unique_ptr<hir::HIR>> tag_indexes;
        for (size_t i = 0; i < argument_types.size(); i += 1)
        {
            auto argument_type = argument_types[i];
            auto argument_name = function_type.get_argument_names()[i];
            auto argument = hir::HIR::create<hir::Variable>(
                argument_type, argument_name, location);
            tag_indexes.push_back(hir::HIR::create<hir::AccessElement>(
                type_environment.get_integer_type(), location,
                std::move(argument), ""));
        }

        for (size_t i = 0; i < argument_types.size(); i += 1)
        {
            auto argument_type = argument_types[i];
            auto argument_name = function_type.get_argument_names()[i];
            auto passed_argument_type = passed_argument_types[i]->get_solved();
            if (passed_argument_type->is_type<requirement::UnionType>() &&
                passed_argument_type->as<requirement::UnionType>()
                    ._is_by_compiler())
            {
                continue;
            }

            if (!argument_type->is_type<requirement::UnionType>())
            {
                continue;
            }
            auto& union_argument_type =
                argument_type->as<requirement::UnionType>();
            auto tags = union_argument_type.get_solved_tags();
            std::string matching_tag = "0";
            bool handled = false;
            for (auto& tag : tags)
            {
                if (tag.second->is_same_as(*passed_argument_type) && (!handled))
                {
                    matching_tag = tag.first;
                    if (tag.first != "")
                    {
                        handled = true;
                    }
                    // break;
                }
            }
            matching_tag_indexes.push_back(
                union_argument_type.get_index(matching_tag));

            auto argument = hir::HIR::create<hir::Variable>(
                argument_type, argument_name, location);
            auto argument_again = hir::HIR::create<hir::Variable>(
                argument_type, argument_name, location);
            auto unboxed = hir::HIR::create<hir::AccessElement>(
                passed_argument_type, location, std::move(argument_again),
                matching_tag);
            unboxed_arguments.push_back(std::move(unboxed));
        }

        auto function_type_to_call =
            specialized_functions.at(passed_argument_types);
        auto function_to_call = hir::HIR::create<hir::Variable>(
            function_type_to_call, function_type_to_call->get_name(),
            function.get_location());

        auto return_type =
            type_environment
                .get_return_type(function.get_type(), passed_argument_types)
                .value();
        auto returning = hir::HIR::create<hir::Return>(
            return_type, function.get_location(),
            hir::HIR::create<hir::FunctionCall>(
                return_type, function.get_location(),
                std::move(function_to_call), std::move(unboxed_arguments)));

        std::optional<std::unique_ptr<hir::HIR>> condition;
        for (size_t i = 0; i < argument_types.size(); i += 1)
        {
            auto passed_argument_type = passed_argument_types[i]->get_solved();
            if (passed_argument_type->is_type<requirement::UnionType>() &&
                passed_argument_type->as<requirement::UnionType>()
                    ._is_by_compiler())
            {
                continue;
            }

            auto tag_checking = hir::HIR::create<hir::BinaryExpression>(
                type_environment.get_boolean_type(), location,
                std::make_pair(std::move(tag_indexes[i]),
                               hir::HIR::create<hir::Integer>(
                                   type_environment.get_integer_type(),
                                   location, matching_tag_indexes[i])),
                hir::OperatorKind::Equal);

            if (!condition.has_value())
            {
                condition = std::move(tag_checking);
            }
            else
            {
                condition = hir::HIR::create<hir::BinaryExpression>(
                    type_environment.get_boolean_type(), location,
                    std::make_pair(std::move(condition.value()),
                                   std::move(tag_checking)),
                    hir::OperatorKind::And);
            }
        }

        if (condition.has_value())
        {
            auto conditional_calling = hir::HIR::create<hir::If>(
                returning->get_type(), location, std::move(condition.value()),
                std::move(returning));

            root_function_body.push_back(std::move(conditional_calling));
        }

        argument_index += 1;
    }

    auto root_function = hir::HIR::create<hir::Function>(
        function.get_type(), location, function.get_name(),
        hir::HIR::create<hir::Block>(function_type.get_return_type(), location,
                                     std::move(root_function_body)));
    return root_function;
}

void type_calling(const hir::FunctionCall& calling,
                  requirement::TypeEnvironment& type_environment);

std::vector<std::unique_ptr<hir::HIR>> monomorphize(
    const hir::Function& function,
    requirement::TypeEnvironment& type_environment,
    std::unordered_map<hir::HIRID, requirement::SearchableByTypeVector<
                                       std::shared_ptr<requirement::Type>>>&
        specializing_patterns)
{
    std::vector<std::unique_ptr<hir::HIR>> specialized_functions;
    auto type_solver = type_environment.get_solver();
    auto& function_type = function.get_type()->as<requirement::FunctionType>();
    auto argument_types_patterns =
        type_environment.get_instantiations(function.get_type());
    size_t pattern_index = 0;

    /*size_t last_argument_types_patterns_number = 0;

    while (last_argument_types_patterns_number <
    type_environment.get_instantiations(function.get_type()).size())
    {
        last_argument_types_patterns_number =
    type_environment.get_instantiations(function.get_type()).size() auto
    argument_types_patterns
    =type_environment.get_instantiations(function.get_type());
    }
    */

    for (auto& argument_types : argument_types_patterns)
    {
        if (specializing_patterns.count(function.get_id()) &&
            specializing_patterns[function.get_id()].contains(argument_types))
        {
            continue;
        }

        requirement::ClonedTypeMap cloned_type_map;
        // auto copied_function =
        //    hir::Cloner(type_environment, cloned_type_map).visit(function);
        //{
        auto cloned_function_body = function.get_body().accept(
            hir::Cloner(type_environment, cloned_type_map)
        );

        std::string suffix = "_" + std::to_string(utils::get_unique_number());

        std::vector<std::shared_ptr<requirement::Type>>
            cloned_function_arguments_types;

        for (auto& name : function_type.get_argument_names())
        {
            cloned_function_arguments_types.push_back(
                requirement::Type::create<requirement::UnsolvedType>(
                    type_solver, name));
        }

        auto cloned_function_type =
            requirement::Type::create<requirement::FunctionType>(
                type_solver, function_type.get_name() + suffix,
                cloned_function_arguments_types,
                function_type.get_argument_names(),
                cloned_function_body->get_type(),
                function_type.is_polymorphic());

        auto copied_function = hir::HIR::create<hir::Function>(
            function.get_type()->clone(cloned_type_map,
                                       function_type.get_name() + suffix),
            // cloned_function_type,
            function.get_location(), function.get_name() + suffix,
            std::move(cloned_function_body));
        //}

        type_solver->remap(cloned_type_map, function.get_location());
        auto& copied_function_type =
            copied_function->get_type()->as<requirement::FunctionType>();

        auto argument_types_of_copied_function =
            copied_function_type.get_argument_types();
        type_solver->bind(
            copied_function->as<hir::Function>().get_body().get_type(),
            copied_function_type.get_return_type(), function.get_location());

        std::map<std::string, std::shared_ptr<requirement::Type>>
            arguments_type_name_map;
        for (size_t i = 0; i < argument_types_of_copied_function.size(); i += 1)
        {
            arguments_type_name_map[copied_function_type
                                        .get_argument_names()[i]] =
                argument_types_of_copied_function[i];
        }
        for (size_t i = 0; i < argument_types.size(); i += 1)
        {
            auto argument_type = argument_types[i];
            auto argument_type_of_copied_function =
                argument_types_of_copied_function
                    [i];  // get_argument_types()[i];
            type_solver->bind(argument_type_of_copied_function, argument_type,
                              function.get_location());
        }

        // resolve type of arguments;
        copied_function->walk([&type_solver, &function,
                               &arguments_type_name_map,
                               &type_environment](const hir::HIR& hir) {
            utils::Match{hir.get_variant()}(
                utils::Type<hir::Variable>(),
                [&hir, &type_solver, &function, &arguments_type_name_map,
                 &type_environment](const hir::Variable& variable) {
                    auto variable_name = variable.get_name();
                    if (arguments_type_name_map.count(variable_name))
                    {
                        type_solver->bind(
                            variable.get_type(),
                            arguments_type_name_map[variable_name],
                            function.get_location());
                        arguments_type_name_map.erase(variable_name);
                        // remap arguments types.
                    }
                    if (hierarchy::Hierarchy::get_root() ==
                        hierarchy::demangle(variable_name).second)
                    // when it's global variable
                    {
                        type_solver->bind(
                            variable.get_type(),
                            type_environment.get_type_by_identifier(
                                variable_name),
                            function.get_location());  // global variables are
                                                       // independent of what
                                                       // arguments function
                                                       // takes, so they need to
                                                       // be resolved.
                    }
                },
                utils::Default(), [](const auto&) {});
        });

        // register instaciations of calling in copied function
        copied_function->walk([&type_solver, &function, &type_environment,
                               &arguments_type_name_map](const hir::HIR& hir) {
            utils::Match{hir.get_variant()}(
                utils::Type<hir::FunctionCall>(),
                [&hir, &type_environment, &type_solver,
                 &function](const hir::FunctionCall& calling) {
                    auto argument_types = utils::get_transformed(
                        calling.get_arguments(), [](const auto& argument) {
                            return argument.get().get_type();
                        });
                    type_environment.register_instanciation(
                        calling.get_function_obj().get_type(), argument_types);
                },
                utils::Default(), [](const auto&) {});
        });

        type_environment.register_return_type(
            function.get_type(), argument_types,
            copied_function_type.get_return_type());

        auto specialized_function_type = copied_function->get_type();

        specialized_functions.push_back(std::move(copied_function));
        /*   auto helper_function_type =
               constraint::Type::create<constraint::FunctionType>(
                   type_solver, function.get_type()->get_name() +
           suffix, argument_types,
           copied_function_body->get_type());

           auto helper_function = hir::HIR::create<hir::Function>(
               helper_function_type, function.get_location(),
               std::move(copied_function_body));
               */
        specializing_patterns[function.get_id()].insert(
            std::make_pair(argument_types, specialized_function_type));

        pattern_index += 1;
    }
    // replace_arguments_types(function, function_type, type_environment);

    return specialized_functions;
}

void type_calling(const hir::FunctionCall& calling,
                  requirement::TypeEnvironment& type_environment)
{
    hir::Printer printer;
    utils::Match{calling.get_function_obj().get_type()->get_variant()}(
        utils::Type<requirement::FunctionType>(),
        [&calling,
         &type_environment](const requirement::FunctionType& function_type) {
            auto argument_types = utils::get_transformed(
                calling.get_arguments(),
                [](const std::reference_wrapper<const clawn::hir::HIR>&
                       argument) { return argument.get().get_type(); });

            type_environment.register_instanciation(
                calling.get_function_obj().get_type(), argument_types);
            if (!function_type
                     .is_polymorphic())  // ||
                                         // !function_type.get_return_type()->is_type<requirement::UnsolvedType>())
            {
                type_environment.get_solver()->bind(
                    calling.get_type(), function_type.get_return_type(),
                    calling.get_location());
                return;
            }

            auto return_type = type_environment.get_return_type(
                calling.get_function_obj().get_type(), argument_types);

            if (return_type.has_value())
            {
                type_environment.get_solver()->bind(calling.get_type(),
                                                    return_type.value(),
                                                    calling.get_location());
            }
            else
            {
            }
        },
        utils::Default(), [](const auto&) {});
}

void type_dereferencing(const hir::Dereference& dereferencing,
                        requirement::TypeEnvironment& type_environment)
{
    hir::Printer printer;
    utils::Match{dereferencing.get_target().get_type()->get_variant()}(
        utils::Type<requirement::ReferenceType>(),
        [&dereferencing,
         &type_environment](const requirement::ReferenceType& reference_type) {
            auto type_solver = type_environment.get_solver();
            type_solver->bind(dereferencing.get_type(),
                              reference_type.get_reference_to(),
                              dereferencing.get_location());
        },
        utils::Default(), [](const auto&) {});
}

void type_element(const hir::AccessElement& accessing,
                  requirement::TypeEnvironment& type_environment)
{
    utils::Match{accessing.get_left_hand_side().get_type()->get_variant()}(
        utils::Type<requirement::StructureType>(),
        [&accessing,
         &type_environment](const requirement::StructureType& structure_type) {
            auto type_solver = type_environment.get_solver();
            auto member_types = structure_type.get_member_types();
            auto element_name = accessing.get_element_name();
            if (structure_type.has_member_named(element_name))
            {
                type_solver->bind(accessing.get_type(),
                                  structure_type.get_element_type(element_name),
                                  accessing.get_location());
            }

            else
            {
                // error
            }
        },
        utils::Type<requirement::UnionType>(),
        [&accessing,
         &type_environment](const requirement::UnionType& union_type) {
            auto type_solver = type_environment.get_solver();
            auto tags = union_type.get_solved_tags();
            auto element_name = accessing.get_element_name();
            if (tags.count(element_name))
            {
                type_solver->bind(accessing.get_type(), tags.at(element_name),
                                  accessing.get_location());
            }
            else
            {
            }  // error
        },
        utils::Default(), [](const auto&) {});
}

void type_element(const hir::AccessList& accessing,
                  requirement::TypeEnvironment& type_environment)
{
    utils::Match{accessing.get_list().get_type()->get_variant()}(
        utils::Type<requirement::StructureType>(),
        [&accessing,
         &type_environment](const requirement::StructureType& list_type) {
            auto type_solver = type_environment.get_solver();
            if (list_type.get_name() == builtins::STRUCTURE_LIST_NAME)
            {
                auto element_type = list_type.get_element_type("pointer");
                type_solver->bind(accessing.get_type(),
                                  element_type->as<requirement::ReferenceType>()
                                      .get_reference_to(),
                                  accessing.get_location());
            }
        },
        utils::Default(), [](const auto&) {});
}

void type_element(const hir::BinaryExpression& addition,
                  requirement::TypeEnvironment& type_environment)
{
    utils::Match{addition.get_targets().first.get().get_type()->get_variant()}(
        utils::Type<requirement::StructureType>(),
        [&addition,
         &type_environment](const requirement::StructureType& list_type) {
            auto type_solver = type_environment.get_solver();
            if (list_type.get_name() == builtins::STRUCTURE_LIST_NAME && addition.get_kind() == hir::OperatorKind::Addition)
            {
                auto registered_element_type = type_environment.get_element_type(addition.get_targets().first.get().get_type());
                if(registered_element_type.has_value())
                {
                    type_solver->bind(
                        registered_element_type.value(),
                        addition.get_targets().second.get().get_type(),
                        addition.get_location()
                    );
                }
                auto element_type = list_type.get_element_type("pointer");
                type_solver->bind(
                    element_type->as<requirement::ReferenceType>().get_reference_to(),
                    addition.get_targets().second.get().get_type(),
                    addition.get_location()
                );
            }
        },
        utils::Default(), [](const auto&) {});
}

namespace pipeline
{
std::vector<std::unique_ptr<hir::HIR>> monomorphizer(
    hir::HIR& hir,
    std::shared_ptr<requirement::TypeEnvironment> type_environment)
{
    std::vector<std::unique_ptr<hir::HIR>> specialized_functions;
    std::unordered_map<hir::HIRID, requirement::SearchableByTypeVector<
                                       std::shared_ptr<requirement::Type>>>
        specializing_patterns;

    auto monomorphizer = [&type_environment, &specializing_patterns,
                          &specialized_functions](const hir::HIR& child) {
        utils::Match{child.get_variant()}(
            utils::Type<hir::Function>(),
            [&child, &type_environment, &specializing_patterns,
             &specialized_functions](const hir::Function& function) {
                if (function.get_type()
                        ->as<requirement::FunctionType>()
                        .is_polymorphic())
                {
                    auto generated = monomorphize(function, *type_environment,
                                                  specializing_patterns);
                    specialized_functions.insert(
                        specialized_functions.end(),
                        std::make_move_iterator(generated.begin()),
                        std::make_move_iterator(generated.end()));
                }
            },

            utils::Default(), [](const auto&) {});
    };
    auto calling_typer = [&type_environment](const hir::HIR& child) {
        utils::Match{child.get_variant()}(
            utils::Type<hir::FunctionCall>(),
            [&child, &type_environment](const hir::FunctionCall& calling) {
                type_calling(calling, *type_environment);
            },

            utils::Default(), [](const auto&) {});
    };

    auto dereferencing_typer = [&type_environment](const hir::HIR& child) {
        utils::Match{child.get_variant()}(
            utils::Type<hir::Dereference>(),
            [&child, &type_environment](const hir::Dereference& dereferencing) {
                type_dereferencing(dereferencing, *type_environment);
            },

            utils::Default(), [](const auto&) {});
    };

    auto element_typer = [&type_environment](const hir::HIR& child) {
        utils::Match{child.get_variant()}(
            utils::Type<hir::AccessElement>(),
            [&child, &type_environment](const hir::AccessElement& accessing) {
                type_element(accessing, *type_environment);
            },
            utils::Type<hir::AccessList>(),
            [&child, &type_environment](const hir::AccessList& accessing) {
                type_element(accessing, *type_environment);
            },
            utils::Type<hir::BinaryExpression>(),
            [&child, &type_environment](const hir::BinaryExpression& addition) {
                type_element(addition, *type_environment);
            },
            utils::Default(), [](const auto&) {});
    };

    unsigned int unsolved_expressions;

    auto unsolved_expression_counter = [&unsolved_expressions,
                                        &type_environment](
                                           const hir::HIR& child) {
        utils::Match{child.get_variant()}(
            utils::Default(), [&child, &type_environment,
                               &unsolved_expressions](const auto& variant) {
                std::visit(
                    [&unsolved_expressions](const auto& hir_) {
                        if (hir_.get_type()
                                ->template is_type<requirement::UnsolvedType>())
                            unsolved_expressions += 1;
                    },
                    variant);
            });
    };

    auto resolve = [&calling_typer, &dereferencing_typer, &element_typer,
                    &unsolved_expressions,
                    &unsolved_expression_counter](const hir::HIR& target) {
        target.walk(unsolved_expression_counter);
        unsigned int previous_unsolved_expressions = unsolved_expressions;
        while (true)
        {
            target.walk(element_typer);
            target.walk(calling_typer);
            target.walk(dereferencing_typer);
            unsolved_expressions = 0;
            target.walk(unsolved_expression_counter);
            // std::cout << "unsolved expressions: " << unsolved_expressions <<
            // std::endl;
            if (unsolved_expressions >= previous_unsolved_expressions)
            {
                break;
            }
            previous_unsolved_expressions = unsolved_expressions;
        }
        unsolved_expressions = 0;
    };

    auto function_typer = [&type_environment,
                           &specializing_patterns](const hir::HIR& child) {
        utils::Match{child.get_variant()}(
            utils::Type<hir::Function>(),
            [&child, &type_environment,
             &specializing_patterns](const hir::Function& function) {
                if (function.get_type()
                        ->as<requirement::FunctionType>()
                        .is_polymorphic())
                {
                    replace_arguments_types(
                        function,
                        function.get_type()->as<requirement::FunctionType>(),
                        *type_environment);
                }
            },
            utils::Default(), [](const auto&) {});
    };

    std::unordered_map<hir::HIRID, std::unique_ptr<hir::HIR>> fixed_functions;

    auto function_fixer = [&type_environment, &specializing_patterns,
                           &fixed_functions](const hir::HIR& child) {
        utils::Match{child.get_variant()}(
            utils::Type<hir::Function>(),
            [&child, &type_environment, &specializing_patterns,
             &fixed_functions](const hir::Function& function) {
                auto id = function.get_id();
                if (!fixed_functions.count(id) &&
                    function.get_type()
                        ->as<requirement::FunctionType>()
                        .is_polymorphic())
                {
                    fixed_functions.insert(std::make_pair(
                        id, create_root_function(
                                function,
                                function.get_type()
                                    ->as<requirement::FunctionType>(),
                                *type_environment, specializing_patterns[id])));
                }
            },
            utils::Default(), [](const auto&) {});
    };

    resolve(hir);
    hir.walk(monomorphizer);

    size_t previous_specialized_function_number = 0;
    while (previous_specialized_function_number < specialized_functions.size())
    {
        previous_specialized_function_number = specialized_functions.size();

        resolve(hir);
        hir.walk(monomorphizer);
        for (auto& specialized_function : specialized_functions)
        {
            resolve(*specialized_function);
            specialized_function->walk(calling_typer);
        }

        resolve(hir);
        hir.walk(monomorphizer);
    }

    resolve(hir);
    hir.walk(function_typer);
    resolve(hir);
    hir.walk(function_fixer);

    for (auto& fixed_function : fixed_functions)
    {
        resolve(*fixed_function.second);
    }

    if (hir.is_type<hir::Root>())
    {
        for (auto& hir_ : hir.as_mutable<hir::Root>().get_hirs())
        {
            auto id = hir_->get_basic_info().get_id();
            if (fixed_functions.count(id))
            {
                hir_.swap(fixed_functions[id]);
            }
            if (hir_->is_type<hir::Block>())
            {
                auto& mutable_hirs =
                    hir_->as_mutable<hir::Block>().get_mutable_list();
                for (auto& hir_in_block : mutable_hirs)
                {
                    id = hir_in_block->get_basic_info().get_id();
                    if (fixed_functions.count(id))
                    {
                        hir_in_block.swap(fixed_functions[id]);
                    }
                }
            }
        }
    }

    return specialized_functions;
}
}  // namespace pipeline

}  // namespace clawn::compiler