#include <string>
#include <utility>

namespace clawn::hierarchy
{
class Hierarchy;
std::string mangle(const std::string& name, const HierarchyID hierarchy);
std::pair<std::string, HierarchyID> demangle(const std::string& mangled_name);
}  // namespace clawn::hierarchy