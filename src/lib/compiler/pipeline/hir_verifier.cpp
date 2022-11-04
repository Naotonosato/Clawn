#include <optional>
#include <algorithm>
#include "include/hir/hir.hpp"
#include "include/hir/visitor.hpp"
#include "include/requirement/requirement.hpp"
#include "include/compiler/error.hpp"
#include "include/compiler/module/module.hpp"
#include "include/compiler/pipeline.hpp"
#include "include/compiler/builtins.hpp"
#include "include/resource/resource.hpp"
#include "include/utils/pattern_match.hpp"
#include "include/utils/transform.hpp"

namespace clawn::compiler
{
namespace visitors
{
class HIRVerifier
{
    private:
    std::unique_ptr<class HIRVerifierImpl> impl;
    const std::shared_ptr<Module> clawn_module;
    const std::shared_ptr<requirement::TypeEnvironment> type_environment;

    public:
    HIRVerifier(const std::shared_ptr<Module> clawn_module);
    std::optional<Error> visit(const hir::HIR& hir) const;
    ~HIRVerifier();
};

class HIRVerifierImpl : public hir::Visitor<std::optional<Error>>
{
    private:
    const std::shared_ptr<requirement::TypeEnvironment> type_environment;
    using VariableName = std::string;
    using ElementName = std::string;
    mutable std::map<hierarchy::HierarchyID,
                     std::pair<VariableName, ElementName>>
        accessible_scopes;
    // accessible_scopes[location] = (variable_name,element_name)

    public:
    HIRVerifierImpl(
        std::shared_ptr<requirement::TypeEnvironment> type_environment)
        : type_environment(type_environment)
    {
    }

    std::optional<Error> visit(const hir::Root& hir) const override
    {
        for (auto&& hir : hir.get_children())
        {
            auto error = hir.get().accept(*this);
            if (error.has_value())
            {
                return error;
            }
        }
        return {std::nullopt};
    }
    std::optional<Error> visit(const hir::Integer& hir) const override
    {
        return hir.get_type()->is_type<requirement::IntegerType>()
                   ? std::nullopt
                   : std::make_optional(
                         Error(hir.get_location(), resource::Path()));
    }
    std::optional<Error> visit(const hir::Float& hir) const override
    {
        return hir.get_type()->is_type<requirement::RealNumberType>()
                   ? std::nullopt
                   : std::make_optional(
                         Error(hir.get_location(), resource::Path()));
    }
    std::optional<Error> visit(const hir::String& hir) const override
    {
        /*
        return hir.get_type()->is_type<requirement::StringType>()
                   ? std::nullopt
                   : std::make_optional(
                         Error(hir.get_location(), resource::Path()));
                         */
        return std::nullopt;
    }
    std::optional<Error> visit(const hir::Reference& hir) const override
    {
        return {std::nullopt};
    }
    std::optional<Error> visit(const hir::Dereference& hir) const override
    {
        auto error = hir.get_target().accept(*this);
        if (error.has_value())
        {
            return error;
        }
        if (!hir.get_target().get_type()->is_type<requirement::ReferenceType>())
        {
            return Error(
                hir.get_location(),
                resource::Path(
                    "you can't use 'access' to value(not reference) "));
        }
        return {std::nullopt};
    }
    std::optional<Error> visit(const hir::List& hir) const override
    {
        return utils::Match{hir.get_type()->get_variant()}(
            utils::Type<requirement::StructureType>(),
            [this, &hir](const requirement::StructureType& type)
                -> std::optional<Error> {
                if (!(type.get_name() == builtins::STRUCTURE_LIST_NAME))
                {
                    return Error(hir.get_location(),
                                 resource::Path("List is not typed List!?"));
                }
                return std::nullopt;
            },
            utils::Default(),
            [&hir_ = hir](auto &&) -> std::optional<Error> {
                return Error(hir_.get_location(),
                             resource::Path("List is not typed List!?"));
            });
    }
    std::optional<Error> visit(const hir::Variable& hir) const override
    {
        return {std::nullopt};
    }
    std::optional<Error> visit(const hir::Function& hir) const override
    {
        return {std::nullopt};
    }
    std::optional<Error> visit(const hir::Assignment& hir) const override
    {
        auto& right = hir.get_right_hand_side();
        auto right_error = right.accept(*this);
        auto& left = hir.get_left_hand_side();
        auto left_error = left.accept(*this);
        // left.get_type()->is_same_as(*left.get_type());
        if (right_error.has_value())
        {
            return right_error;
        }
        if (left_error.has_value())
        {
            return left_error;
        }

        if (left.get_type()->is_type<requirement::VoidType>())
        {
            return Error::void_assignment(hir.get_location());
        }

        if (hir.is_both_reference())
        {
            if (!(right.get_type()->is_type<requirement::ReferenceType>() &&
                  left.get_type()->is_type<requirement::ReferenceType>()))
            {
                return Error(hir.get_location(),
                             resource::Path("`<->` operator is only available "
                                            "with reference types."));
            }
        }
        return {std::nullopt};
    }
    std::optional<Error> visit(const hir::FunctionCall& hir) const override
    {
        auto location = hir.get_location();
        auto function_type = hir.get_function_obj().get_type();

        if (!function_type->is_type<requirement::FunctionType>())
        {
            return Error::invalid_call(location);
        }

        auto argument_types = function_type->as<requirement::FunctionType>()
                                  .get_argument_types();  // parameter_types?
        auto passed_arguments = hir.get_arguments();
        if (passed_arguments.size() != argument_types.size())
        {
            return Error::invalid_arguments_number(
                location, argument_types.size(), passed_arguments.size());
        }
        for (size_t i = 0; i < argument_types.size(); i += 1)
        {
            auto argument_type = argument_types[i];
            auto passed_argument_type = passed_arguments[i].get().get_type();
            if (passed_argument_type->is_same_as(*argument_type))
            {
                continue;
            }
            if (argument_type->is_type<requirement::UnionType>() &&
                argument_type->as<requirement::UnionType>()._is_by_compiler())
            {
                bool ok = false;
                for (auto& tag : argument_type->as<requirement::UnionType>()
                                     .get_solved_tags())
                {
                    if (tag.second->is_same_as(*passed_argument_type))
                    {
                        ok = true;
                        break;
                    }
                }
                if (!ok)
                {
                    return Error::invalid_argument_type(
                        location, argument_type->to_string(),
                        passed_argument_type->to_string(), i + 1);
                }
                continue;
            }
            return Error::invalid_argument_type(
                location, argument_type->to_string(),
                passed_argument_type->to_string(), i + 1);
        }

        return {std::nullopt};
    }
    std::optional<Error> visit(const hir::AccessElement& hir) const override
    {
        auto& left_hand_side = hir.get_left_hand_side();
        auto element_name = hir.get_element_name();
        // this->clawn_module->get_hir_module()->get_type_by_identifier("")->
        if (left_hand_side.get_type()->is_type<requirement::StructureType>())
        {
            if (left_hand_side.get_type()
                    ->as<requirement::StructureType>()
                    .has_member_named(element_name))
            {
                return {std::nullopt};
            }
        }
        if (left_hand_side.get_type()->is_type<requirement::UnionType>())
        {
            if (left_hand_side.get_type()
                    ->as<requirement::UnionType>()
                    .get_solved_tags()
                    .count(element_name))
            {
                auto scope = hir.get_location()->get_scope_id();
                if (left_hand_side.is_type<hir::Variable>() &&
                    accessible_scopes.count(scope) &&
                    (accessible_scopes[scope].first ==
                     left_hand_side.as<hir::Variable>().get_name()) &&
                    (accessible_scopes[scope].second == element_name))
                {
                    return {std::nullopt};
                }
                else
                {
                    return Error::unsafe_accessing(hir.get_location(),
                                                   element_name);
                }
            }
        }
        return Error::no_such_member(hir.get_location(), element_name);
    }
    std::optional<Error> visit(const hir::AccessList& hir) const override
    {
        return {std::nullopt};
    }
    std::optional<Error> visit(const hir::Construction& hir) const override
    {
        // this->clawn_module->get_hir_module()->get_type_by_identifier("")->
        return {std::nullopt};
    }
    std::optional<Error> visit(const hir::UnionConstruction& hir) const override
    {
        // this->clawn_module->get_hir_module()->get_type_by_identifier("")->
        return {std::nullopt};
    }
    std::optional<Error> visit(const hir::Block& hir) const override
    {
        for (auto&& hir_ : hir.get_list())
        {
            auto err = hir_.get().accept(*this);
            if (err.has_value())
            {
                return err;
            }
        }
        return {std::nullopt};
    }

    std::optional<Error> visit(const hir::Match& hir) const override
    {
        auto location = hir.get_location();
        auto& target = hir.get_target();
        if (target.get_type()->get_name() == "[GLOBAL]/Various")
        {
            return std::nullopt;
        }
        auto target_error = target.accept(*this);
        if (target_error.has_value())
        {
            return target_error;
        }

        auto default_case = hir.get_default_case();
        if (default_case.has_value())
        {
            auto default_case_error = default_case.value().get().accept(*this);
            if (default_case_error.has_value())
            {
                return default_case_error;
            }
        }

        auto target_type = target.get_type();
        if (target_type->is_type<requirement::UnionType>())
        {
            auto& tags = target_type->as<requirement::UnionType>().get_tags();

            for (auto& pattern : hir.get_patterns())
            {
                auto pattern_name = pattern.first;
                auto& pattern_expression = pattern.second.get();
                if (!tags.count(pattern_name))
                {
                    return Error::invalid_match_case(location, pattern_name);
                }
                if (target.is_type<hir::Variable>())
                {
                    auto union_variable_name =
                        target.as<hir::Variable>().get_name();
                    auto accessible_scope = pattern_expression.get_basic_info()
                                                .get_location()
                                                ->get_scope_id();
                    accessible_scopes.insert(std::make_pair(
                        accessible_scope,
                        std::make_pair(union_variable_name, pattern_name)));
                }
            }
            if (!hir.get_default_case().has_value())
            {
                auto pattern_names = utils::get_transformed(
                    hir.get_patterns(),
                    [](const auto& pattern) { return pattern.first; });
                for (auto& tag_pair : tags)
                {
                    auto tag_name = tag_pair.first;
                    if (tag_name == "")
                    {
                        continue;
                    }
                    if (!std::count(pattern_names.begin(), pattern_names.end(),
                                    tag_name))
                    {
                        return Error::never_handled(location, tag_name);
                    }
                }
            }
            for (auto& pattern : hir.get_patterns())
            // patterns have to be processed after above proccessings because
            // accessible locations are not set untill it.
            {
                auto pattern_error = pattern.second.get().accept(*this);
                if (pattern_error.has_value())
                {
                    return pattern_error;
                }
            }
            return {std::nullopt};
        }
        else
        {
            return Error::invalid_match_target(
                location, hir.get_target().get_type()->to_string());
        }
    }

    std::optional<Error> visit(const hir::If& hir) const override
    {
        auto is_condition_boolean =
            hir.get_condition().get_type()->is_type<requirement::BooleanType>();
        if (!is_condition_boolean)
        {
            return Error::invalid_condition_type(
                hir.get_location(), hir.get_condition().get_type()->get_name());
        }

        auto condition_error = hir.get_condition().accept(*this);
        if (condition_error.has_value())
        {
            return condition_error;
        }

        auto body_error = hir.get_body().accept(*this);
        if (body_error.has_value())
        {
            return body_error;
        }

        auto& else_body = hir.get_else_body();
        if (else_body.has_value())
        {
            return hir.get_else_body().value().get().accept(*this);
        }
        return std::nullopt;
    }

    std::optional<Error> visit(const hir::Loop& hir) const override
    {
        return std::nullopt;
    }

    std::optional<Error> visit(const hir::BinaryExpression& hir) const override
    {
        auto targets = hir.get_targets();

        return {std::nullopt};
    }

    std::optional<Error> visit(const hir::SetResult& hir) const override
    {
        return std::nullopt;
    }

    std::optional<Error> visit(const hir::Return& hir) const override
    {
        return std::nullopt;
    }
};

HIRVerifier::HIRVerifier(const std::shared_ptr<Module> clawn_module)
    : clawn_module(clawn_module),
      type_environment(clawn_module->get_type_environment())
{
    impl = std::make_unique<HIRVerifierImpl>(type_environment);
}

std::optional<Error> HIRVerifier::visit(const hir::HIR& hir_root) const
{
    auto error = hir_root.accept(*impl);
    auto history = std::vector<requirement::TypeBinding>(
        type_environment->get_solver()->get_history());
    /*
    std::sort(
        history.begin(),
        history.end(),
        [](const constraint::TypeBinding& lhs, const constraint::TypeBinding&
    rhs)
            {
                return (lhs.get_location()->get_token_info().get_line_number())
                <= (rhs.get_location()->get_token_info().get_line_number());
            }
        );
    */
    for (auto& binding : history)
    {
        /* std::cout << binding.from()->to_string() << "->"
                   << binding.to()->to_string() << "(line:"
                   << binding.get_location()->get_token_info().get_line_number()
                   << " '" <<
           binding.get_location()->get_token_info().get_token_string() << "')`"
           <<

             std::endl;
             */

        auto verification_result = binding.verify();
        if (verification_result.has_value())
        {
            auto location = binding.get_location();
            return Error::types_not_compatible(
                location, verification_result.value().first->to_string(),
                verification_result.value().second->to_string());
        }
    }

    clawn_module->verify_requirements();

    return error;
}

HIRVerifier::~HIRVerifier() = default;
}  // namespace visitors

std::optional<Error> pipeline::verify_hir(
    const hir::HIR& hir, const std::shared_ptr<Module> clawn_module)
{
    return visitors::HIRVerifier(clawn_module).visit(hir);
}

}  // namespace clawn::compiler