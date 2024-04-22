#include "preprocessor.hpp"
#include <gtest/gtest.h>
#include <sstream>
using namespace lexy2;

TEST(MacroExpansionTest, PreprocessorTests) {
  std::string in(R"(
    #define N 10
    #define something 1000
    #define MY_STRING "hello world"

    N + something / 12 -> MY_STRING)");
  Preprocessor p(in);
  auto actual = p.expandMacros();
  auto expected = R"(




    10 + 1000 / 12 -> "hello world"
)";
  EXPECT_STREQ(actual.c_str(), expected);
}
