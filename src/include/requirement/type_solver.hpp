#pragma once
#include <memory>
#include <set>
#include "include/algorithm/union_find/union_find.hpp"
#include "include/location/location.hpp"

namespace clawn::requirement
{
class Type;
class ClonedTypeMap;
using TypeID = size_t;
class TypeEnvironment;
}  // namespace clawn::requirement
// forward declarations;

namespace clawn::requirement
{
class TypeBinding
{
    // equality requirement
    private:
    const std::shared_ptr<Type> _from;
    const std::shared_ptr<Type> _to;
    const std::shared_ptr<location::Location> location;

    public:
    TypeBinding(const std::shared_ptr<Type> from,
                const std::shared_ptr<Type> to,
                const std::shared_ptr<location::Location> location);
    const std::shared_ptr<Type> from() const;
    const std::shared_ptr<Type> to() const;
    const std::shared_ptr<location::Location> get_location() const;
    std::optional<
        std::pair<const std::shared_ptr<Type>, const std::shared_ptr<Type>>>
    verify() const;
};

class TypeSolver
{
    private:
    std::unordered_map<unsigned long, std::shared_ptr<Type>> id_type_map;
    algorithm::UnionFindTree<std::shared_ptr<Type>> type_tree;
    std::unordered_map<std::string, std::shared_ptr<Type>> named_types;
    std::vector<TypeBinding> history;
    std::set<std::shared_ptr<Type>> union_types;
    TypeEnvironment& environment;

    void complete_union_types(std::vector<std::shared_ptr<Type>>& types);

    public:
    TypeSolver(TypeEnvironment& environment) : environment(environment) {}
    using TypeTree = algorithm::UnionFindTree<std::shared_ptr<Type>>;
    void add_type_variable(std::shared_ptr<Type> new_type);
    void complementary_bind(std::shared_ptr<Type> type,
                            std::shared_ptr<Type> depend_on,
                            const std::shared_ptr<location::Location> location);
    void bind(std::shared_ptr<Type> type, std::shared_ptr<Type> depend_on,
              const std::shared_ptr<location::Location> location,
              bool complete_union_type = true);
    std::shared_ptr<Type> solve(std::shared_ptr<Type> type,
                                bool complete_union_type = true);
    void register_named_type(const std::string& name,
                             std::shared_ptr<Type> type);
    void register_type(std::shared_ptr<Type> type);
    std::shared_ptr<Type> get_type_by_id(size_t) const;
    const std::unordered_map<std::string, std::shared_ptr<Type>>&
    get_named_types() const;
    const TypeTree& get_all_types();
    const std::vector<TypeBinding>& get_history() const;
    std::optional<std::shared_ptr<Type>> get_binded_type(TypeID);
    void remap(const ClonedTypeMap& cloned_type_map,
               std::shared_ptr<location::Location> location);
    const TypeEnvironment& get_environment() const;
};
}  // namespace clawn::requirement

/*
l = lazy l:lazy
l2 = l l2->l1
l = l2 l1->l2

*/