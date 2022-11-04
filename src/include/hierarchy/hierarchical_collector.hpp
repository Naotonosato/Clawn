#pragma once
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <optional>
#include "include/hierarchy/hierarchy.hpp"
#include "include/utils/unique_number.hpp"

namespace clawn::hierarchy
{
template <typename T>
class HierarchicalCollector
{
    private:
    std::map<HierarchyID, std::map<std::string, T>> elements;
    HierarchyID current_hierarchy_id;
    HierarchyID top_hierarchy_id;
    int _unique_number;

    public:
    struct ElementAndHierarchy
    {
        std::optional<T> element;
        HierarchyID hierarchy_id;
        ElementAndHierarchy(std::optional<T> element, HierarchyID hierarchy_id)
            : element(element), hierarchy_id(hierarchy_id)
        {
        }
    };

    HierarchicalCollector(std::string top_hierarchy_name)
        : current_hierarchy_id(Hierarchy::create(top_hierarchy_name)),
          top_hierarchy_id(current_hierarchy_id)
    {
    }

    void add(std::string name, T new_element)
    {
        add(name, new_element, current_hierarchy_id);
    }

    void add(std::string name, T new_element, HierarchyID hierarchy_id)
    {
        elements[hierarchy_id].insert(std::make_pair(name, new_element));
    }

    ElementAndHierarchy get(const std::string& name, HierarchyID hierarchy_id)
    {
        auto accessible = hierarchy_id.enumerate_accesible_hierarchys();
        for (const auto& accessible_hierarchy : accessible)
        {
            if (elements[accessible_hierarchy].count(name))
            {
                return {elements[accessible_hierarchy].at(name),
                        accessible_hierarchy};
            }
        }
        return {std::nullopt, hierarchy_id};
    }

    ElementAndHierarchy get(const std::string& name)
    {
        return get(name, current_hierarchy_id);
    }

    void into_named_hierarchy(std::string name)
    {
        current_hierarchy_id = current_hierarchy_id.get_appended(name);
    }

    void into_anonymous_hierarchy()
    {
        int number = utils::UniqueNumber::get().get_unique_number();
        current_hierarchy_id = current_hierarchy_id.get_appended(
            std::string("anonymous#") + std::to_string(number));
    }

    void break_out_of_namespace()
    {
        current_hierarchy_id = current_hierarchy_id.get_poped_back();
    }

    HierarchyID& get_hierarchy_id() { return current_hierarchy_id; }

    void dump()
    {
        for (auto& pair : elements)
        {
            auto hierarchy_id = pair.first;
            auto& element_map = pair.second;
            std::cout << "in hierarchy " << hierarchy_id.to_string()
                      << std::endl;
            for (auto& name_and_element : elements[hierarchy_id])
            {
                std::cout << "    " << name_and_element.first << std::endl;
            }
        }
    }
};
}  // namespace clawn::hierarchy