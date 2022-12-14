#include <iostream>
#include <map>
#include <set>
#include "include/requirement/type_solver.hpp"
#include "include/requirement/type.hpp"
#include "include/utils/transform.hpp"

namespace clawn::requirement
{
std::vector<ClonedTypeID> ClonedTypeMap::get_values(OriginalTypeID key) const
{
    auto range = equal_range(key);
    std::vector<ClonedTypeID> result;
    for (auto& key_value_pair = range.first; key_value_pair != range.second;
         ++key_value_pair)
    {
        result.push_back(key_value_pair->second);
    }
    return result;
}

class TypeCloner : public utils::VisitorWrapper<TypeCloner, Type>
{
    std::shared_ptr<TypeSolver> type_solver;
    ClonedTypeMap& cloned_type_map;
    std::unordered_map<std::shared_ptr<Type>, std::shared_ptr<Type>>
        original_owner;
    std::optional<const std::string> new_name;

    public:
    TypeCloner(std::shared_ptr<TypeSolver> type_solver,
               ClonedTypeMap& cloned_type_map,
               std::optional<const std::string> new_name = std::nullopt)
        : type_solver(type_solver),
          cloned_type_map(cloned_type_map),
          new_name(new_name)
    {
    }
    std::shared_ptr<Type> visit(const VoidType& original) const
    {
        auto cloned = Type::create<VoidType>(type_solver);
        cloned_type_map.insert(
            std::make_pair(original.get_id(), cloned->get_id()));
        return cloned;
    }
    std::shared_ptr<Type> visit(const CType& original) const
    {
        auto cloned = Type::create<CType>(type_solver, original.get_info());
        cloned_type_map.insert(
            std::make_pair(original.get_id(), cloned->get_id()));
        return cloned;
    }
    std::shared_ptr<Type> visit(const BooleanType& original) const
    {
        auto cloned = Type::create<BooleanType>(type_solver);
        cloned_type_map.insert(
            std::make_pair(original.get_id(), cloned->get_id()));
        return cloned;
    }
    std::shared_ptr<Type> visit(const IntegerType& original) const
    {
        auto cloned = Type::create<IntegerType>(type_solver);
        cloned_type_map.insert(
            std::make_pair(original.get_id(), cloned->get_id()));
        return cloned;
    }

    std::shared_ptr<Type> visit(const RealNumberType& original) const
    {
        auto cloned = Type::create<RealNumberType>(type_solver);
        cloned_type_map.insert(
            std::make_pair(original.get_id(), cloned->get_id()));
        return cloned;
    }
    std::shared_ptr<Type> visit(const ReferenceType& original) const
    {
        auto cloned = Type::create<ReferenceType>(
            type_solver, original.get_reference_to_without_solving()->clone(
                             cloned_type_map));
        cloned_type_map.insert(
            std::make_pair(original.get_id(), cloned->get_id()));
        return cloned;
    }
    std::shared_ptr<Type> visit(const ListType& original) const
    {
        auto cloned = Type::create<ListType>(
            type_solver, original.get_element_type()->clone(cloned_type_map));
        cloned_type_map.insert(
            std::make_pair(original.get_id(), cloned->get_id()));
        return cloned;
    }

    std::shared_ptr<Type> visit(const UnionType& original) const
    {
        const auto& original_tags = original.get_tags();
        std::map<std::string, std::shared_ptr<Type>> cloned_tags;
        for (const auto& pair : original_tags)
        {
            cloned_tags.insert(std::make_pair(
                pair.first, pair.second->clone(cloned_type_map)));
        }
        auto cloned = Type::create<UnionType>(
            type_solver,
            new_name.has_value() ? new_name.value() : original.get_name(),
            cloned_tags, type_solver);
        cloned_type_map.insert(
            std::make_pair(original.get_id(), cloned->get_id()));
        return cloned;
    }
    std::shared_ptr<Type> visit(const StructureType& original) const
    {
        const auto& original_members =
            original.get_member_types_without_solving();
        auto cloned_members =
            utils::get_transformed(original_members, [this](const auto& pair) {
                return std::make_pair(pair.first,
                                      pair.second->clone(cloned_type_map));
            });
        auto cloned = Type::create<StructureType>(
            type_solver,
            new_name.has_value() ? new_name.value() : original.get_name(),
            cloned_members);
        cloned_type_map.insert(
            std::make_pair(original.get_id(), cloned->get_id()));
        return cloned;
    }
    std::shared_ptr<Type> visit(const FunctionType& original) const
    {
        auto cloned_argument_types = utils::get_transformed(
            original.get_argument_types_without_solving(),
            [this](const std::shared_ptr<Type>& t) {
                return t->clone(cloned_type_map);
            });
        std::unordered_map<std::string, std::shared_ptr<Type>>
            cloned_argument_names;

        auto cloned_result_type =
            original.get_return_type_without_solving()->clone(cloned_type_map);
        auto cloned = Type::create<FunctionType>(
            type_solver,
            new_name.has_value() ? new_name.value() : original.get_name(),
            cloned_argument_types, original.get_argument_names(),
            cloned_result_type, original.is_polymorphic());
        cloned_type_map.insert(
            std::make_pair(original.get_id(), cloned->get_id()));
        return cloned;
    }

    std::shared_ptr<Type> visit(const UnsolvedType& original) const
    {
        auto cloned = Type::create<UnsolvedType>(
            type_solver,
            new_name.has_value() ? new_name.value() : original.get_name());
        cloned_type_map.insert(
            std::make_pair(original.get_id(), cloned->get_id()));
        return cloned;
    }
};

const std::string& TypeBase::get_name() const { return name; }

const TypeID TypeBase::get_id() const { return id; }

const CType::TypeInfo& CType::get_info() const { return info; }

const std::shared_ptr<Type> ReferenceType::get_reference_to() const
{
    return reference_to->get_type_solver()->solve(reference_to);
}

const std::shared_ptr<Type> ReferenceType::get_reference_to_without_solving()
    const
{
    return reference_to;
}

const std::shared_ptr<Type> ListType::get_element_type() const
{
    return element_type;
}

UnionType::UnionType(const std::string& name,
                     std::map<std::string, std::shared_ptr<Type>>& tags_,
                     std::shared_ptr<TypeSolver> solver, bool _by_compiler)
    : TypeBase(name), tags(tags_), _by_compiler(_by_compiler)
{
    // if (!tags.empty())
    //{
    // union{A:int} will be treated as union{taginfo:int,A:int} so index
    // need to be added 1.
    // auto solver = tags.begin()->second->get_type_solver();
    auto tag_info_type = Type::create<IntegerType>(solver);
    tags.insert(std::make_pair("", tag_info_type));
    //}
}

const std::map<std::string, std::shared_ptr<Type>>& UnionType::get_tags() const
{
    return tags;
}

const std::map<std::string, std::shared_ptr<Type>> UnionType::get_solved_tags()
    const
{
    std::map<std::string, std::shared_ptr<Type>> solved_tags;
    for (auto&& pair : tags)
    {
        auto name = pair.first;
        auto type = pair.second;
        auto solver = type->get_type_solver();
        solved_tags[name] = solver->solve(type, false);
    }
    return solved_tags;
}

bool UnionType::_is_by_compiler() const { return _by_compiler; }

unsigned int UnionType::get_index(const std::string& member_name) const
{
    unsigned int index = 0;
    for (auto& pair : tags)
    {
        if (pair.first == member_name)
        {
            return index;
        }
        index += 1;
    }
    throw std::runtime_error("There's no such element: " + member_name);
}

std::shared_ptr<Type> UnionType::get_element_type(unsigned int index) const
{
    unsigned int index_ = 0;
    for (auto& pair : tags)
    {
        if (index_ == index)
        {
            return pair.second->get_type_solver()->solve(pair.second);
        }
        index_ += 1;
    }
    throw std::out_of_range("index out of range: " + std::to_string(index));
}

unsigned int UnionType::get_tag_info_index() const { return 0; }

std::shared_ptr<Type> UnionType::get_tag_info_type() const
{
    return tags.at("");
}

const StructureType::MemberTypes&
StructureType::get_member_types_without_solving() const
{
    return member_types;
}

const StructureType::MemberTypes StructureType::get_member_types() const
{
    MemberTypes solved_member_types;
    for (auto&& pair : member_types)
    {
        auto& name = pair.first;
        auto& type = pair.second;
        solved_member_types.push_back(std::make_pair(name, type->get_solved()));
    }
    return solved_member_types;
}

unsigned int StructureType::get_index(const std::string& member_name) const
{
    unsigned int index = 0;
    for (auto& pair : member_types)
    {
        if (pair.first == member_name)
        {
            return index;
        }
        index += 1;
    }
    throw std::runtime_error("no such element: " + member_name);
}

std::shared_ptr<Type> StructureType::get_element_type(unsigned int index) const
{
    unsigned int index_ = 0;
    for (auto& pair : member_types)
    {
        if (index_ == index)
        {
            return pair.second;
        }
        index_ += 1;
    }
    throw std::out_of_range("element out of range: index " +
                            std::to_string(index));
}

bool StructureType::has_member_named(const std::string& member_name) const
{
    for (auto& pair : member_types)
    {
        if (pair.first == member_name)
        {
            return true;
        }
    }
    return false;
}

std::shared_ptr<Type> StructureType::get_element_type(
    const std::string& member_name) const
{
    for (auto& pair : get_member_types())
    {
        if (pair.first == member_name)
        {
            return pair.second;
        }
    }
    throw std::runtime_error("There's no such member: '" + member_name +
                             "' in " + get_name());
}

std::shared_ptr<Type> StructureType::get_element_type_without_solving(
    const std::string& member_name) const
{
    for (auto& pair : member_types)
    {
        if (pair.first == member_name)
        {
            return pair.second;
        }
    }
    throw std::runtime_error("There's no such member: '" + member_name +
                             "' in " + get_name());
}

const std::vector<std::shared_ptr<Type>>&
FunctionType::get_argument_types_without_solving() const
{
    return argument_types;
}

const std::vector<std::shared_ptr<Type>> FunctionType::get_argument_types()
    const
{
    std::vector<std::shared_ptr<Type>> solved_argument_types;

    for (auto& type_ : argument_types)
    {
        auto solver = type_->get_type_solver();
        solved_argument_types.push_back(solver->solve(type_));
    }

    return solved_argument_types;
}

const std::vector<std::string>& FunctionType::get_argument_names() const
{
    return argument_names;
}

std::shared_ptr<Type> FunctionType::get_return_type_without_solving() const
{
    return return_type;
}

std::shared_ptr<Type> FunctionType::get_return_type() const
{
    return return_type->get_type_solver()->solve(return_type);
}

bool FunctionType::is_polymorphic() const { return _is_polymorphic; }

void Type::swap(Type& other) { content.swap(other.content); }

const TypeBase& Type::get_basic_info() const
{
    return std::visit(
        [](const auto& sometype) -> const TypeBase& { return sometype; },
        content);
}

const std::shared_ptr<TypeSolver> Type::get_type_solver() const
{
    return solver;
}

std::string Type::get_name() const { return get_basic_info().get_name(); }

TypeID Type::get_id() const { return get_basic_info().get_id(); }

bool Type::is_same_as(const Type& other, bool allow_unsolved_type,
                      bool strict_at_function) const
{
    if (get_id() == other.get_id())
    {
        return true;
    }
    return utils::Match{this->get_variant()}(
        utils::Type<ListType>(),
        [&other, allow_unsolved_type,
         strict_at_function](const ListType& self) {
            return utils::Match{other.get_variant()}(
                utils::Type<ListType>(),
                [&self, allow_unsolved_type,
                 strict_at_function](const ListType& other_unboxed) {
                    return self.get_element_type()->is_same_as(
                        *other_unboxed.get_element_type(), allow_unsolved_type,
                        strict_at_function);
                },
                utils::Default(), [](const auto&) { return false; });
        },

        utils::Type<CType>(),
        [&other, allow_unsolved_type, strict_at_function](const CType& self) {
            return utils::Match{other.get_variant()}(
                utils::Type<CType>(),
                [&self, allow_unsolved_type,
                 strict_at_function](const CType& other_unboxed) {
                    return self.get_info().kind ==
                           other_unboxed.get_info().kind;
                },
                utils::Type<IntegerType>(),
                [&self](const auto&) {
                    return self.get_info().kind == CPrimitiveType::LONGLONG;
                },
                utils::Type<RealNumberType>(),
                [&self](const auto&) {
                    return self.get_info().kind == CPrimitiveType::DOUBLE;
                },
                utils::Default(), [](const auto&) { return false; });
        },

        utils::Type<FunctionType>(),
        [&other, allow_unsolved_type,
         strict_at_function](const FunctionType& self) {
            return utils::Match{other.get_variant()}(
                utils::Type<FunctionType>(),
                [&self, allow_unsolved_type,
                 strict_at_function](const FunctionType& other_unboxed) {
                    // check names are same. -> No! there is no need to be equal
                    // -> No No! functions are typed nominally
                    if (self.get_name() == other_unboxed.get_name())
                    {
                        return true;
                    }
                    if (self.get_name() != other_unboxed.get_name())
                    {
                        return false;
                    }
                    auto self_argument_types = self.get_argument_types();
                    auto other_argument_types =
                        other_unboxed.get_argument_types();
                    // check arguments size are same.
                    if (self_argument_types.size() !=
                        other_argument_types.size())
                    {
                        return false;
                    }
                    // check each types are same.
                    for (size_t i = 0; i < self_argument_types.size(); i += 1)
                    {
                        auto argument_type = self_argument_types[i];
                        auto other_argument_type = other_argument_types[i];
                        if (!argument_type->is_same_as(*other_argument_type,
                                                       allow_unsolved_type,
                                                       !strict_at_function))
                        {
                            return false;
                        }
                    }
                    // check return types are same.
                    if (!self.get_return_type()->is_same_as(
                            *other_unboxed.get_return_type(),
                            allow_unsolved_type, !strict_at_function))
                    {
                        return false;
                    }
                    return true;
                },
                utils::Type<UnsolvedType>(),
                [allow_unsolved_type](const auto&) {
                    return allow_unsolved_type;
                },
                utils::Default(), [](const auto&) { return false; });
        },
        utils::Type<StructureType>(),
        [&other, allow_unsolved_type,
         strict_at_function](const StructureType& self) {
            return utils::Match{other.get_variant()}(
                utils::Type<StructureType>(),
                [&self, allow_unsolved_type,
                 strict_at_function](const StructureType& other_unboxed) {
                    // check names are same.
                    if (self.get_name() != other_unboxed.get_name())
                    {
                        return false;
                    }
                    auto self_member_types = self.get_member_types();
                    auto other_member_types = other_unboxed.get_member_types();
                    // check member size are same.
                    if (self_member_types.size() != other_member_types.size())
                    {
                        return false;
                    }
                    // check each types are same.
                    for (auto& name_type_pair : self_member_types)
                    {
                        auto& arg_name = name_type_pair.first;
                        auto& arg_type = name_type_pair.second;
                        bool has_arg = other_unboxed.has_member_named(arg_name);
                        if (!has_arg)
                        {
                            return false;
                        }
                        if (!arg_type->is_same_as(
                                *other_unboxed.get_element_type(arg_name),
                                allow_unsolved_type, strict_at_function))
                        {
                            return false;
                        }
                    }
                    return true;
                },
                utils::Type<UnsolvedType>(),
                [allow_unsolved_type](const auto&) {
                    return allow_unsolved_type;
                },
                utils::Default(), [](const auto&) { return false; });
        },
        utils::Type<ReferenceType>(),
        [&other, allow_unsolved_type,
         strict_at_function](const ReferenceType& self) {
            return utils::Match{other.get_variant()}(
                utils::Type<ReferenceType>(),
                [&self, allow_unsolved_type,
                 strict_at_function](const ReferenceType& other_unboxed) {
                    return self.get_reference_to()->is_same_as(
                        *other_unboxed.get_reference_to(), allow_unsolved_type,
                        strict_at_function);
                },
                utils::Type<UnsolvedType>(),
                [allow_unsolved_type](const auto&) {
                    return allow_unsolved_type;
                },
                utils::Default(), [](const auto&) { return false; });
        },
        utils::Type<UnionType>(),
        [&other, allow_unsolved_type,
         strict_at_function](const UnionType& self) {
            return utils::Match{other.get_variant()}(
                utils::Type<UnionType>(),
                [&self, allow_unsolved_type,
                 strict_at_function](const UnionType& other_unboxed) {
                    auto self_tags = self.get_solved_tags();
                    auto other_tags = other_unboxed.get_solved_tags();
                    if (self_tags.size() != other_tags.size())
                    {
                        return false;
                    }
                    for (auto& name_type_pair : self_tags)
                    {
                        auto tag_name = name_type_pair.first;
                        auto tag_type = name_type_pair.second;
                        if (other_tags.count(tag_name) == 0)
                        {
                            return false;
                        }
                        if (!tag_type->is_same_as(*other_tags[tag_name],
                                                  allow_unsolved_type,
                                                  strict_at_function))
                        {
                            return false;
                        }
                    }
                    return true;
                },
                utils::Type<UnsolvedType>(),
                [allow_unsolved_type](const auto&) {
                    return allow_unsolved_type;
                },
                utils::Default(), [](const auto&) { return false; });
        },
        utils::Type<UnsolvedType>(),
        [&other, allow_unsolved_type, this](const UnsolvedType& self) {
            if (allow_unsolved_type)
            {
                return true;
            }
            else
            {
                if (get_solved()->get_id() == other.get_solved()->get_id())
                {
                    return true;
                }

                if (get_solved()->is_type<UnsolvedType>() &&
                    other.get_solved()->is_type<UnsolvedType>())
                {
                    return (get_solved()->get_id() ==
                            other.get_solved()->get_id());
                }

                if ((!get_solved()->is_type<UnsolvedType>()) &&
                    (!other.get_solved()->is_type<UnsolvedType>()))
                {
                    return get_solved()->is_same_as(*other.get_solved(),
                                                    allow_unsolved_type);
                }

                return false;

                /*
                auto solver = get_type_solver();
                return solver->solve(solver->get_type_by_id(get_id()))
                           ->get_id() ==
                       solver->solve(solver->get_type_by_id(other.get_id()))
                           ->get_id();
                */
            }
        },
        utils::Default(),
        [&other, allow_unsolved_type](const variant_type& self_variant) {
            if (allow_unsolved_type &&
                other.is_type<requirement::UnsolvedType>())
            {
                return true;
            }
            return self_variant.index() == other.get_variant().index();
        });
}
bool Type::is_same_as(const std::vector<std::shared_ptr<Type>>& x,
                      const std::vector<std::shared_ptr<Type>>& y,
                      bool allow_unsolved_type, bool strict_at_function)
{
    if (x.size() != y.size())
    {
        return false;
    }
    for (size_t i = 0; i < x.size(); i += 1)
    {
        if (!x[i]->is_same_as(*y[i], allow_unsolved_type, strict_at_function))
        {
            return false;
        }
    }
    return true;
}
std::string Type::to_string(bool debug) const
{
    auto debug_info = [debug](const auto& type) {
        return debug ? "@" + std::to_string(type.get_id()) : "";
    };
    // std::cout << get_id() << std::endl;
    return utils::Match{this->get_variant()}(
        utils::Type<ListType>(),
        [debug, debug_info](const ListType& self) {
            return "array<" + self.get_element_type()->to_string(debug) + ">" +
                   debug_info(self);
        },
        utils::Type<FunctionType>(),
        [this, debug, debug_info](const FunctionType& self) {
            std::string arguments_types_string;
            for (auto& argument_type : self.get_argument_types())
            {
                arguments_types_string +=
                    argument_type->to_string(debug) + ", ";
            }
            if (!arguments_types_string.empty())
            {
                arguments_types_string = arguments_types_string.substr(
                    0, arguments_types_string.size() - 2);
            }
            std::string return_type_string =
                self.get_return_type()->to_string(debug);
            return self.get_name() + "<(" + arguments_types_string + ")->" +
                   return_type_string + ">" + debug_info(self);
        },
        utils::Type<StructureType>(),
        [this, debug, debug_info](const StructureType& self) {
            std::string members_types_string;
            for (auto& member_type : self.get_member_types())
            {
                auto member_name = member_type.first;
                members_types_string += member_name + ":" +
                                        member_type.second->to_string(debug) +
                                        ", ";
            }
            if (!members_types_string.empty())
            {
                members_types_string = members_types_string.substr(
                    0, members_types_string.size() - 2);
            }
            return "type " + self.get_name() + "{" + members_types_string +
                   "}" + debug_info(self);
        },
        utils::Type<ReferenceType>(),
        [this, debug, debug_info](const ReferenceType& self) {
            return "reference<" + self.get_reference_to()->get_name() + ">" +
                   debug_info(self);
        },
        utils::Type<UnionType>(),
        [this, debug, debug_info](const UnionType& self) {
            std::string tags_string;
            for (auto& tag : self.get_solved_tags())
            {
                auto tag_name = tag.first;
                auto tag_type_name = tag.second->to_string(debug);
                tags_string += tag_name + ":" + tag_type_name + ", ";
            }
            if (!tags_string.empty())
            {
                tags_string = tags_string.substr(0, tags_string.size() - 2);
            }
            return "union " + self.get_name() + "{" + tags_string + "}" +
                   debug_info(self);
        },
        utils::Default(),
        [this, debug, debug_info](const variant_type& self) {
            return get_name() +
                   (debug ? "@" + std::to_string(std::visit(
                                      [](const auto& v) { return v.get_id(); },
                                      self))
                          : "");
        });
}

std::shared_ptr<Type> Type::get_solved() const
{
    return get_type_solver()->solve(
        get_type_solver()->get_type_by_id(get_id()));
}

std::shared_ptr<Type> Type::get_pointer_to() const
{
    return create<ReferenceType>(get_type_solver(),
                                 get_type_solver()->get_type_by_id(get_id()));
}

std::shared_ptr<Type> Type::clone(
    ClonedTypeMap& cloned_type_map,
    std::optional<const std::string> new_name) const
{
    const auto cloner = TypeCloner(solver, cloned_type_map, new_name);
    solver->solve(solver->get_type_by_id(get_id()))->accept(cloner);
    auto binded_type = solver->get_binded_type(get_id());
    if (binded_type.has_value())
    {
        binded_type.value()->accept(cloner);
    }
    return this->accept(cloner);
}

void TypeSet::insert(std::shared_ptr<Type> type)
{
    if (contains(type))
    {
        return;
    }
    else
    {
        data->insert(type);
    }
}

bool TypeSet::contains(std::shared_ptr<Type> type) const
{
    for (auto& t : *data)
    {
        if (t->is_same_as(*type, false))
        {
            return true;
        }
    }
    return false;
}
const std::set<std::shared_ptr<Type>>& TypeSet::get_data() const
{
    return *data;
}

TypeEnvironment::TypeEnvironment() : solver(std::make_shared<TypeSolver>(*this))
{
    add_identifier_and_type("[GLOBAL]/Integer",
                            Type::create<IntegerType>(solver));
    add_identifier_and_type("[GLOBAL]/RealNumber",
                            Type::create<RealNumberType>(solver));
    add_identifier_and_type("[GLOBAL]/Boolean",
                            Type::create<BooleanType>(solver));
}

bool TypeEnvironment::exists(const std::string& name)
{
    return identifier_type_map.count(name);
}

std::shared_ptr<requirement::TypeSolver> TypeEnvironment::get_solver() const
{
    return solver;
}

void TypeEnvironment::add_identifier_and_type(
    const std::string& identifier, std::shared_ptr<requirement::Type> type)
{
    identifier_type_map.insert(std::make_pair(identifier, type));
}

std::shared_ptr<Type> TypeEnvironment::get_integer_type() const
{
    return get_type_by_identifier("[GLOBAL]/Integer");
}

std::shared_ptr<Type> TypeEnvironment::get_float_type() const
{
    return get_type_by_identifier("[GLOBAL]/RealNumber");
}

std::shared_ptr<Type> TypeEnvironment::get_boolean_type() const
{
    return get_type_by_identifier("[GLOBAL]/Boolean");
}

std::shared_ptr<requirement::Type> TypeEnvironment::get_type_by_identifier(
    const std::string& identifier) const
{
    return identifier_type_map.at(identifier);
}

void TypeEnvironment::register_instanciation(
    std::shared_ptr<requirement::Type> function_type,
    std::vector<std::shared_ptr<requirement::Type>>& arguments_types)
{
    instantiations[function_type].push_back(arguments_types);
}

void TypeEnvironment::register_element_type(std::shared_ptr<Type> from,
                                            const std::string& element_name,
                                            std::shared_ptr<Type> element_type)
{
    structure_element_types[from].insert(
        std::make_pair(element_name, element_type));
}

std::optional<std::shared_ptr<Type>> TypeEnvironment::get_element_type(
    std::shared_ptr<Type> from, const std::string& element_name) const
{
    if (structure_element_types.contains(from) &&
        structure_element_types.at(from).count(element_name))
    {
        return structure_element_types.at(from).at(element_name);
    }
    return std::nullopt;
}

void TypeEnvironment::register_element_type(std::shared_ptr<Type> list_type,
                                            std::shared_ptr<Type> element_type)
{
    list_element_types.insert(std::make_pair(list_type, element_type));
}

std::optional<std::shared_ptr<Type>> TypeEnvironment::get_element_type(
    std::shared_ptr<Type> list_type) const
{
    if (list_element_types.contains(list_type))
    {
        return list_element_types.at(list_type);
    }
    return std::nullopt;
}

void TypeEnvironment::register_return_type(std::shared_ptr<Type> function_type,
                                           const ArgumentTypes& argument_types,
                                           std::shared_ptr<Type> return_type)
{
    return_types[function_type][argument_types] = return_type;
}

std::optional<std::shared_ptr<Type>> TypeEnvironment::get_return_type(
    std::shared_ptr<Type> function_type,
    const ArgumentTypes& argument_types) const
{
    if (return_types.contains(function_type) &&
        return_types.at(function_type).contains(argument_types))
    {
        return get_solver()->solve(
            return_types.at(function_type).at(argument_types));
    }
    return std::nullopt;
}

const SearchableByType<std::vector<TypeEnvironment::ArgumentTypes>>&
TypeEnvironment::get_instantiations() const
{
    return instantiations;
}

const std::vector<TypeEnvironment::ArgumentTypes>
TypeEnvironment::get_instantiations(
    const std::shared_ptr<requirement::Type> generics) const
{
    std::vector<std::vector<std::shared_ptr<requirement::Type>>> result;
    if (!instantiations.contains(generics))
    {
        return {};
    }
    auto found_candidates = instantiations.at(generics);
    auto result_set =
        std::set(found_candidates.begin(), found_candidates.end());
    auto result_unique =
        std::vector<std::vector<std::shared_ptr<requirement::Type>>>(
            result_set.begin(), result_set.end());  // There is no duplication.

    return result_unique;
}

void TypeEnvironment::dump_named_types()
{
    for (auto& pair : identifier_type_map)
    {
        std::cout << pair.first << ": " << pair.second->to_string()
                  << std::endl;
    }
}

}  // namespace clawn::requirement
