#pragma once
#include <string>
#include <include/utils/hash_table.hpp>
#include <include/hierarchy/hierarchy.hpp>

namespace clawn::location
{
class TokenInfo
{
    private:
    size_t line_number;
    size_t index_in_line;
    const std::string token_string;

    public:
    TokenInfo() = delete;
    TokenInfo(size_t line_number, size_t index_in_line,
              std::string token_string)
        : line_number(line_number),
          index_in_line(index_in_line),
          token_string(token_string)
    {
    }
    const size_t get_line_number() const;
    const size_t get_index_in_line() const;
    const std::string& get_token_string() const;
};

class Location
{
    private:
    const std::string filename;
    const TokenInfo token_info;
    const hierarchy::HierarchyID scope_id;

    public:
    Location() = delete;
    Location(const Location&) = default;
    Location(const std::string& filename, TokenInfo token_info,
             hierarchy::HierarchyID scope_id)
        : filename(filename), token_info(token_info), scope_id(scope_id)
    {
    }

    const std::string& get_filename() const;
    const TokenInfo& get_token_info() const;
    const hierarchy::HierarchyID get_scope_id() const;
    size_t hash() const;
};

}  // namespace clawn::location