#include "include/location/location.hpp"

namespace clawn::location
{
const size_t TokenInfo::get_line_number() const { return line_number; }
const size_t TokenInfo::get_index_in_line() const { return index_in_line; }
const std::string& TokenInfo::get_token_string() const { return token_string; }

size_t Location::hash() const
{
    std::string sirialized =
        (std::to_string(scope_id.get_hash()) + filename + std::string("*")) +
        (std::to_string(token_info.get_line_number()) + ":" +
         std::to_string(token_info.get_index_in_line()) +
         get_token_info().get_token_string());
    return std::hash<std::string>()(sirialized);
}

const std::string& Location::get_filename() const { return filename; }

const TokenInfo& Location::get_token_info() const { return token_info; }

const hierarchy::HierarchyID Location::get_scope_id() const { return scope_id; }

}  // namespace clawn::location