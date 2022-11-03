#include <string>
#include <functional>
#include <iostream>
#include "include/hierarchy/hierarchy.hpp"

namespace clawn::hierarchy
{
HierarchyID HierarchyID::get_appended(const std::string& name) const
{
    return HierarchyPool::get(*this).get_appended(name);
}

HierarchyID HierarchyID::get_poped_back() const
{
    return HierarchyPool::get(*this).get_poped_back();
}

std::string HierarchyID::to_string() const
{
    return HierarchyPool::get(*this).to_string();
}

std::string HierarchyID::front() const
{
    return HierarchyPool::get(*this).front();
}

std::string HierarchyID::back() const
{
    return HierarchyPool::get(*this).back();
}

const char HierarchyID::get_separator() const
{
    return HierarchyPool::get(*this).get_separator();
}

bool HierarchyID::is_deeper_than(const HierarchyID& hierarchy) const
{
    return HierarchyPool::get(*this).is_deeper_than(
        HierarchyPool::get(hierarchy));
}

bool HierarchyID::is_shallower_than(const HierarchyID& hierarchy) const
{
    return HierarchyPool::get(*this).is_shallower_than(
        HierarchyPool::get(hierarchy));
}

const std::vector<HierarchyID> HierarchyID::enumerate_accesible_hierarchys()
{
    return HierarchyPool::get(*this).enumerate_accesible_hierarchys();
}

Hierarchy::Hierarchy(const std::string& name, char separator)
    : std::string(name), separator(separator)
{
    std::string hierarchy_name;
    for (const auto& letter : name)
    {
        if (letter == separator)
        {
            hierarchy_names.push_back(hierarchy_name);
            hierarchy_name = "";
        }
        else
        {
            hierarchy_name += letter;
        }
    }
    hierarchy_names.push_back(hierarchy_name);
}

Hierarchy::Hierarchy(const Hierarchy& old_hierarchy,
                     std::string new_hierarchy_name, char separator)
    : std::string(old_hierarchy.to_string() + separator + new_hierarchy_name),
      separator(separator)
{
    std::string hierarchy_name;
    for (const auto& letter : to_string())
    {
        if (letter == separator)
        {
            hierarchy_names.push_back(hierarchy_name);
            hierarchy_name = "";
        }
        else
        {
            hierarchy_name += letter;
        }
    }
    hierarchy_names.push_back(hierarchy_name);
}

HierarchyID Hierarchy::get_root() { return create("[GLOBAL]"); }

std::string Hierarchy::get_root_name() { return "[GLOBAL]"; }

HierarchyID Hierarchy::create(char separator)
{
    return HierarchyPool::add(Hierarchy(separator));
}
HierarchyID Hierarchy::create(const std::string& name, char separator)
{
    return HierarchyPool::add(Hierarchy(name, separator));
}

HierarchyID Hierarchy::create(const Hierarchy& hierarchy_)
{
    return HierarchyPool::add(Hierarchy(hierarchy_));
}
HierarchyID Hierarchy::create(const Hierarchy& hierarchy_,
                              std::string new_hierarchy_name, char separator)
{
    return HierarchyPool::add(
        Hierarchy(hierarchy_, new_hierarchy_name, separator));
}

HierarchyID Hierarchy::create(const HierarchyID& id)
{
    return Hierarchy::create(HierarchyPool::get(id));
}

HierarchyID Hierarchy::create(const HierarchyID& id,
                              std::string new_hierarchy_name, char separator)
{
    return Hierarchy::create(HierarchyPool::get(id), new_hierarchy_name,
                             separator);
}

HierarchyID Hierarchy::get_appended(const std::string& new_hierarchy_name) const
{
    return create(*this, new_hierarchy_name, separator);
}

HierarchyID Hierarchy::get_poped_back() const
{
    std::string hierarchy_name = to_string();
    int separator_length = 1;
    auto poped = to_string().substr(0, to_string().size() - separator_length -
                                           hierarchy_names.back().size());
    return create(poped);
}

bool Hierarchy::operator==(const Hierarchy& hierarchy) const
{
    return to_string() == hierarchy.to_string();
}

bool Hierarchy::operator!=(const Hierarchy& hierarchy) const
{
    return !(*this == hierarchy);
}

bool Hierarchy::operator<(const Hierarchy& hierarchy) const
{
    return to_string() < hierarchy.to_string();
}

bool Hierarchy::is_deeper_than(const Hierarchy& hierarchy) const
{
    if (hierarchy_names.size() < hierarchy.hierarchy_names.size()) return false;
    int idx = 0;
    for (const auto& hierarchy_name : hierarchy_names)
    {
        if (hierarchy_name != hierarchy.hierarchy_names[idx]) return false;
        idx += 1;
    }
    return true;
}

bool Hierarchy::is_shallower_than(const Hierarchy& hierarchy) const
{
    return hierarchy.is_deeper_than(*this);
}

std::string Hierarchy::to_string() const { return std::string(c_str()); }

std::string Hierarchy::front() const { return hierarchy_names.front(); }
std::string Hierarchy::back() const { return hierarchy_names.back(); }

const char Hierarchy::get_separator() const { return separator; }

const std::vector<HierarchyID> Hierarchy::enumerate_accesible_hierarchys()
{
    std::vector<HierarchyID> enumerated;
    std::string previous_hierarchy_name;
    for (const auto& hierarchy_name : hierarchy_names)
    {
        enumerated.push_back(create(previous_hierarchy_name + hierarchy_name));
        previous_hierarchy_name += hierarchy_name + separator;
    }
    return enumerated;
}

size_t Hierarchy::hash() const { return std::hash<std::string>()(to_string()); }

}  // namespace clawn::hierarchy

/*
#include <iostream>
int main()
{
    hierarchy::Hierarchy a = std::string("one/two/three");
    hierarchy::Hierarchy b(a,std::string("bbb"));
    hierarchy::Hierarchy c(b,std::string("ccc"));
    c.pop();
    c.append("ddd");
    std::cout << c.to_string() << std::endl;
    for (auto i:c.enumerate_accesible_hierarchys())
    {
        std::cout << i.to_string() << std::endl;
    }
}
*/