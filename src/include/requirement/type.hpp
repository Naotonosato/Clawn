#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <set>
#include <memory>
#include <variant>
#include <iostream>
#include "include/utils/pattern_match.hpp"
#include "include/utils/unique_number.hpp"
#include "include/requirement/type_solver.hpp"
#include "include/utils/variant_wrapper.hpp"

namespace clawn::requirement
{
class Type;
class ClonedTypeMap;
// forward declarations

using TypeID = size_t;

class TypeBase
{
    private:
    std::string name;
    TypeID id;

    protected:
    std::weak_ptr<Type> container;
    TypeBase(const std::string& name)
        : name(name), id(utils::get_unique_number())
    {
    }
    TypeBase(const TypeBase&) = delete;
    TypeBase(TypeBase&&) noexcept = default;
    TypeBase& operator=(const TypeBase&) = delete;
    TypeBase& operator=(TypeBase&&) noexcept = default;

    public:
    const std::string& get_name() const;
    std::string to_string(bool debug=false) const   ;
    const TypeID get_id() const;
};

using OriginalTypeID = TypeID;
using ClonedTypeID = TypeID;

// map<ID(original),ID(Clone)>
class ClonedTypeMap
    : public std::unordered_multimap<OriginalTypeID, ClonedTypeID>
{
    ClonedTypeMap(const ClonedTypeMap&) = delete;

    public:
    ClonedTypeMap() = default;
    std::vector<ClonedTypeID> get_values(OriginalTypeID) const;
};

class VoidType : public TypeBase
{
    protected:
    VoidType() : TypeBase("Void") {}
};

enum class CPrimitiveType
{
    BOOL,
    CHAR,
    DOUBLE,
    FLOAT,
    INT,
    LONG,
    LONGDOUBLE,
    LONGLONG,
    SCHAR,
    SHORT,
    UCHAR,
    UINT,
    ULONG,
    ULONGLONG,
    USHORT,
    WCHAR,
    UNKNOWN
};

class CType : public TypeBase
{
    public:
    struct TypeInfo
    {
        CPrimitiveType kind;
        size_t size_in_bytes;
        TypeInfo() = delete;
        TypeInfo(CPrimitiveType kind, size_t size_in_bytes)
            : kind(kind), size_in_bytes(size_in_bytes)
        {
        }
    };

    private:
    TypeInfo info;

    protected:
    CType(TypeInfo info)
        : TypeBase("CType" + std::to_string(int(info.kind))), info(info)
    {
    }

    public:
    const TypeInfo& get_info() const;
    std::string to_string(bool debug=false) const;
};

class BooleanType : public TypeBase
{
    protected:
    BooleanType() : TypeBase("boolean") {}
};

class IntegerType : public TypeBase
{
    protected:
    IntegerType() : TypeBase("Integer") {}
};

class RealNumberType : public TypeBase
{
    protected:
    RealNumberType() : TypeBase("RealNumber") {}
};

class ReferenceType : public TypeBase
{
    private:
    std::shared_ptr<Type> reference_to;

    protected:
    ReferenceType(std::shared_ptr<Type> reference_to)
        : TypeBase("Reference"), reference_to(reference_to)
    {
    }

    public:
    std::string to_string(bool debug=false) const;
    const std::shared_ptr<Type> get_reference_to() const;
    const std::shared_ptr<Type> get_reference_to_without_solving() const;
};

class ListType : public TypeBase
{
    private:
    std::shared_ptr<Type> element_type;

    protected:
    ListType(std::shared_ptr<Type> element_type)
        : TypeBase("list"), element_type(element_type)
    {
    }

    public:
    std::string to_string(bool debug=false) const;
    const std::shared_ptr<Type> get_element_type() const;
};

class UnionType : public TypeBase
{
    private:
    std::map<std::string, std::shared_ptr<Type>> tags;
    bool _by_compiler;  // this is not good idea

    protected:
    UnionType(const std::string& name,
              std::map<std::string, std::shared_ptr<Type>>& tags,
              std::shared_ptr<TypeSolver> solver, bool _by_compiler = false);

    public:
    std::string to_string(bool debug=false) const;
    const std::map<std::string, std::shared_ptr<Type>>& get_tags() const;

    const std::map<std::string, std::shared_ptr<Type>> get_solved_tags() const;

    unsigned int get_index(const std::string& member_name) const;
    unsigned int get_tag_info_index() const;
    std::shared_ptr<Type> get_tag_info_type() const;
    std::shared_ptr<Type> get_element_type(unsigned int index) const;
    bool _is_by_compiler() const;  // maybe this is not good idea
    std::pair<bool,std::optional<std::shared_ptr<Type>>> _can_be_treated_as(const Type& type) const; //maybe this will be duplicated because verifier ignores union type(by compiler).
};

class StructureType : public TypeBase
{
    public:
    using MemberType = std::pair<std::string, std::shared_ptr<Type>>;
    using MemberTypes = std::vector<MemberType>;

    private:
    MemberTypes member_types;

    protected:
    StructureType(const std::string& name, MemberTypes& member_types)
        : TypeBase(name), member_types(member_types)
    {
    }

    public:
    std::string to_string(bool debug=false) const;
    const MemberTypes& get_member_types_without_solving() const;
    const MemberTypes get_member_types() const;
    unsigned int get_index(const std::string& member_name) const;
    std::shared_ptr<Type> get_element_type(unsigned int index) const;
    std::shared_ptr<Type> get_element_type(
        const std::string& member_name) const;
    std::shared_ptr<Type> get_element_type_without_solving(
        const std::string& member_name) const;
    bool has_member_named(const std::string& member_name) const;
};

class FunctionType : public TypeBase
{
    private:
    std::vector<std::shared_ptr<Type>> argument_types;
    std::vector<std::string> argument_names;
    std::shared_ptr<Type> return_type;
    bool _is_polymorphic;

    protected:
    FunctionType(const std::string& name,
                 std::vector<std::shared_ptr<Type>>&& argument_types,
                 std::vector<std::string> argument_names,
                 std::shared_ptr<Type> return_type, bool is_polymorphic = true)
        : TypeBase(name),
          argument_types(std::move(argument_types)),
          argument_names(argument_names),
          return_type(return_type),
          _is_polymorphic(is_polymorphic)
    {
    }
    FunctionType(const std::string& name,
                 std::vector<std::shared_ptr<Type>>& argument_types,
                 std::vector<std::string> argument_names,
                 std::shared_ptr<Type> return_type, bool is_polymorphic = true)
        : TypeBase(name),
          argument_types(argument_types),
          argument_names(argument_names),
          return_type(return_type),
          _is_polymorphic(is_polymorphic)
    {
    }

    public:
    std::string to_string(bool debug=false) const;

    const std::vector<std::shared_ptr<Type>>&
    get_argument_types_without_solving() const;

    const std::vector<std::shared_ptr<Type>> get_argument_types() const;

    const std::vector<std::string>& get_argument_names() const;

    std::shared_ptr<Type> get_return_type_without_solving() const;

    std::shared_ptr<Type> get_return_type() const;

    bool is_polymorphic() const;
};

class UnsolvedType : public TypeBase
{
    protected:
    UnsolvedType() : TypeBase("unsolved") {}
    UnsolvedType(const std::string& name) : TypeBase(name) {}
};

class Type
    : public utils::VariantWrapper<VoidType, CType, BooleanType, IntegerType,
                                   RealNumberType, ReferenceType, ListType,
                                   UnionType, StructureType, FunctionType,
                                   UnsolvedType>
{
    public:
    using parent_type =
        utils::VariantWrapper<VoidType, CType, BooleanType, IntegerType,
                              RealNumberType, ReferenceType, ListType,
                              UnionType, StructureType, FunctionType,
                              UnsolvedType>;

    private:
    const std::shared_ptr<TypeSolver> solver;

    protected:
    template <class T>
    Type(const std::shared_ptr<TypeSolver> solver, T&& content)
        : parent_type(std::move(content)), solver(solver)
    {
    }

    struct Visitor
    {
        template <typename T>
        std::string operator()(T& arg) const
        {
            return arg.get_name();
        }
    };

    public:
    std::string get_name() const;

    TypeID get_id() const;

    const TypeBase& get_basic_info() const;

    const std::shared_ptr<TypeSolver> get_type_solver() const;

    void swap(Type& other);

    template <class T = UnsolvedType, typename... Args>
    static std::shared_ptr<Type> create(
        const std::shared_ptr<TypeSolver> solver, Args&&... args)
    {
        auto body = CreateHelper<T>(std::forward<Args>(args)...);
        auto type =
            std::make_shared<CreateHelper<Type>>(solver, std::move(body));
        solver->add_type_variable(type);
        solver->register_type(type);
        return type;
    }

    template <typename VisitorType>
    auto accept(VisitorType& visitor) const
    {
        return std::visit(visitor, content);
    }

    template <typename TypeKind>
    bool is_type() const
    {
        return std::holds_alternative<TypeKind>(content);
    }
    bool operator==(const Type& other) = delete;
    // second parameter swithces how this function determines equality of types.
    // if it's true, any unsolved type is determined to be same as any type.
    // third parameter will be obsoluted.
    bool is_same_as(const Type& other, bool allow_unsolved_type = true,
                    bool strict_at_function = false) const; 
    static bool is_same_as(const std::vector<std::shared_ptr<Type>>& x,
                           const std::vector<std::shared_ptr<Type>>& y,
                           bool allow_unsolved_type = true,
                           bool strict_at_function = false);
    std::string to_string(bool debug = false) const;
    std::shared_ptr<Type> get_solved() const;
    std::shared_ptr<Type> clone(
            ClonedTypeMap&,
            std::optional<const std::string> new_name = std::nullopt
        ) const;
    std::shared_ptr<Type> get_pointer_to() const;
};

// Wrapper class for unordered_map that has requirement::TypeID as key.
// This class don't compare keys as ID value but as type equality.
// for example,
// t1 = IntType(id=0); t2= IntType(id=2); SearchableByType[t1] = "A"; (pseudo
// code) then, SearchableByType[t2] will return the same value as
// SearchableByType[t1] because of its type equality. performance is the issue.
// this takes O(N) for every search. This is so bad comapared to O(log N).
template <typename ValueType>
class SearchableByType
{
    private:
    std::unordered_map<std::shared_ptr<Type>, ValueType> data;

    public:
    SearchableByType() = default;
    bool contains(const std::shared_ptr<Type>& key) const
    {
        auto type_solver = key->get_type_solver();
        for (auto& pair : data)
        {
            if (type_solver->solve(pair.first)->is_same_as(*key, false))
            {
                return true;
            }
        }
        return false;
    }

    std::vector<std::shared_ptr<Type>> get_keys() const
    {
        std::vector<std::shared_ptr<Type>> keys;
        for (auto& pair : data)
        {
            keys.push_back(pair.first);
        }
        return keys;
    }

    ValueType& operator[](const std::shared_ptr<Type>& key)
    {
        auto type_solver = key->get_type_solver();
        for (auto& pair : data)
        {
            if (type_solver->solve(pair.first)->is_same_as(*key, false))
            {
                return pair.second;
            }
        }
        return data[type_solver->solve(key)];
    }
    const ValueType& at(const std::shared_ptr<Type>& key) const
    {
        auto type_solver = key->get_type_solver();
        for (auto& pair : data)
        {
            if (type_solver->solve(pair.first)->is_same_as(*key, false))
            {
                return pair.second;
            }
        }
        throw std::out_of_range(
            "key " + key->to_string() +
            " does not exist in container class 'SearchableByType'.");
    }

    auto insert(std::pair<std::shared_ptr<Type>, ValueType> pair)
    {
        return data.insert(pair);
    }
};

// Wrapper class for unordered_map that has std::vector<requirement::TypeID> as
// key. This class behaves like the multiple edition of SearchableByType class.
// This class is used for searching return type of a (generic) function from
// passed argument types. This takes O(NM) for every search.(M is size of key.)
template <typename ValueType>
class SearchableByTypes
{
    private:
    std::map<std::vector<std::shared_ptr<Type>>, ValueType> data;

    public:
    SearchableByTypes() = default;

    bool contains(const std::vector<std::shared_ptr<Type>>& key) const
    {
        for (auto& pair : data)
        {
            if (Type::is_same_as(pair.first, key,false))
            {
                return true;
            }
        }
        return false;
    }

    ValueType& operator[](const std::vector<std::shared_ptr<Type>>& key)
    {
        for (auto& pair : data)
        {
            if (Type::is_same_as(pair.first, key, false))
            {
                return pair.second;
            }
        }
        return data[key];
    }
    const ValueType& at(const std::vector<std::shared_ptr<Type>>& key) const
    {
        for (auto& pair : data)
        {
            if (Type::is_same_as(pair.first, key, false))
            {
                return pair.second;
            }
        }
        throw std::out_of_range(
            "key vector does not exist in container class "
            "'SearchableByTypes'.");
    }

    auto insert(std::pair<std::vector<std::shared_ptr<Type>>, ValueType> pair)
    {
        return data.insert(pair);
    }

    const std::map<std::vector<std::shared_ptr<Type>>, ValueType>& get_body() const
    {
        return data;
    }
};

class TypeSet
{
    private:
    std::unique_ptr<std::set<std::shared_ptr<Type>>> data;

    public:
    TypeSet() : data(std::make_unique<std::set<std::shared_ptr<Type>>>()) {}
    void insert(std::shared_ptr<Type> type);
    bool contains(std::shared_ptr<Type> type) const;
    const std::set<std::shared_ptr<Type>>& get_data() const;
};

class TypesVectorWithoutDuplicates
{
    private:
    std::vector<std::vector<std::shared_ptr<Type>>> data;

    public:
    TypesVectorWithoutDuplicates() : data(std::vector<std::vector<std::shared_ptr<Type>>>()) {}
    void insert(const std::vector<std::shared_ptr<Type>>& types);
    bool contains(const std::vector<std::shared_ptr<Type>>& types) const;
    const std::vector<std::vector<std::shared_ptr<Type>>>& get_data() const;
};

class TypeEnvironment
{
    public:
    using ArgumentTypes = std::vector<std::shared_ptr<Type>>;

    private:
    std::shared_ptr<TypeSolver> solver;
    std::unordered_map<std::string, std::shared_ptr<Type>> identifier_type_map;
    SearchableByType<TypesVectorWithoutDuplicates> instantiations;
    SearchableByType<SearchableByTypes<std::shared_ptr<Type>>>
        return_types;
    SearchableByType<std::unordered_map<std::string, std::shared_ptr<Type>>>
        structure_element_types;

    SearchableByType<std::shared_ptr<Type>> list_element_types;

    public:
    TypeEnvironment(const TypeEnvironment&) = delete;
    TypeEnvironment(TypeEnvironment&&) = default;
    TypeEnvironment();
    template <typename... Args>
    std::shared_ptr<Type> create_type(Args&&... args);
    std::shared_ptr<Type> get_type(TypeID);
    bool exists(const std::string& name);
    void add_identifier_and_type(const std::string& identifier,
                                 std::shared_ptr<Type> type);
    std::shared_ptr<Type> get_integer_type() const;
    std::shared_ptr<Type> get_float_type() const;
    std::shared_ptr<Type> get_boolean_type() const;
    std::shared_ptr<Type> get_type_by_identifier(
        const std::string& identifier) const;
    void register_instanciation(
        std::shared_ptr<Type> function_type,
        std::vector<std::shared_ptr<Type>>& argument_types);
    void register_element_type(std::shared_ptr<Type> from,
                               const std::string& element_name,
                               std::shared_ptr<Type> element_type);
    std::optional<std::shared_ptr<Type>> get_element_type(
        std::shared_ptr<Type> from, const std::string& element_name) const;
    void register_element_type(std::shared_ptr<Type> list_type,
                               std::shared_ptr<Type> element_type);
    std::optional<std::shared_ptr<Type>> get_element_type(
        std::shared_ptr<Type> list_type) const;
    void register_return_type(std::shared_ptr<Type> function_type,
                              const ArgumentTypes& argument_types,
                              std::shared_ptr<Type> return_type);
    std::optional<std::shared_ptr<Type>> get_return_type(
        std::shared_ptr<Type> function_type,
        const ArgumentTypes& argument_types) const;
    const SearchableByType<TypesVectorWithoutDuplicates>& get_instantiations()
        const;
    const TypesVectorWithoutDuplicates get_instantiations(
        const std::shared_ptr<Type> generics_type) const;
    std::shared_ptr<TypeSolver> get_solver() const;

    void dump_named_types();
};

template <class Visitor>
using TypeVisitor = utils::VisitorWrapper<Visitor, Type>;

}  // namespace clawn::requirement