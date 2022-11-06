#include <optional>
#include <algorithm>
#include "include/hir/hir.hpp"
#include "include/hir/visitor.hpp"
#include "include/requirement/requirement.hpp"
#include "include/compiler/error.hpp"
#include "include/compiler/module/module.hpp"
#include "include/compiler/pipeline.hpp"
#include "include/resource/resource.hpp"
#include "include/hierarchy/mangler.hpp"
#include "include/utils/pattern_match.hpp"
#include "include/utils/transform.hpp"
#include "include/mir/mir.hpp"
#include "include/compiler/builtins.hpp"

namespace clawn::compiler::pipeline
{
class HIRToMIR : public hir::Visitor<std::shared_ptr<mir::Value>>
{
    private:
    std::shared_ptr<mir::Context> context;
    requirement::TypeEnvironment& type_environment;
    std::map<std::string, std::shared_ptr<mir::Value>>& variable_addresses;
    mutable mir::Builder builder;
    std::shared_ptr<mir::BasicBlock> definitions_block;
    // accessible_scopes[location] = (variable_name,element_name)

    public:
    HIRToMIR(std::shared_ptr<mir::Context> context)
        : context(context),
          type_environment(context->get_type_environment()),
          variable_addresses(context->get_variable_addresses()),
          builder(mir::Builder(context, hierarchy::Hierarchy::get_root())),
          definitions_block(builder.get_current_block())
    {
    }

    std::shared_ptr<mir::Value> visit(const hir::Root& hir) const override
    {
        auto global_function = builder.get_current_block()->get_owner();
        auto entry = mir::BasicBlock::create(builder.get_context(),
                                             global_function, "entry");
        builder.set_current_block(entry);
        auto scope = hir.get_location()->get_scope_id();
        std::vector<std::reference_wrapper<const hir::HIR>> functions;

        for (auto& child : hir.get_children())
        {
            auto mir = child.get().accept(*this);
        }
        for (auto& function : functions)
        {
            function.get().accept(*this);
        }
        builder.set_current_block(entry);
        auto zero =
            mir::MIR::create<mir::Integer, mir::Value>(context, scope, 0);

        builder.set_current_block(global_function->get_body().back());
        builder.create_return(hir.get_location()->get_scope_id(), zero);

        return zero;
    }
    std::shared_ptr<mir::Value> visit(const hir::Integer& hir) const override
    {
        auto scope = hir.get_location()->get_scope_id();
        return mir::MIR::create<mir::Integer, mir::Value>(
            context, scope, hir.get_initial_value());
    }
    std::shared_ptr<mir::Value> visit(const hir::Float& hir) const override
    {
        auto scope = hir.get_location()->get_scope_id();
        return mir::MIR::create<mir::RealNumber, mir::Value>(
            context, scope, hir.get_initial_value());
    }
    std::shared_ptr<mir::Value> visit(const hir::String& hir) const override
    {
        auto scope = hir.get_location()->get_scope_id();
        auto string_pointer = mir::MIR::create<mir::StringPointer, mir::Value>(
            context, scope, hir.get_initial_value());
        auto length = mir::MIR::create<mir::Integer, mir::Value>(
            context, scope, hir.get_initial_value().size());
        return builder.create_call(
            scope,
            builder.create_load(scope,
                                variable_addresses.at("string_constructor")),
            {string_pointer, length});
    }
    std::shared_ptr<mir::Value> visit(const hir::Reference& hir) const override
    {
        auto scope = hir.get_location()->get_scope_id();
        auto refer_to = hir.get_refer_to().accept(*this);
        if (!refer_to->is_type<mir::Load>())
        {
            throw std::runtime_error("not load");
        }
        auto address = refer_to->as_mutable<mir::Load>().get_address();
        return address;
        // auto allcoation_for_refered_address =
        // builder.create_allocate(scope,address->get_type(),"allcoation_for_refered_address");
        // builder.create_store(scope,allcoation_for_refered_address,address);
        // return builder.create_load(scope,allcoation_for_refered_address);
    }
    std::shared_ptr<mir::Value> visit(
        const hir::Dereference& hir) const override
    {
        auto scope = hir.get_location()->get_scope_id();
        auto target = hir.get_target().accept(*this);
        return builder.create_load(scope, target, "deref");
    }
    std::shared_ptr<mir::Value> visit(const hir::List& hir) const override
    {
        auto scope = hir.get_location()->get_scope_id();
        auto& list_type = hir.get_type()->as<requirement::StructureType>();
        auto element_type = list_type.get_element_type("pointer")
                                ->as<requirement::ReferenceType>()
                                .get_reference_to();
        auto list_constructor = builder.create_load(
            scope, variable_addresses.at("list_constructor"));
        auto list_entity =
            builder.create_call(scope, list_constructor,
                                {builder.create_sizeof(scope, element_type)});

        auto appender =
            builder.create_load(scope, variable_addresses.at("append"));
        for (auto& initial_value_hir : hir.get_initial_values())
        {
            auto initial_value = initial_value_hir.get().accept(*this);
            auto pointer = builder.create_allocate(
                scope, initial_value_hir.get().get_type());
            builder.create_store(scope, pointer, initial_value);
            builder.create_call(
                scope, appender,
                {list_entity,
                 builder.create_cast(
                     scope, pointer,
                     type_environment.get_integer_type()->get_pointer_to())});
        }

        auto casted = builder.create_cast(scope, list_entity,
                                          hir.get_type()->get_pointer_to());
        return builder.create_load(scope, casted);
    }
    std::shared_ptr<mir::Value> visit(const hir::Variable& hir) const override
    {
        auto scope = hir.get_location()->get_scope_id();
        auto name = hir.get_name();
        if (!hir.is_global())  // when it's not a global variable
        {
            auto current_function_name =
                builder.get_current_block()->get_owner()->get_name();
            name += ":" + current_function_name;
        }
        if (variable_addresses.count(name))
        {
            return builder.create_load(scope, variable_addresses.at(name));
        }
        else
        {
            auto allocation = hir.is_global()
                                  ? builder.create_global_allocate(
                                        scope, hir.get_type(), "addr:" + name)
                                  : builder.create_allocate(
                                        scope, hir.get_type(), "addr:" + name);
            variable_addresses.insert(std::make_pair(name, allocation));
            return builder.create_load(scope, allocation, name);
        }
    }
    std::shared_ptr<mir::Value> visit(const hir::Function& hir) const override
    {
        auto scope = hir.get_location()->get_scope_id();
        auto scope_in_function =
            hir.get_body().get_basic_info().get_location()->get_scope_id();
        auto previous_block = builder.get_current_block();
        builder.set_current_block(definitions_block);

        auto name = hir.get_name();

        std::shared_ptr<mir::Value> function_allocation;

        if (!variable_addresses.count(name))
        {
            function_allocation = builder.create_global_allocate(
                scope, hir.get_type(), "addr:" + name);
            variable_addresses.insert(
                std::make_pair(name, function_allocation));
        }
        else
        {
            function_allocation = variable_addresses.at(name);
        }

        auto function =
            builder.create_function(scope, hir.get_name(), hir.get_type());
        builder.create_store(scope, function_allocation,
                             mir::Value::from(function));
        variable_addresses.insert(
            std::make_pair(hir.get_name(), function_allocation));

        builder.set_current_block(previous_block);

        if (hir.is_declaration_only())
        {
            return mir::Value::from(function);
        }

        auto block = mir::BasicBlock::create(builder.get_context(), function,
                                             hir.get_name());
        builder.set_current_block(block);
        auto& function_type = hir.get_type()->as<requirement::FunctionType>();
        auto return_type = function_type.get_return_type();
        auto argument_types = function_type.get_argument_types();
        // auto arguments = function->get_arguments();

        int index = 0;
        for (auto& argument_name :
             function_type.get_argument_names())  // set arguments
        {
            auto argument_type = argument_types.at(index);
            auto allocation =
                builder.create_allocate(scope_in_function, argument_type,
                                        "ptr_arg_" + std::to_string(index));
            auto argument = builder.create_argument(scope, function, index);
            builder.create_store(scope_in_function, allocation, argument);
            // builder.create_load(scope_in_function, allocation);
            variable_addresses.insert(std::make_pair(
                argument_name + ":" + hir.get_name(), allocation));
            index += 1;
        }
        auto body = hir.get_body().accept(*this);

        auto body_scope =
            hir.get_body().get_basic_info().get_location()->get_scope_id();
        if (!body->get_type()->is_same_as(*return_type) &&
            return_type->is_type<requirement::UnionType>())
        {
            auto& return_type_as_union =
                return_type->as<requirement::UnionType>();
            for (auto& tag : return_type_as_union.get_solved_tags())
            {
                auto tag_name = tag.first;
                auto tag_type = tag.second;
                if (body->get_type()->is_same_as(*tag_type))
                {
                    auto return_value_allocation =
                        builder.create_allocate(body_scope, return_type);
                    auto index = return_type_as_union.get_index(tag_name);
                    auto index_pointer = builder.create_get_element_pointer(
                        body_scope, return_value_allocation, 0);
                    builder.create_store(
                        body_scope, index_pointer,
                        mir::MIR::create<mir::Integer, mir::Value>(
                            context, scope, index));
                    auto element_pointer = builder.create_get_element_pointer(
                        body_scope, return_value_allocation, index);
                    builder.create_store(body_scope, element_pointer, body);
                    builder.create_load(body_scope, return_value_allocation);
                }
            }
        }

        builder.set_current_block(previous_block);
        return mir::Value::from(function);
    }
    std::shared_ptr<mir::Value> visit(const hir::Assignment& hir) const override
    {
        auto& left_hir = hir.get_left_hand_side();
        auto& right_hir = hir.get_right_hand_side();
        auto scope = hir.get_location()->get_scope_id();
        auto left = left_hir.accept(*this);
        auto right = right_hir.accept(*this);

        if (left_hir.is_type<hir::Variable>() &&
            right_hir.is_type<hir::Block>() &&
            right_hir.as<hir::Block>().get_list().empty())
        {
            return left;
        }

        if (!left->is_type<mir::Load>())
        {
            throw std::runtime_error(hir.get_left_hand_side().to_string() +
                                     " is not assignable.");
        }
        auto address = left->as<mir::Load>().get_address();
        builder.create_store(scope, address, right);
        return builder.create_load(scope, address);
    }
    std::shared_ptr<mir::Value> visit(
        const hir::FunctionCall& hir) const override
    {
        auto scope = hir.get_location()->get_scope_id();

        if (!hir.get_function_obj()
                 .get_type()
                 ->is_type<requirement::FunctionType>())
        {
            throw std::runtime_error(
                "calling shold have right function. but " +
                hir.get_function_obj().to_string() + ":" +
                hir.get_function_obj().get_type()->to_string() +
                " is not that typed.");
        }

        auto arguments_hir = hir.get_arguments();
        std::vector<std::shared_ptr<mir::Value>> arguments;

        for (size_t i = 0; i < arguments_hir.size(); i += 1)
        {
            auto& argument_hir = arguments_hir.at(i).get();
            auto cast_to = hir.get_function_obj()
                               .get_type()
                               ->as<requirement::FunctionType>()
                               .get_argument_types()
                               .at(i);

            if (cast_to->is_type<requirement::UnionType>() &&
                cast_to->as<requirement::UnionType>()._is_by_compiler())
            {
                auto allocation = builder.create_allocate(scope, cast_to);
                bool argument_handled = false;
                for (auto& tag :
                     cast_to->as<requirement::UnionType>().get_solved_tags())
                {
                    if (tag.first == "")
                    {
                        continue;
                    }
                    if (tag.second->is_same_as(*argument_hir.get_type()))
                    {
                        auto index_of_tag =
                            cast_to->as<requirement::UnionType>().get_index(
                                tag.first);
                        auto element_pointer =
                            builder.create_get_element_pointer(
                                scope, allocation, index_of_tag);
                        builder.create_store(scope, element_pointer,
                                             argument_hir.accept(*this));
                        auto tag_pointer = builder.create_get_element_pointer(
                            scope, allocation, 0, "tag_of_arg");
                        builder.create_store(
                            scope, tag_pointer,
                            mir::MIR::create<mir::Integer, mir::Value>(
                                context, scope, index_of_tag));
                        auto actuall_argument = builder.create_load(
                            scope, allocation, "actuall_arg");
                        arguments.push_back(actuall_argument);
                        argument_handled = true;
                        break;
                    }
                }
                if (!argument_handled)
                {
                    throw std::runtime_error(
                        "invalid calling:" +
                        argument_hir.get_type()->to_string() +
                        " is passed as argument which should be typed " +
                        cast_to->to_string() + " of function " +
                        hir.get_function_obj().get_type()->to_string());
                }
            }
            else
            {
                arguments.push_back(argument_hir.accept(*this));
            }
        }

        auto calling = builder.create_call(
            scope, hir.get_function_obj().accept(*this), arguments);

        auto function_return_type = hir.get_function_obj()
                                        .get_type()
                                        ->as<requirement::FunctionType>()
                                        .get_return_type();
        if (function_return_type->is_type<requirement::UnionType>() &&
            function_return_type->as<requirement::UnionType>()
                ._is_by_compiler())  // multiple return type candidate.
        {
            auto& union_function_return_type =
                function_return_type->as<requirement::UnionType>();
            if (union_function_return_type.get_solved_tags().size() <= 1)
            {
                return calling;
            }
            for (auto& return_type_candidate_info :
                 union_function_return_type.get_solved_tags())
            {
                auto return_type_candidate_name =
                    return_type_candidate_info.first;
                if (return_type_candidate_name ==
                    "") /*only compiler can access actuall tag info*/
                {
                    continue;
                }
                auto return_type_candidate = return_type_candidate_info.second;
                auto type_solver = context->get_type_environment().get_solver();

                if (hir.get_type()->is_same_as(*return_type_candidate))
                {
                    auto index = union_function_return_type.get_index(
                        return_type_candidate_name);
                    auto temporary_allocation = builder.create_allocate(
                        scope, function_return_type, "temp_ptr_for_res");
                    builder.create_store(scope, temporary_allocation, calling);
                    auto accessing = builder.create_get_element_pointer(
                        scope, temporary_allocation, index);
                    return builder.create_load(scope, accessing);
                }
            }

            throw std::logic_error("invalid calling");
        }
        else
        {
            return calling;
        }
    }
    std::shared_ptr<mir::Value> visit(
        const hir::AccessElement& hir) const override
    {
        return utils::Match{hir.get_left_hand_side().get_type()->get_variant()}(
            utils::Type<requirement::StructureType>(),
            [this, &hir](const requirement::StructureType& type) {
                auto element_name = hir.get_element_name();
                auto index = type.get_index(element_name);
                auto scope = hir.get_location()->get_scope_id();
                auto access_from = hir.get_left_hand_side()
                                       .accept(*this)
                                       ->as<mir::Load>()
                                       .get_address();
                auto accessing = builder.create_get_element_pointer(
                    scope, access_from, index);
                return builder.create_load(
                    scope, accessing,
                    "load_" + access_from->get_name() + "." + element_name);
            },
            utils::Type<requirement::UnionType>(),
            [this, &hir](const requirement::UnionType& type) {
                auto index = type.get_index(hir.get_element_name());
                auto scope = hir.get_location()->get_scope_id();
                auto access_from = hir.get_left_hand_side()
                                       .accept(*this)
                                       ->as<mir::Load>()
                                       .get_address();
                auto mir_name =
                    index == 0 ? "ptr_of_tag"
                               : "ptr_of_elm_'" + hir.get_element_name() + "'";
                // if index is 0, it's tag info.
                auto accessing = builder.create_get_element_pointer(
                    scope, access_from, index, mir_name);
                return builder.create_load(scope, accessing,
                                           "load_" + mir_name);
            },
            utils::Default(),
            [](const auto&) {
                throw std::runtime_error("not union or structure");
                return std::shared_ptr<mir::Value>(nullptr);
            });
    }
    std::shared_ptr<mir::Value> visit(const hir::AccessList& hir) const override
    {
        auto scope = hir.get_location()->get_scope_id();
        auto list = hir.get_list().accept(*this);
        auto element_type = list->get_type()
                                ->as<requirement::StructureType>()
                                .get_element_type("pointer")
                                ->as<requirement::ReferenceType>()
                                .get_reference_to();
        auto index = hir.get_index().accept(*this);
        auto indexer = builder.create_load(scope, variable_addresses.at("get"));
        auto list_type = variable_addresses.at("list_constructor")
                             ->get_type()
                             ->as<requirement::ReferenceType>()
                             .get_reference_to()
                             ->as<requirement::FunctionType>()
                             .get_return_type();
        auto list_pointer = builder.create_cast(
            scope, list->as<mir::Load>().get_address(), list_type);

        auto element =
            builder.create_call(scope, indexer, {list_pointer, index});
        auto casted_element = builder.create_cast(
            scope, element, element_type->get_pointer_to()->get_pointer_to());
        return builder.create_load(scope,
                                   builder.create_load(scope, casted_element));
    }
    std::shared_ptr<mir::Value> visit(
        const hir::Construction& hir) const override
    {
        auto scope = hir.get_location()->get_scope_id();
        auto allocation = builder.create_allocate(scope, hir.get_type());
        auto& structure_type = hir.get_type()->as<requirement::StructureType>();

        for (auto& element : hir.get_arguments())
        {
            auto element_name = element.first;
            auto element_value = element.second.get().accept(*this);
            auto element_pointer = builder.create_get_element_pointer(
                scope, allocation, structure_type.get_index(element_name));
            builder.create_store(scope, element_pointer, element_value);
        }
        auto instance = builder.create_load(scope, allocation);
        return instance;
    }
    std::shared_ptr<mir::Value> visit(
        const hir::UnionConstruction& hir) const override
    {
        auto scope = hir.get_location()->get_scope_id();

        auto allocation = builder.create_allocate(scope, hir.get_type());

        auto& union_type = hir.get_type()->as<requirement::UnionType>();
        auto tag_name = hir.get_argument().first;
        auto tag_value = hir.get_argument().second.get().accept(*this);

        auto tag_index = union_type.get_index(tag_name);
        auto element_pointer =
            builder.create_get_element_pointer(scope, allocation, tag_index);
        builder.create_store(scope, element_pointer, tag_value);
        auto tag_pointer =
            builder.create_get_element_pointer(scope, allocation, 0);
        builder.create_store(scope, tag_pointer,
                             mir::MIR::create<mir::Integer, mir::Value>(
                                 context, scope, tag_index));
        auto instance = builder.create_load(scope, allocation);

        return instance;
    }
    std::shared_ptr<mir::Value> visit(const hir::Block& hir) const override
    {
        auto scope = hir.get_location()->get_scope_id();
        if (hir.get_list().empty() ||
            hir.get_type()->is_type<requirement::VoidType>())
        {
            for (auto& expression : hir.get_list())
            {
                expression.get().accept(*this);
            }
            return mir::MIR::create<mir::Integer, mir::Value>(context, scope,
                                                              0);
        }

        auto type = hir.get_type();
        auto result_address =
            builder.create_allocate(scope, type, "result_of_block");
        auto previous_block = builder.get_current_block();
        // auto block = mir::BasicBlock::create(
        //    builder.get_context(), previous_block->get_owner(),
        //    previous_block->get_owner()->get_name());
        // builder.set_current_block(block);
        for (auto& expression : hir.get_list())
        {
            auto value = expression.get().accept(*this);
            if (expression.get().is_type<hir::Return>())
            {
                break;
            }
            if (expression.get().is_type<hir::SetResult>())
            {
                builder.create_store(expression.get()
                                         .get_basic_info()
                                         .get_location()
                                         ->get_scope_id(),
                                     result_address, value);
            }
        }
        // builder.set_current_block(previous_block);
        return builder.create_load(scope, result_address);
    }

    std::shared_ptr<mir::Value> visit(const hir::Match& hir) const override
    {
        bool is_various_match =
            hir.get_target().get_type()->get_name() == "[GLOBAL]/Various";
        auto scope = hir.get_location()->get_scope_id();
        auto target = hir.get_target().accept(*this);
        auto& target_union_type =
            target->get_type()->as<requirement::UnionType>();
        auto tag_info = builder.create_load(
            scope, builder.create_get_element_pointer(
                       scope, target->as<mir::Load>().get_address(), 0));
        std::optional<std::shared_ptr<mir::Value>> result_address;
        if (!hir.get_type()->is_type<requirement::VoidType>())
        {
            result_address = builder.create_allocate(scope, hir.get_type());
        }

        auto previous_block = builder.get_current_block();
        auto current_function = builder.get_current_block()->get_owner();
        auto patterns_not_fixed = hir.get_patterns();
        std::vector<std::pair<std::string, std::reference_wrapper<const clawn::hir::HIR>>> patterns;

        if(is_various_match)
        {
            for (size_t i = 0; i < patterns_not_fixed.size(); i += 1)
            {
                auto pattern = patterns_not_fixed.at(i);
                auto tag_name = pattern.first;
                auto& hir_on_matched = pattern.second.get();
                if (! target_union_type
                        .get_element_type(target_union_type.get_index(tag_name))
                        ->is_type<requirement::UnsolvedType>())
                {
                    patterns.push_back(pattern);
                }
            }
        }
        else
        {
            patterns = patterns_not_fixed;
        }

        std::vector<std::shared_ptr<mir::BasicBlock>> matching_blocks;
        std::vector<std::shared_ptr<mir::BasicBlock>>
            last_blocks_of_matching_blocks;
        std::vector<std::shared_ptr<mir::BasicBlock>> merge_blocks;

        for (size_t i = 0; i < patterns.size(); i += 1)
        {
            auto pattern = patterns.at(i);
            auto tag_name = pattern.first;
            auto& hir_on_matched = pattern.second.get();
            if (target_union_type
                    .get_element_type(target_union_type.get_index(tag_name))
                    ->is_type<requirement::UnsolvedType>() &&
                is_various_match)
            {
                continue;
            }

            auto matching_block = mir::BasicBlock::create(
                context, current_function, "on_" + tag_name);
            matching_blocks.push_back(matching_block);
            builder.set_current_block(matching_block);
            auto on_matched = hir_on_matched.accept(*this);
            auto scope_on_matched =
                hir_on_matched.get_basic_info().get_location()->get_scope_id();
            // builder.set_current_block(matching_block);
            if (result_address.has_value())
            {
                builder.create_store(scope_on_matched, result_address.value(),
                                     on_matched);
            }
            last_blocks_of_matching_blocks.push_back(
                builder.get_current_block());

            auto merge_block =
                mir::BasicBlock::create(context, current_function, "merge");
            merge_blocks.push_back(merge_block);
        }

        builder.set_current_block(previous_block);
        auto tag_name = patterns.at(0).first;
        auto match_tag_index = mir::MIR::create<mir::Integer, mir::Value>(
            context, scope, target_union_type.get_index(tag_name));
        auto comparison = builder.create_compare_primitive(
            scope, tag_info, match_tag_index, "tag_cmp");
        builder.create_conditional_jump(scope, matching_blocks.at(0),
                                        merge_blocks.at(0), comparison);

        for (size_t i = 0; i < merge_blocks.size() - 1; i += 1)
        {
            auto merge_block = merge_blocks.at(i);
            builder.set_current_block(merge_block);
            auto tag_name = patterns.at(i + 1).first;
            auto match_tag_index = mir::MIR::create<mir::Integer, mir::Value>(
                context, scope, target_union_type.get_index(tag_name));
            auto comparison = builder.create_compare_primitive(scope, tag_info,
                                                               match_tag_index);
            builder.create_conditional_jump(
                scope, matching_blocks.at(i + 1), merge_blocks.at(i + 1),
                comparison);  // back to prev block and insert conditional jump.
        }

        std::optional<std::shared_ptr<mir::BasicBlock>> default_block;
        if (hir.get_default_case().has_value())
        {
            default_block =
                mir::BasicBlock::create(context, current_function, "default");
            builder.set_current_block(default_block.value());
            auto& hir_on_default = hir.get_default_case().value().get();
            auto scope_of_default =
                hir_on_default.get_basic_info().get_location()->get_scope_id();
            if (result_address.has_value())
            {
                builder.create_store(scope_of_default, result_address.value(),
                                     hir_on_default.accept(*this));
            }

            builder.set_current_block(merge_blocks.back());
            builder.create_jump(scope, default_block.value());
        }

        auto last_merge_block =
            mir::BasicBlock::create(context, current_function, "last_merge");

        if (default_block.has_value())
        {
            builder.set_current_block(default_block.value());
            builder.create_jump(scope, last_merge_block);
        }

        else
        {
            builder.set_current_block(merge_blocks.back());
            builder.create_jump(scope, last_merge_block);
        }

        for (auto& last_of_matching_block : last_blocks_of_matching_blocks)
        {
            builder.set_current_block(last_of_matching_block);
            builder.create_jump(scope, last_merge_block);
        }

        builder.set_current_block(last_merge_block);
        if (result_address.has_value())
        {
            return builder.create_load(scope, result_address.value());
        }

        return mir::MIR::create<mir::Integer, mir::Value>(context, scope, 0);
    }

    std::shared_ptr<mir::Value> visit(const hir::If& hir) const override
    {
        auto scope = hir.get_location()->get_scope_id();
        auto condition = hir.get_condition().accept(*this);
        std::optional<std::shared_ptr<mir::Value>> result_address;
        if (!hir.get_type()->is_type<requirement::VoidType>())
        {
            result_address = builder.create_allocate(scope, hir.get_type());
        }
        auto current_function = builder.get_current_block()->get_owner();
        auto previous_block = builder.get_current_block();
        auto then_block =
            mir::BasicBlock::create(context, current_function, "then");

        /*
        auto else_block =
            mir::BasicBlock::create(context, current_function, "else");
        auto merge_block =
            mir::BasicBlock::create(context, current_function, "merge");
        */
        builder.set_current_block(then_block);
        auto& then_hir = hir.get_body();
        auto then_scope =
            then_hir.get_basic_info().get_location()->get_scope_id();
        auto then_processing = then_hir.accept(*this);
        if (result_address.has_value())
        {
            builder.create_store(then_scope, result_address.value(),
                                 then_processing);
        }
        // builder.create_jump(then_scope, merge_block);
        then_block = builder.get_current_block();
        auto else_block =
            mir::BasicBlock::create(context, current_function, "else");
        builder.set_current_block(else_block);
        if (hir.get_else_body().has_value())
        {
            auto& else_hir = hir.get_else_body().value().get();
            auto else_processing = else_hir.accept(*this);
            auto else_scope =
                else_hir.get_basic_info().get_location()->get_scope_id();
            if (result_address.has_value())
            {
                builder.create_store(else_scope, result_address.value(),
                                     else_processing);
            }
            // builder.create_jump(else_scope, merge_block);
        }
        else
        {
            // builder.create_jump(scope, merge_block);
        }
        else_block = builder.get_current_block();
        auto merge_block =
            mir::BasicBlock::create(context, current_function, "merge");

        builder.set_current_block(previous_block);
        builder.create_conditional_jump(scope, then_block, else_block,
                                        condition);
        builder.set_current_block(then_block);
        builder.create_jump(then_scope, merge_block);
        builder.set_current_block(else_block);
        builder.create_jump(scope, merge_block);
        builder.set_current_block(merge_block);
        if (result_address.has_value())
        {
            return builder.create_load(scope, result_address.value());
        }
        return mir::MIR::create<mir::Integer, mir::Value>(context, scope, 0);
    }

    std::shared_ptr<mir::Value> visit(const hir::Loop& hir) const override
    {
        auto scope = hir.get_location()->get_scope_id();
        auto current_function = builder.get_current_block()->get_owner();

        // auto merge_block = mir::BasicBlock::create(context, current_function,
        // "merge");
        std::optional<std::shared_ptr<mir::Value>> result_address;
        if (!hir.get_type()->is_type<requirement::VoidType>())
        {
            result_address = builder.create_allocate(scope, hir.get_type(),
                                                     "result_of_repeat");
        }
        auto repeat_block =
            mir::BasicBlock::create(context, current_function, "repeat");
        builder.create_jump(scope, repeat_block);
        builder.set_current_block(repeat_block);
        auto repeat_continue_block = mir::BasicBlock::create(
            context, current_function, "repeat_continue");
        builder.set_current_block(repeat_continue_block);
        auto result = hir.get_body().accept(*this);
        if (result_address.has_value())
        {
            builder.create_store(scope, result_address.value(), result);
        }
        builder.create_jump(scope, repeat_block);
        auto merge_block =
            mir::BasicBlock::create(context, current_function, "merge");
        builder.set_current_block(repeat_block);
        builder.create_conditional_jump(scope, repeat_continue_block,
                                        merge_block,
                                        hir.get_condition().accept(*this));
        builder.set_current_block(merge_block);
        if (result_address.has_value())
        {
            return builder.create_load(scope, result_address.value());
        }
        return mir::MIR::create<mir::Integer, mir::Value>(context, scope, 0);
    }

    std::shared_ptr<mir::Value> visit(
        const hir::BinaryExpression& hir) const override
    {
        auto scope = hir.get_location()->get_scope_id();
        auto kind = hir.get_kind();
        auto left = hir.get_targets().first.get().accept(*this);
        auto right = hir.get_targets().second.get().accept(*this);
        std::optional<std::string> builtin_function_name_to_call;

        auto is_string = [](std::shared_ptr<clawn::mir::Value> mir) {
            return mir->get_type()->is_type<requirement::ReferenceType>() &&
                   mir->get_type()
                       ->as<requirement::ReferenceType>()
                       .get_reference_to()
                       ->is_type<requirement::StructureType>() &&
                   mir->get_type()
                           ->as<requirement::ReferenceType>()
                           .get_reference_to()
                           ->as<requirement::StructureType>()
                           .get_name() == "String";
        };

        if (kind == hir::OperatorKind::Addition)
        {
            if (left->get_type()->is_type<requirement::IntegerType>())
            {
                builtin_function_name_to_call = "add_int";
            }
            if (left->get_type()->is_type<requirement::RealNumberType>())
            {
                builtin_function_name_to_call = "add_double";
            }
            if (is_string(left))
            {
                builtin_function_name_to_call = "get_appended_string";
            }
        }
        if (kind == hir::OperatorKind::Subtraction)
        {
            if (left->get_type()->is_type<requirement::IntegerType>())
            {
                builtin_function_name_to_call = "sub_int";
            }
            if (left->get_type()->is_type<requirement::RealNumberType>())
            {
                builtin_function_name_to_call = "sub_double";
            }
        }
        if (kind == hir::OperatorKind::Multiplication)
        {
            if (left->get_type()->is_type<requirement::IntegerType>())
            {
                builtin_function_name_to_call = "mul_int";
            }
            if (left->get_type()->is_type<requirement::RealNumberType>())
            {
                builtin_function_name_to_call = "mul_double";
            }
        }
        if (kind == hir::OperatorKind::Division)
        {
            if (left->get_type()->is_type<requirement::IntegerType>())
            {
                if (right->get_type()->is_type<requirement::IntegerType>())
                {
                    builtin_function_name_to_call = "div_int_int";
                }
                if (right->get_type()->is_type<requirement::RealNumberType>())
                {
                    builtin_function_name_to_call = "div_int_doube";
                }
            }
            if (left->get_type()->is_type<requirement::RealNumberType>())
            {
                if (right->get_type()->is_type<requirement::IntegerType>())
                {
                    builtin_function_name_to_call = "div_double_int";
                }
                if (right->get_type()->is_type<requirement::RealNumberType>())
                {
                    builtin_function_name_to_call = "div_double_int";
                }
            }
        }
        if (kind == hir::OperatorKind::GreaterThan)
        {
            if (left->get_type()->is_type<requirement::IntegerType>())
            {
                builtin_function_name_to_call = "op_greater_than_int";
            }
            if (left->get_type()->is_type<requirement::RealNumberType>())
            {
                builtin_function_name_to_call = "op_greater_than_double";
            }
        }
        if (kind == hir::OperatorKind::GreaterThanOrEqual)
        {
            if (left->get_type()->is_type<requirement::IntegerType>())
            {
                builtin_function_name_to_call = "op_greater_than_or_equal_int";
            }
            if (left->get_type()->is_type<requirement::RealNumberType>())
            {
                builtin_function_name_to_call =
                    "op_greater_than_or_equal_double";
            }
        }
        if (kind == hir::OperatorKind::LessThan)
        {
            if (left->get_type()->is_type<requirement::IntegerType>())
            {
                builtin_function_name_to_call = "op_less_than_int";
            }
            if (left->get_type()->is_type<requirement::RealNumberType>())
            {
                builtin_function_name_to_call = "op_less_than_double";
            }
        }
        if (kind == hir::OperatorKind::LessThanOrEqual)
        {
            if (left->get_type()->is_type<requirement::IntegerType>())
            {
                builtin_function_name_to_call = "op_less_than_or_equal_int";
            }
            if (left->get_type()->is_type<requirement::RealNumberType>())
            {
                builtin_function_name_to_call = "op_less_than_or_equal_double";
            }
        }
        if (kind == hir::OperatorKind::Equal)
        {
            if (left->get_type()->is_type<requirement::IntegerType>())
            {
                builtin_function_name_to_call = "op_equal_int";
            }
            if (left->get_type()->is_type<requirement::RealNumberType>())
            {
                builtin_function_name_to_call = "op_equal_double";
            }
            if (is_string(left))
            {
                builtin_function_name_to_call = "op_equal_string";
            }
        }
        if (kind == hir::OperatorKind::NotEqual)
        {
            if (left->get_type()->is_type<requirement::IntegerType>())
            {
                builtin_function_name_to_call = "op_not_equal_int";
            }
            if (left->get_type()->is_type<requirement::RealNumberType>())
            {
                builtin_function_name_to_call = "op_not_equal_double";
            }
        }
        if (kind == hir::OperatorKind::And)
        {
            builtin_function_name_to_call = "op_and";
        }
        if (kind == hir::OperatorKind::Or)
        {
            builtin_function_name_to_call = "op_or";
        }

        if (!builtin_function_name_to_call.has_value())
        {
            throw std::runtime_error("Invalid Operator");
        }

        auto builtin_function = builder.create_load(
            scope,
            variable_addresses.at(builtin_function_name_to_call.value()));
        return builder.create_call(scope, builtin_function, {left, right});
    }

    std::shared_ptr<mir::Value> visit(const hir::SetResult& hir) const override
    {
        return hir.get_expression().accept(*this);
    }

    std::shared_ptr<mir::Value> visit(const hir::Return& hir) const override
    {
        // return hir.get_expression().accept(*this);

        auto scope = hir.get_location()->get_scope_id();
        auto function = builder.get_current_block()->get_owner();
        auto expression = hir.get_expression().accept(*this);
        auto return_type = function->get_type()
                               ->as<requirement::FunctionType>()
                               .get_return_type();

        if ((!hir.get_type()->is_same_as(*return_type)) &&
            return_type->is_type<requirement::UnionType>())
        {
            auto& return_type_as_union =
                return_type->as<requirement::UnionType>();
            for (auto& tag : return_type_as_union.get_solved_tags())
            {
                auto tag_name = tag.first;
                auto tag_type = tag.second;
                if (tag_name == "")
                {
                    continue;
                }
                if (expression->get_type()->is_same_as(*tag_type))
                {
                    auto return_value_allocation =
                        builder.create_allocate(scope, return_type);
                    auto index = return_type_as_union.get_index(tag_name);
                    auto index_pointer = builder.create_get_element_pointer(
                        scope, return_value_allocation, 0);
                    builder.create_store(
                        scope, index_pointer,
                        mir::MIR::create<mir::Integer, mir::Value>(
                            context, scope, index));
                    auto element_pointer = builder.create_get_element_pointer(
                        scope, return_value_allocation, index);
                    builder.create_store(scope, element_pointer, expression);
                    auto value_to_return =
                        builder.create_load(scope, return_value_allocation);
                    builder.create_return(scope, value_to_return);
                    return value_to_return;
                }
            }
            auto ch = type_environment.get_instantiations(function->get_type());

            throw std::runtime_error(
                "return type dind't match. " + hir.get_type()->to_string() +
                " should be in " + return_type->to_string());
        }
        else
        {
            builder.create_return(hir.get_location()->get_scope_id(),
                                  expression);
            return expression;
        }
    }
};

void connect_blocks(std::shared_ptr<mir::Context> context)
{
    for (auto& function : context->get_functions())
    {
        auto& blocks = function->get_body();
        size_t block_index = 0;
        for (auto& block : blocks)
        {
            size_t mir_index_in_block = 0;
            for (auto& mir_ : block->get_mirs())
            {
                if (mir_->get_id() == block->get_mirs()
                                          .back()
                                          ->get_id())  // last mir in the block
                {
                    if (mir_->is_type<mir::Value>() &&
                        block_index < blocks.size() - 1)
                    {
                        block->insert(mir::MIR::create<mir::Jump>(
                            context, hierarchy::Hierarchy::get_root(),
                            blocks.at(block_index + 1)));
                    }
                }
                mir_index_in_block += 1;
            }

            if (block->get_mirs().empty() && block_index < blocks.size() - 1)
            {
                block->insert(mir::MIR::create<mir::Jump>(
                    context, hierarchy::Hierarchy::get_root(),
                    blocks.at(block_index + 1)));
            }
            block_index += 1;
        }
    }
}

std::shared_ptr<mir::Value> hir_to_mir(const hir::HIR& hir,
                                       std::shared_ptr<mir::Context> context)
{
    auto converter = HIRToMIR(context);
    auto mir = hir.accept(converter);
    connect_blocks(context);

    return mir;
}
}  // namespace clawn::compiler::pipeline