#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <set>
#include <unordered_set>

namespace clawn::algorithm
{
template <typename ValueType>
class Tree
{
    public:
    Tree() = default;
    Tree(Tree&& other) = default;
    virtual const std::vector<std::reference_wrapper<const ValueType>>
    get_children() const = 0;
};
};  // namespace clawn::algorithm