#include "include/requirement/type.hpp"
#include "include/requirement/type_solver.hpp"
#include "include/hir/hir.hpp"
#include "include/hir/visitor.hpp"
#include "include/location/location.hpp"
#include "include/utils/transform.hpp"

namespace clawn::hir
{
std::unique_ptr<hir::HIR> Cloner::visit(const hir::Root& hir) const
{
    std::vector<std::unique_ptr<HIR>> children = utils::get_transformed(
        hir.get_children(),
        [this](const HIR& child) { return child.accept(*this); });

    return HIR::create<Root>(
        hir.get_type_without_solving()->clone(cloned_type_map),
        hir.get_location(), std::move(children));
}
std::unique_ptr<hir::HIR> Cloner::visit(const hir::Integer& hir) const
{
    return HIR::create<Integer>(
        hir.get_type_without_solving()->clone(cloned_type_map),
        hir.get_location(), hir.get_initial_value());
}
std::unique_ptr<hir::HIR> Cloner::visit(const hir::Float& hir) const
{
    return HIR::create<Float>(
        hir.get_type_without_solving()->clone(cloned_type_map),
        hir.get_location(), hir.get_initial_value());
}
std::unique_ptr<hir::HIR> Cloner::visit(const hir::String& hir) const
{
    return HIR::create<String>(
        hir.get_type_without_solving()->clone(cloned_type_map),
        hir.get_location(), hir.get_initial_value());
}
std::unique_ptr<hir::HIR> Cloner::visit(const hir::Reference& hir) const
{
    return HIR::create<Reference>(
        hir.get_type_without_solving()->clone(cloned_type_map),
        hir.get_location(), hir.get_refer_to().accept(*this));
}
std::unique_ptr<hir::HIR> Cloner::visit(const hir::Dereference& hir) const
{
    return HIR::create<Dereference>(
        hir.get_type_without_solving()->clone(cloned_type_map),
        hir.get_location(), hir.get_target().accept(*this));
}
std::unique_ptr<hir::HIR> Cloner::visit(const hir::List& hir) const
{
    return HIR::create<List>(
        hir.get_type_without_solving()->clone(cloned_type_map),
        hir.get_location(),
        utils::get_transformed(hir.get_initial_values(),
                               [this](const auto& initial_value) {
                                   return initial_value.get().accept(*this);
                               }));
}
std::unique_ptr<hir::HIR> Cloner::visit(const hir::Variable& hir) const
{
    // std::cout << "cloning variable " << Printer().visit(hir) << ":" <<
    // hir.get_type()->to_string() << std::endl;
    return HIR::create<Variable>(
        hir.get_type_without_solving()->clone(cloned_type_map), hir.get_name(),
        hir.get_location());  //,hir.get_name());
}
std::unique_ptr<hir::HIR> Cloner::visit(const hir::Function& hir) const
{
    auto body = hir.get_body().accept(*this);
    auto cloned_type = hir.get_type_without_solving()->clone(cloned_type_map);
    type_environment.register_return_type(
        cloned_type,
        cloned_type->as<requirement::FunctionType>()
            .get_argument_types_without_solving(),
        body->get_type_without_solving());
    return HIR::create<Function>(cloned_type, hir.get_location(),
                                 hir.get_name(), std::move(body),
                                 hir.is_declaration_only());
}
std::unique_ptr<hir::HIR> Cloner::visit(const hir::Assignment& hir) const
{
    auto left_hand_side = hir.get_left_hand_side().accept(*this);
    auto right_hand_side = hir.get_right_hand_side().accept(*this);
    return HIR::create<Assignment>(
        hir.get_type_without_solving()->clone(cloned_type_map),
        hir.get_location(), std::move(right_hand_side),
        std::move(left_hand_side));
}
std::unique_ptr<hir::HIR> Cloner::visit(const hir::FunctionCall& hir) const
{
    auto cloned_function = hir.get_function_obj().accept(*this);
    auto cloned_arguments = utils::get_transformed(
        hir.get_arguments(),
        [this](const HIR& argument) { return argument.accept(*this); });
    auto argument_types =
        utils::get_transformed(cloned_arguments, [](const auto& argument) {
            return argument->get_type_without_solving();
        });
    // type_environment.register_instanciation(cloned_function->get_type(),
    //                                        argument_types);

    return HIR::create<FunctionCall>(
        hir.get_type_without_solving()->clone(cloned_type_map),
        hir.get_location(), std::move(cloned_function),
        std::move(cloned_arguments));
}
std::unique_ptr<hir::HIR> Cloner::visit(const hir::AccessElement& hir) const
{
    auto cloned_left_hand_side = hir.get_left_hand_side().accept(*this);
    auto cloned_type = hir.get_type()->clone(cloned_type_map);
    auto element_name = hir.get_element_name();
    // type_environment.register_element_type(cloned_left_hand_side->get_type(),element_name,cloned_type);
    // std::cout << cloned_left_hand_side->get_type()->to_string() << " has " <<
    // element_name << ":" << cloned_type->to_string() << std::endl;
    return HIR::create<AccessElement>(cloned_type, hir.get_location(),
                                      std::move(cloned_left_hand_side),
                                      element_name);
}

std::unique_ptr<hir::HIR> Cloner::visit(const hir::AccessList& hir) const
{
    auto cloned_list = hir.get_list().accept(*this);
    auto cloned_index = hir.get_index().accept(*this);
    auto cloned_type = hir.get_type()->clone(cloned_type_map);
    // type_environment.register_element_type(cloned_left_hand_side->get_type(),element_name,cloned_type);
    // std::cout << cloned_left_hand_side->get_type()->to_string() << " has " <<
    // element_name << ":" << cloned_type->to_string() << std::endl;
    return HIR::create<AccessList>(cloned_type, hir.get_location(),
                                   std::move(cloned_list),
                                   std::move(cloned_index));
}

std::unique_ptr<hir::HIR> Cloner::visit(const hir::Construction& hir) const
{
    std::vector<std::pair<std::string, std::unique_ptr<HIR>>> cloned_arguments;
    for (auto& pair : hir.get_arguments())
    {
        cloned_arguments.push_back(
            std::make_pair(pair.first, pair.second.get().accept(*this)));
    }

    return HIR::create<Construction>(
        hir.get_type_without_solving()->clone(cloned_type_map),
        hir.get_location(), std::move(cloned_arguments));
}

std::unique_ptr<hir::HIR> Cloner::visit(const hir::UnionConstruction& hir) const
{
    auto cloned_argument =
        std::make_pair(hir.get_argument().first,
                       hir.get_argument().second.get().accept(*this));
    return HIR::create<UnionConstruction>(
        hir.get_type_without_solving()->clone(cloned_type_map),
        hir.get_location(), std::move(cloned_argument));
}

std::unique_ptr<hir::HIR> Cloner::visit(const hir::Block& hir) const
{
    auto hirs = utils::get_transformed(
        hir.get_list(), [this](const HIR& hir_) { return hir_.accept(*this); });
    return HIR::create<Block>(
        hir.get_type_without_solving()->clone(cloned_type_map),
        hir.get_location(), std::move(hirs));
}
std::unique_ptr<hir::HIR> Cloner::visit(const hir::Match& hir) const
{
    auto cloned_target = hir.get_target().accept(*this);
    auto cloned_patterns = utils::get_transformed(
        hir.get_patterns(),
        [this](const std::pair<std::string, std::reference_wrapper<const HIR>>&
                   pattern) {
            return std::make_pair(pattern.first,
                                  pattern.second.get().accept(*this));
        });
    auto default_case = hir.get_default_case();
    if (default_case.has_value())
    {
        return HIR::create<Match>(
            hir.get_type_without_solving()->clone(cloned_type_map),
            hir.get_location(), std::move(cloned_target),
            std::move(cloned_patterns),
            default_case.value().get().accept(*this));
    }
    else
    {
        return HIR::create<Match>(
            hir.get_type_without_solving()->clone(cloned_type_map),
            hir.get_location(), std::move(cloned_target),
            std::move(cloned_patterns));
    }
}

std::unique_ptr<hir::HIR> Cloner::visit(const hir::If& hir) const
{
    auto condition = hir.get_condition().accept(*this);
    auto body = hir.get_body().accept(*this);
    auto else_body = hir.get_else_body();
    if (else_body.has_value())
    {
        return HIR::create<If>(
            hir.get_type_without_solving()->clone(cloned_type_map),
            hir.get_location(), std::move(condition), std::move(body),
            else_body.value().get().accept(*this));
    }
    else
    {
        return HIR::create<If>(
            hir.get_type_without_solving()->clone(cloned_type_map),
            hir.get_location(), std::move(condition), std::move(body));
    }
}

std::unique_ptr<hir::HIR> Cloner::visit(const hir::Loop& hir) const
{
    return HIR::create<Loop>(
        hir.get_type_without_solving()->clone(cloned_type_map),
        hir.get_location(), hir.get_condition().accept(*this),
        hir.get_body().accept(*this));
}

std::unique_ptr<hir::HIR> Cloner::visit(const hir::BinaryExpression& hir) const
{
    auto cloned_targets =
        std::make_pair(hir.get_targets().first.get().accept(*this),
                       hir.get_targets().second.get().accept(*this));
    return HIR::create<BinaryExpression>(
        hir.get_type_without_solving()->clone(cloned_type_map),
        hir.get_location(), std::move(cloned_targets), hir.get_kind());
}

std::unique_ptr<hir::HIR> Cloner::visit(const hir::SetResult& hir) const
{
    return HIR::create<SetResult>(
        hir.get_type_without_solving()->clone(cloned_type_map),
        hir.get_location(), hir.get_expression().accept(*this));
}

std::unique_ptr<hir::HIR> Cloner::visit(const hir::Return& hir) const
{
    return HIR::create<Return>(
        hir.get_type_without_solving()->clone(cloned_type_map),
        hir.get_location(), hir.get_expression().accept(*this));
}
// Cloner::~Cloner() = default;

std::string Printer::visit(const hir::Root& hir) const
{
    std::string all = "~ROOT~";
    for (auto& child : hir.get_children())
    {
        all += child.get().accept(*this) + "\n";
    }

    return all;
}
std::string Printer::visit(const hir::Integer& hir) const
{
    return std::to_string(hir.get_initial_value());
}
std::string Printer::visit(const hir::Float& hir) const
{
    return std::to_string(hir.get_initial_value());
}
std::string Printer::visit(const hir::String& hir) const
{
    return '"' + hir.get_initial_value() + '"';
}
std::string Printer::visit(const hir::Reference& hir) const
{
    return "refer " + hir.get_refer_to().accept(*this);
}
std::string Printer::visit(const hir::Dereference& hir) const
{
    return "access " + hir.get_target().accept(*this);
}
std::string Printer::visit(const hir::List& hir) const
{
    std::string elements;
    for (auto& initial_value : hir.get_initial_values())
    {
        elements += initial_value.get().accept(*this) + ", ";
    }
    if (!elements.empty())
    {
        elements.pop_back();
        elements.pop_back();
    }
    return hir.get_type()->to_string() + "[" + elements + "]";
}
std::string Printer::visit(const hir::Variable& hir) const
{
    return hir.get_name() + ":@" + hir.get_type()->to_string();
}
std::string Printer::visit(const hir::Function& hir) const
{
    auto type_name = hir.get_type()->to_string();
    std::string body = hir.get_body().accept(*this);
    return "function:" + hir.get_name() + type_name + "{\n" + body + "\n}";
}
std::string Printer::visit(const hir::Assignment& hir) const
{
    return hir.get_left_hand_side().accept(*this) + " = " +
           hir.get_right_hand_side().accept(*this);
}
std::string Printer::visit(const hir::FunctionCall& hir) const
{
    auto function = hir.get_function_obj().to_string();
    std::string arguments;
    for (auto& argument : hir.get_arguments())
    {
        arguments += "    " + argument.get().accept(*this) + ",\n";
    }
    arguments.pop_back();
    arguments.pop_back();
    return function + "(\n" + arguments + "\n):" + hir.get_type()->to_string();
}
std::string Printer::visit(const hir::AccessElement& hir) const
{
    auto left_hand_side = hir.get_left_hand_side().accept(*this);
    auto element_name = hir.get_element_name();
    if (element_name.empty())
    {
        element_name = "[tag]";
    }
    return left_hand_side + "." + element_name;
}

std::string Printer::visit(const hir::AccessList& hir) const
{
    auto list = hir.get_list().accept(*this);
    auto index = hir.get_index().accept(*this);
    return list + "[" + index + "]";
}

std::string Printer::visit(const hir::Construction& hir) const
{
    // assert(false);
    std::string arguments;
    for (auto& argument : hir.get_arguments())
    {
        arguments +=
            argument.first + " : " + argument.second.get().accept(*this) + ", ";
    }
    arguments.pop_back();
    arguments.pop_back();
    return "construct " + hir.get_type()->to_string() + "{" + arguments + "}";
}

std::string Printer::visit(const hir::UnionConstruction& hir) const
{
    // assert(false);
    return "union construct " + hir.get_type()->to_string() + "{" +
           hir.get_argument().second.get().accept(*this) + "}";
}

std::string Printer::visit(const hir::Block& hir) const
{
    std::string all;
    for (auto& child : hir.get_list())
    {
        all += "  " + child.get().accept(*this) + "\n";
    }

    return "Block:{\n" + all + "}:" + hir.get_type()->to_string() + "\n";
}
std::string Printer::visit(const hir::Match& hir) const
{
    std::string all;
    all += "match " + hir.get_target().accept(*this) + "\n{\n";
    for (auto& pattern : hir.get_patterns())
    {
        all +=
            pattern.first + " => " + pattern.second.get().accept(*this) + "\n";
    }
    if (hir.get_default_case().has_value())
    {
        all +=
            "default"
            " => " +
            hir.get_default_case().value().get().accept(*this) + "\n";
    }

    return "{\n" + all + "}\n";
}
std::string Printer::visit(const hir::If& hir) const
{
    auto condition = hir.get_condition().accept(*this);
    auto body = hir.get_body().accept(*this);
    auto else_body = hir.get_else_body();
    if (else_body.has_value())
    {
        return "if " + condition + "\n{\n" + body + "\n}\n" + "else\n{\n" +
               else_body.value().get().accept(*this) + "\n}\n";
    }
    else
    {
        return "if " + condition + "\n{\n" + body + "\n}\n";
    }
}

std::string Printer::visit(const hir::Loop& hir) const
{
    return "loop while " + hir.get_condition().accept(*this) + "\n{\n" +
           hir.get_body().accept(*this) + "\n}" + hir.get_type()->to_string() +
           "\n";
}

std::string Printer::visit(const hir::BinaryExpression& hir) const
{
    return "compare(" + hir.get_targets().first.get().to_string() + ", " +
           hir.get_targets().second.get().to_string() + ")";
}

std::string Printer::visit(const hir::SetResult& hir) const
{
    return "=> " + hir.get_expression().accept(*this);
}

std::string Printer::visit(const hir::Return& hir) const
{
    return "return " + hir.get_expression().accept(*this);
}

}  // namespace clawn::hir