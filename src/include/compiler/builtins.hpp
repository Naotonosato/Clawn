#include <string>
#include <memory>
#include "include/hierarchy/hierarchy.hpp"
#include "include/hierarchy/mangler.hpp"

namespace clawn::hir
{
class HIR;
}

namespace clawn::location
{
class Location;
}

namespace clawn::compiler::builtins
{
const std::string OPERATOR_AND =
    hierarchy::mangle("and", hierarchy::Hierarchy::get_root());
const std::string OPERATOR_EQUAL =
    hierarchy::mangle("EQUAL", hierarchy::Hierarchy::get_root());
std::unique_ptr<hir::HIR> create_calling_operator_and(
    std::unique_ptr<hir::HIR>&& x, std::unique_ptr<hir::HIR>&& y,
    std::shared_ptr<location::Location>);
std::unique_ptr<hir::HIR> create_calling_operator_equal(
    std::unique_ptr<hir::HIR>&& x, std::unique_ptr<hir::HIR>&& y,
    std::shared_ptr<location::Location>);
const std::string STRUCTURE_LIST_NAME = "[GLOBAL]/List";

enum class BuiltinFunction
{
    STRING_CONSTRUCTOR
};

}  // namespace clawn::compiler::builtins