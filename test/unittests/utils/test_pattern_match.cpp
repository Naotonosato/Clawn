#include <vector>
#include <map>
#include <variant>
#include "include/utils/pattern_match.hpp"
#include <gtest/gtest.h>

using namespace clawn;

TEST(PatternMatchTest,TypeMatching)
{
    std::variant<int,std::string> variant(10);
    bool is_done = false;
    utils::Match{variant}
    (
        utils::Type<int>(),[&is_done](auto&& value)
            {
                ASSERT_EQ(value,10);   
                is_done = true;
            },
        utils::Default(),[&is_done](auto&&)
            {
                FAIL() << "Implementaion of pattern matching has bug. Variant contains int and it should match int case.";
                is_done = true;
            }
    );

    if (!is_done) {FAIL() << "Implementaion of pattern matching has bug. didn't match Case<int> or Default case.";}
}