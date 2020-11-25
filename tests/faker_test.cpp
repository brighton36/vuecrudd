#include <regex>

#include "faker.hpp"
#include "gtest/gtest.h"

using namespace std;

TEST(faker, random_decimal_string) {
  EXPECT_EQ( "", Faker::random_decimal_string(0) );

  EXPECT_TRUE(regex_match(Faker::random_decimal_string(1), regex("^[\\d]{1}$")));
  EXPECT_TRUE(regex_match(Faker::random_decimal_string(2), regex("^[\\d]{2}$")));
  EXPECT_TRUE(regex_match(Faker::random_decimal_string(3), regex("^[\\d]{3}$")));
  EXPECT_TRUE(regex_match(Faker::random_decimal_string(4), regex("^[\\d]{4}$")));
  EXPECT_TRUE(regex_match(Faker::random_decimal_string(5), regex("^[\\d]{5}$")));
}
