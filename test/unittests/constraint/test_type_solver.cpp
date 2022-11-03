#include <vector>
#include <map>
#include "include/utils/pattern_match.hpp"
#include "include/location/location.hpp"
#include "include/requirement/type.hpp"
#include "include/requirement/type_solver.hpp"
#include <gtest/gtest.h>

using namespace clawn;

class TypeSolverTest: public testing::Test
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

TEST_F(TypeSolverTest,TypeContains)
{
    /*
    v = 0
    
    -> what is type of `v`?
    */
    ASSERT_TRUE(integer_type->is_type<requirement::IntegerType>());
}

TEST_F(TypeSolverTest,MultipleAssignment)
{
    auto t = requirement::Type::create<requirement::UnsolvedType>(type_solver);
    auto t2 = requirement::Type::create<requirement::UnsolvedType>(type_solver);
    auto t3 = requirement::Type::create<requirement::UnsolvedType>(type_solver);
    auto float_type = requirement::Type::create<requirement::RealNumberType>(type_solver);

    type_solver->bind(t,float_type,dummy_location);
    type_solver->bind(t,t2,dummy_location);
    type_solver->bind(t2,t3,dummy_location);

    ASSERT_TRUE(type_solver->solve(t)->is_type<requirement::RealNumberType>());
}

TEST_F(TypeSolverTest,ManySimpleAssignment)
{
    /*
    v = 0
    v1 = v
    v2 = v1
    v3 = v2
    ...

    -> what is type of `vn`?
    */

    std::vector<std::shared_ptr<requirement::Type>> unknown_types(100000);
    for (auto& t : unknown_types)
    {
        t = requirement::Type::create<requirement::UnsolvedType>(type_solver);
    }//initialize

    type_solver->bind(unknown_types[0],integer_type,dummy_location);
    
    for (int i = 1;i < unknown_types.size();i++)
    {
        type_solver->bind(unknown_types[i],unknown_types[i-1],dummy_location);
    }//bind
    for (auto& t : unknown_types)
    {
        auto solution = type_solver->solve(t);
        //std::cout << solution->get_name() << std::endl;
        ASSERT_TRUE(solution->is_type<requirement::IntegerType>());
    }
}

TEST_F(TypeSolverTest,LazyAssignment)
{
    /*
    v = optional.none //'optional.none' means like std::nullopt. maybe other expression in other version
    v = 1

    -> what is type of `v`?
    */

   auto unknown_type = requirement::Type::create<requirement::UnsolvedType>(type_solver);
   type_solver->bind(unknown_type,integer_type,dummy_location);
   auto solution = type_solver->solve(unknown_type);

   ASSERT_TRUE(solution->is_type<requirement::IntegerType>());
}

TEST_F(TypeSolverTest,CircularAssignment)
{
    /*
    v = optional.none //'optional.none' means like std::nullopt. maybe other expression in other version
    v1 = v
    v = 0

    -> what is type of `v`,`v1`?
    */

    auto unknown_type_1 = requirement::Type::create<requirement::UnsolvedType>(type_solver);
    auto unknown_type_2 = requirement::Type::create<requirement::UnsolvedType>(type_solver);

    type_solver->bind(unknown_type_2,unknown_type_1,dummy_location);
    type_solver->bind(unknown_type_1,integer_type,dummy_location);

    auto solution_type_1 = type_solver->solve(unknown_type_1);
    auto solution_type_2 = type_solver->solve(unknown_type_2);

    ASSERT_TRUE(solution_type_1->is_type<requirement::IntegerType>());
    ASSERT_TRUE(solution_type_1->is_type<requirement::IntegerType>());
}

TEST_F(TypeSolverTest,FunctionWithNoArgument)
{
    /*
    function f()
    (
        v = 0
        return v
    )
    
    f()

    -> what is signature of `f`?
    */
    auto return_type = requirement::Type::create<requirement::UnsolvedType>(type_solver);
    auto function_type = requirement::Type::create<requirement::FunctionType>(
        type_solver,
        "f",
        std::vector<std::shared_ptr<requirement::Type>>(),std::vector<std::string>(),
        return_type
    );

    type_solver->bind(return_type,integer_type,dummy_location);
    auto solution = type_solver->solve(function_type);
    utils::Match{solution->get_variant()}
    (
        utils::Type<requirement::FunctionType>(),[this](auto&& function_type_)
        {
            ASSERT_TRUE(function_type_.get_return_type() ->template is_type<requirement::IntegerType>());
        },
        utils::Default(),[](auto&&)
        {
            FAIL() << "Function type passed to `type_solver.solve()` but returns not function type.";
        }
    );
}

TEST_F(TypeSolverTest,FunctionWithArguments)
{
    /*
    function f(x,y)
    (
        v = 0
        return v
    )
    
    f(10,"abc") // A

    -> what is the signature of `f` at `A`?
    */

    auto return_type = requirement::Type::create<requirement::UnsolvedType>(type_solver);
    std::vector<std::shared_ptr<requirement::Type>> arguments_types = 
    {requirement::Type::create<requirement::UnsolvedType>(type_solver),
    requirement::Type::create<requirement::UnsolvedType>(type_solver)};
    auto function_type = requirement::Type::create<requirement::FunctionType>(type_solver,"f",arguments_types,std::vector<std::string>({"x","y"}),return_type);

    type_solver->bind(arguments_types[0],integer_type,dummy_location);
    type_solver->bind(arguments_types[1],float_type,dummy_location);
    type_solver->bind(return_type,integer_type,dummy_location);

    auto solution = type_solver->solve(function_type);
    utils::Match{solution->get_variant()}
    (
        utils::Type<requirement::FunctionType>(),[this](auto&& function_type_)
        {
            ASSERT_TRUE(function_type_.get_argument_types()[0] ->template is_type<requirement::IntegerType>());
            ASSERT_TRUE(function_type_.get_argument_types()[1] ->template is_type<requirement::RealNumberType>());
            ASSERT_TRUE(function_type_.get_return_type() ->template is_type<requirement::IntegerType>());
        },
        utils::Default(),[](auto&&)
        {
            FAIL() << "Function type passed to `type_solveer.solve()` but return was not function type.";
        }
    );
}

TEST_F(TypeSolverTest, ComplementaryUnionTypeInference)
{
    /*
    x = Union#tag1(0)
    x = Union$tag2("")

    x should be typed Union{tag1:integer,tag2:string}
    */

    auto x_type = requirement::Type::create<requirement::UnsolvedType>(type_solver);
    std::map<std::string, std::shared_ptr<requirement::Type>> tags1;
    tags1["tag1"] = requirement::Type::create<requirement::IntegerType>(type_solver);
    tags1["tag2"] = requirement::Type::create<requirement::UnsolvedType>(type_solver);
    auto union_type1 = requirement::Type::create<requirement::UnionType>(type_solver, "Union", tags1,type_solver);

    type_solver->bind(x_type,union_type1,dummy_location);

    std::map<std::string, std::shared_ptr<requirement::Type>> tags2;
    tags2["tag1"] = requirement::Type::create<requirement::UnsolvedType>(type_solver);
    tags2["tag2"] = requirement::Type::create<requirement::RealNumberType>(type_solver);
    auto union_type2 = requirement::Type::create<requirement::UnionType>(type_solver, "Union", tags2,type_solver);

    type_solver->bind(union_type1,union_type2,dummy_location);
    auto solved_x_type = type_solver->solve(x_type);
    ASSERT_TRUE(solved_x_type->is_type<requirement::UnionType>());
    auto& solved_tags = solved_x_type->as<requirement::UnionType>().get_solved_tags();
    ASSERT_TRUE(solved_tags.count("tag1") && solved_tags.at("tag1")->is_type<requirement::IntegerType>());
    ASSERT_TRUE(solved_tags.count("tag2") && solved_tags.at("tag2")->is_type<requirement::RealNumberType>());
}

TEST_F(TypeSolverTest,RemappingClonedType)
{
    /*
    x = ?
    x = 0

    when it comes to cloning above code,

    what is type of x_cloned?

    */
    requirement::ClonedTypeMap cloned_type_map;
    auto x_type = requirement::Type::create<requirement::UnsolvedType>(type_solver);
    type_solver->bind(x_type,integer_type,dummy_location);
    auto cloned_type = x_type->clone(cloned_type_map); 
    ASSERT_TRUE(type_solver->solve(cloned_type)->is_type<requirement::UnsolvedType>()); //not yet remapped, so cloned type should be unsolved yet.
    type_solver->remap(cloned_type_map,dummy_location);
    ASSERT_TRUE(type_solver->solve(cloned_type)->is_type<requirement::IntegerType>()); //types are remapped, so cloned type should be integer type because original is integer.
}

TEST_F(TypeSolverTest,RemappingClonedFunctionType)
{
    /*
    function f(x,y)
    (
        return x
    )



    */
    requirement::ClonedTypeMap cloned_type_map;
    auto x_type = requirement::Type::create<requirement::UnsolvedType>(type_solver);
    type_solver->bind(x_type,integer_type,dummy_location);
    auto cloned_type = x_type->clone(cloned_type_map); 
    ASSERT_TRUE(type_solver->solve(cloned_type)->is_type<requirement::UnsolvedType>()); //not yet remapped, so cloned type should be unsolved yet.
    type_solver->remap(cloned_type_map,dummy_location);
    ASSERT_TRUE(type_solver->solve(cloned_type)->is_type<requirement::IntegerType>()); //types are remapped, so cloned type should be integer type because original is integer.
}
