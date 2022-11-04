#include <string>
#include <utility>
#include "include/hierarchy/hierarchy.hpp"
#include "include/hierarchy/mangler.hpp"

namespace clawn::hierarchy
{
std::string mangle(const std::string& name, HierarchyID hierarchy_id)
{
    auto separator = hierarchy_id.get_separator();
    auto mangled_name = hierarchy_id.to_string() + separator + name;
    return mangled_name;
}

std::pair<std::string, HierarchyID> demangle(const std::string& mangled_name)
{
    auto hierarchy_id = Hierarchy::create(mangled_name);
    auto name = hierarchy_id.back();
    return std::make_pair(name, hierarchy_id.get_poped_back());
}
}  // namespace clawn::hierarchy