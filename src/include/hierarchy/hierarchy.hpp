#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "include/utils/unique_number.hpp"
#include "include/utils/hash_table.hpp"

namespace clawn::hierarchy
{
class Hierarchy;

class HierarchyID : public utils::Hash<Hierarchy>
{
    public:
    HierarchyID() = delete;
    HierarchyID(utils::Hash<Hierarchy> id) : utils::Hash<Hierarchy>(id) {}
    HierarchyID get_appended(const std::string&) const;
    HierarchyID get_poped_back() const;
    std::string to_string() const;
    std::string front() const;
    std::string back() const;
    const char get_separator() const;
    bool is_deeper_than(const HierarchyID& hierarchy) const;
    bool is_shallower_than(const HierarchyID& hierarchy) const;
    const std::vector<HierarchyID> enumerate_accesible_hierarchys();
};

class Hierarchy final : protected std::string
{
    private:
    char separator;
    std::vector<std::string> hierarchy_names;
    Hierarchy(char separator = '/') : std::string(), separator(separator) {}
    Hierarchy(const std::string& name, char separator = '/');
    Hierarchy(std::string&& name, char separator = '/')
        : std::string(std::move(name)), separator(separator)
    {
    }

    Hierarchy(const Hierarchy&, std::string new_hierarchy_name,
              char separator = '/');

    public:
    Hierarchy(const Hierarchy&) = default;
    static HierarchyID get_root();
    static std::string get_root_name();
    static HierarchyID create(char separator = '/');
    static HierarchyID create(const std::string& name, char separator = '/');
    static HierarchyID create(const Hierarchy&);
    static HierarchyID create(const Hierarchy&, std::string new_hierarchy_name,
                              char separator = '/');
    static HierarchyID create(const HierarchyID&);
    static HierarchyID create(const HierarchyID&,
                              std::string new_hierarchy_name,
                              char separator = '/');
    Hierarchy& operator=(const Hierarchy&) = delete;
    Hierarchy& operator=(Hierarchy&&) = delete;

    HierarchyID get_appended(const std::string&) const;
    HierarchyID get_poped_back() const;
    std::string to_string() const;
    std::string front() const;
    std::string back() const;
    const char get_separator() const;
    bool operator==(const Hierarchy&) const;
    bool operator!=(const Hierarchy&) const;
    bool operator<(const Hierarchy&) const;
    bool is_deeper_than(const Hierarchy& hierarchy) const;
    bool is_shallower_than(const Hierarchy& hierarchy) const;
    const std::vector<HierarchyID> enumerate_accesible_hierarchys();

    size_t hash() const;
};

using HierarchyPool = utils::HashTable<Hierarchy>;

}  // namespace clawn::hierarchy