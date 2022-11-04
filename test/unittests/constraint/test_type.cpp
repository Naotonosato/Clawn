#include <vector>
#include <map>
#include "include/utils/pattern_match.hpp"
#include "include/location/location.hpp"
#include "include/requirement/type.hpp"
#include "include/requirement/type_solver.hpp"
#include <gtest/gtest.h>

using namespace clawn;

class TypeTest : public testing::Test
{
protected:
    std::shared_ptr<requirement::Type> integer_type;
    std::shared_ptr<requirement::Type> float_type;
    std::shared_ptr<requirement::TypeSolver> type_solver;
    std::shared_ptr<location::Location> dummy_location;

    virtual void SetUp()
    {
        requirement::TypeEnvironment environment;
        type_solver = environment.get_solver();
        integer_type = requirement::Type::create<requirement::IntegerType>(type_solver);
        float_type = requirement::Type::create<requirement::RealNumberType>(type_solver);
    }

    virtual void TearDown()
    {
        requirement::TypeEnvironment environment;
        type_solver = environment.get_solver();
        integer_type = requirement::Type::create<requirement::IntegerType>(type_solver);
        float_type = requirement::Type::create<requirement::RealNumberType>(type_solver);
    }
};

TEST_F(TypeTest, TypeEquality)
{
    auto t1 = requirement::Type::create<requirement::UnsolvedType>(type_solver);
    auto t2 = requirement::Type::create<requirement::IntegerType>(type_solver);
    ASSERT_TRUE(t1->is_same_as(*t2));
    ASSERT_TRUE(t2->is_same_as(*t1));
}

TEST_F(TypeTest, FunctionTypeEquality)
{
    auto t1 = requirement::Type::create<requirement::FunctionType>(
        type_solver,
        std::string("f"),
        std::vector<std::shared_ptr<requirement::Type>>({requirement::Type::create<requirement::UnsolvedType>(type_solver),requirement::Type::create<requirement::UnsolvedType>(type_solver)}),
        std::vector<std::string>({"x","y"}),
        requirement::Type::create<requirement::UnsolvedType>(type_solver)
        );
    auto t2 = requirement::Type::create<requirement::FunctionType>(
        type_solver,
        std::string("f"),
        std::vector<std::shared_ptr<requirement::Type>>({requirement::Type::create<requirement::UnsolvedType>(type_solver),requirement::Type::create<requirement::UnsolvedType>(type_solver)}),
        std::vector<std::string>({"x","y"}),
        requirement::Type::create<requirement::UnsolvedType>(type_solver)
        );
    ASSERT_TRUE(t1->is_same_as(*t2));
    ASSERT_TRUE(t2->is_same_as(*t1));
}

TEST_F(TypeTest, StructureTypeEquality)
{
    requirement::StructureType::MemberTypes member_types_1;
    member_types_1.push_back(std::make_pair("x",requirement::Type::create<requirement::UnsolvedType>(type_solver)));
    member_types_1.push_back(std::make_pair("y",requirement::Type::create<requirement::UnsolvedType>(type_solver)));
    requirement::StructureType::MemberTypes member_types_2;
    member_types_2.push_back(std::make_pair("x", requirement::Type::create<requirement::UnsolvedType>(type_solver)));
    member_types_2.push_back(std::make_pair("y", requirement::Type::create<requirement::UnsolvedType>(type_solver)));
    auto t1 = requirement::Type::create<requirement::StructureType>(
        type_solver,
        std::string("S1"),
        member_types_1
        );
    auto t2 = requirement::Type::create<requirement::StructureType>(
        type_solver,
        std::string("S1"),
        member_types_2
        );
    ASSERT_TRUE(t1->is_same_as(*t2));
    ASSERT_TRUE(t2->is_same_as(*t1));
}

TEST_F(TypeTest, UnionTypeEquality)
{
    std::map<std::string, std::shared_ptr<requirement::Type>> tag_types;
    tag_types["tag1"] = integer_type->get_pointer_to();
    auto union_type = requirement::Type::create<requirement::UnionType>(
                type_solver, "Union", tag_types, type_solver);
    ASSERT_TRUE(!union_type->is_same_as(*integer_type->get_pointer_to()));
}

TEST_F(TypeTest, SearchableByType)
{
    auto t1 = requirement::Type::create<requirement::UnsolvedType>(type_solver);
    auto t2 = requirement::Type::create<requirement::UnsolvedType>(type_solver);
    requirement::SearchableByType<std::string> map;
    map[t1] = "linked to t1";
    type_solver->bind(t1, t2, dummy_location);
    ASSERT_EQ(map[t1], map[t2]);
}

TEST_F(TypeTest, SearchableByTypeVector)
{
    auto t1 = requirement::Type::create<requirement::UnsolvedType>(type_solver);
    auto t2 = requirement::Type::create<requirement::UnsolvedType>(type_solver);
    auto t3 = requirement::Type::create<requirement::UnsolvedType>(type_solver);
    auto t4 = requirement::Type::create<requirement::UnsolvedType>(type_solver);
    type_solver->bind(t3, t1, dummy_location);
    type_solver->bind(t4, t2, dummy_location);
    // t3 is t1, t4 is t2
    requirement::SearchableByTypeVector<std::string> map;
    map[{t1, t2}] = "linked to {t1,t2}";
    type_solver->bind(t1, t2, dummy_location);
    auto first = map[{t1, t2}];
    auto second = map[{t3, t4}];
    // map[{t1,t2}] should be equal to map[{t3,t4}]
    ASSERT_EQ(first, second);
}