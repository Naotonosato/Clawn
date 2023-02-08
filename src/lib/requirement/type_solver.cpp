#include "include/requirement/type_solver.hpp"
#include "include/requirement/type.hpp"
#include "include/hir/hir.hpp"

namespace clawn::requirement
{
TypeBinding::TypeBinding(const std::shared_ptr<Type> from,
                         const std::shared_ptr<Type> to,
                         const std::shared_ptr<location::Location> location)
    : _from(from), _to(to), location(location)
{
}
const std::shared_ptr<Type> TypeBinding::from() const { return _from; }

const std::shared_ptr<Type> TypeBinding::to() const { return _to; }

const std::shared_ptr<location::Location> TypeBinding::get_location() const
{
    return location;
}

std::optional<
    std::pair<const std::shared_ptr<Type>, const std::shared_ptr<Type>>>
TypeBinding::verify() const
{
    auto type_solver = from()->get_type_solver();
    const auto from_solved = type_solver->solve(from());
    const auto to_solved = type_solver->solve(to());

    if(
        (from()->is_type<UnionType>() && from()->as<UnionType>()._is_by_compiler())
        || (from_solved->is_type<UnionType>() && from_solved->as<UnionType>()._is_by_compiler())
        || (to()->is_type<UnionType>() && to()->as<UnionType>()._is_by_compiler())
        || (to_solved->is_type<UnionType>() && to_solved->as<UnionType>()._is_by_compiler())
    )//ignores types about generics
    {
        return std::nullopt;
    }

    if (!from()->is_same_as(*from_solved, true))
    {
        //if(
        //    !(from()->is_type<requirement::UnionType>() && from()->as<requirement::UnionType>()._can_be_treated_as(*from_solved).first)
        //    &&
        //    !(from_solved->is_type<requirement::UnionType>() && from_solved->as<requirement::UnionType>()._can_be_treated_as(*from()).first)
        //)
        //{
            return std::make_pair(from(), from_solved);
        //}
    }
    if (!to()->is_same_as(*to_solved, true))
    {
        //if(
        //    !(to()->is_type<requirement::UnionType>() && to()->as<requirement::UnionType>()._can_be_treated_as(*to_solved).first)
        //    &&
        //    !(to_solved->is_type<requirement::UnionType>() && to_solved->as<requirement::UnionType>()._can_be_treated_as(*to()).first)
        //)
        //{
            return std::make_pair(to(), to_solved);
        //}
    }
    if (!from()->is_same_as(*to_solved, true))
    {
        //if(
        //    !(from()->is_type<requirement::UnionType>() && from()->as<requirement::UnionType>()._can_be_treated_as(*to_solved).first)
        //    ||
        //    !(from_solved->is_type<requirement::UnionType>() && from_solved->as<requirement::UnionType>()._can_be_treated_as(*to_solved).first)
        //)
        //{

            return std::make_pair(from(), to_solved);
        //}
    }
    return std::nullopt;
}

void TypeSolver::add_type_variable(std::shared_ptr<Type> new_type)
{
    type_tree.add_root(new_type);
}

void TypeSolver::bind(std::shared_ptr<Type> type,
                      std::shared_ptr<Type> depend_on,
                      const std::shared_ptr<location::Location> location,
                      bool complete_union_type)
{
    //if either type is not unsolved type, put it into top of the tree so that we can get solved type by access top of the tree.
    type_tree.unite(type, depend_on);
    auto root = type_tree.get_root(type);
    bool root_swapped = false;

    if (!type->is_type<UnsolvedType>())
    {
        if (type->is_type<UnionType>() && complete_union_type)
        {
            union_types.insert(root);
        }
        root->swap(*type);
        if (!type->is_type<UnionType>())
        {
            union_types.erase(type);
        }
        root_swapped = true;
    }

    if (!depend_on->is_type<UnsolvedType>())
    {
        if (depend_on->is_type<UnionType>() && complete_union_type)
        {
            union_types.insert(root);
            root->swap(*depend_on);
            if (!depend_on->is_type<UnionType>())
            {
                union_types.erase(depend_on);
            }
        }
        else
        {
            root->swap(*depend_on);
        }
    }

    if (type->is_type<UnionType>() && complete_union_type)
    {
        union_types.insert(type);
    }

    if (depend_on->is_type<UnionType>() && complete_union_type)
    {
        union_types.insert(depend_on);
    }

    history.push_back(TypeBinding(type, depend_on, location));
}
void TypeSolver::complete_union_types(std::vector<std::shared_ptr<Type>>& types)
{
    // for each union type, binding the common tagged type.
    //[Union{A:?,B:String},Union{A:Int,B:?}]->[Union{A:Int,B:String},Union{A:Int,B:String}]

    auto dummy_location = std::make_shared<location::Location>(
        "[solver]", location::TokenInfo(0, 0, ""),
        hierarchy::Hierarchy::create("[solver]"));

    for (auto type : types)
    {
        if (type->is_type<UnionType>())
        {
            auto& union_type = type->as<UnionType>();
            auto tags = union_type.get_solved_tags();
            for (auto& tag : tags)
            {
                if (tag.second->is_type<UnsolvedType>())
                {
                    for (auto type_ : types)
                    {
                        if (!type_->is_type<UnionType>() ||
                            (type->get_id() == type_->get_id()))
                        {
                            continue;
                        }
                        auto to_bind =
                            type_->as<UnionType>().get_solved_tags().at(
                                tag.first);
                        bind(tag.second, to_bind, dummy_location);
                    }
                }
            }
        }
    }
}

std::shared_ptr<Type> TypeSolver::solve(std::shared_ptr<Type> type,
                                        bool complete_union_type)
{
    auto root = type_tree.get_root(type);

    if (root->is_type<UnionType>())
    // initialization of union type doesn't have complete type information (only
    // has one type of a tag) so it needs be complemeted.
    // Union{A,B,C}; u = U#A(0) <-u is Union{A:Int,B:?,C:?} (tag B and C are
    // unknown)
    {
        auto& got_union_type = root->as<UnionType>();
        std::map<std::string, std::shared_ptr<Type>> unsolved_tags;
        for (auto& tag : got_union_type.get_tags())
        {
            if (tag.second->is_type<UnsolvedType>())
            {
                unsolved_tags.insert(tag);
            }
        }

        std::map<TypeID, std::vector<std::shared_ptr<Type>>> same_roots;

        for (auto& union_type : union_types)
        {
            auto root_of_union_type = type_tree.get_root(union_type);
            same_roots[root_of_union_type->get_id()].push_back(union_type);
        }

        if (complete_union_type)
            for (auto& same_root_types : same_roots)
            {
                complete_union_types(same_root_types.second);
            }
    }

    return root;
}

void TypeSolver::register_named_type(const std::string& name,
                                     std::shared_ptr<Type> type)
{
    named_types[name] = type;
}

void TypeSolver::register_type(std::shared_ptr<Type> type)
{
    id_type_map.insert(std::make_pair(type->get_basic_info().get_id(), type));
}

std::shared_ptr<Type> TypeSolver::get_type_by_id(
    size_t id) const  // need to be considered
{
    return id_type_map.at(id);
}

const std::unordered_map<std::string, std::shared_ptr<Type>>&
TypeSolver::get_named_types() const
{
    return named_types;
}

const TypeSolver::TypeTree& TypeSolver::get_all_types() { return type_tree; }

const std::vector<TypeBinding>& TypeSolver::get_history() const
{
    return history;
}

std::optional<std::shared_ptr<Type>> TypeSolver::get_binded_type(TypeID type_id)
{
    for (auto& binding : history)
    {
        if (binding.from()->get_basic_info().get_id() == type_id)
        {
            return binding.to();
        }
    }
    return std::nullopt;
}

void TypeSolver::remap(const ClonedTypeMap& cloned_type_map,
                       std::shared_ptr<location::Location> location)
{
    auto history = get_history();
    std::vector<OriginalTypeID> keys_in_cloned_type_map;
    for (auto& pair : cloned_type_map)
    {
        keys_in_cloned_type_map.push_back(pair.first);
    }

    for (auto key : keys_in_cloned_type_map)
    {
        if (cloned_type_map.count(key) <= 1)
        {
            continue;
        }
        auto range = cloned_type_map.equal_range(
            key);  // Types in this range are cloned from same type.
        auto previous_pair = range.first;

        for (auto pair = range.first; pair != range.second; pair++)
        {
            if (pair == previous_pair)
            {
                continue;
            }
            bind(get_type_by_id(previous_pair->second),
                 get_type_by_id(pair->second), location);
            previous_pair = pair;
        }
    }

    for (auto& binding : history)
    {
        auto from_id = binding.from()->get_basic_info().get_id();
        auto to_id = binding.to()->get_basic_info().get_id();

        if (cloned_type_map.count(from_id) && cloned_type_map.count(to_id))
        {
            auto cloned_from_ids = cloned_type_map.get_values(from_id);
            auto cloned_to_ids = cloned_type_map.get_values(to_id);

            for (auto cloned_from_id : cloned_from_ids)
            {
                for (auto cloned_to_id : cloned_to_ids)
                {
                    bind(get_type_by_id(cloned_from_id),
                         get_type_by_id(cloned_to_id), binding.get_location());
                }
            }
        }
    }
}

const TypeEnvironment& TypeSolver::get_environment() const
{
    return environment;
}

}  // namespace clawn::requirement