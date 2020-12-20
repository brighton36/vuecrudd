#include <regex>

#include "gtest/gtest.h"

#include "faker.hpp"
#include "faker.en_us.hpp"

using namespace std;

TEST(faker, random_decimal_string) {
  EXPECT_EQ( "", Faker::random_decimal_string(0) );

  EXPECT_TRUE(regex_match(Faker::random_decimal_string(1), regex("^[\\d]{1}$")));
  EXPECT_TRUE(regex_match(Faker::random_decimal_string(2), regex("^[\\d]{2}$")));
  EXPECT_TRUE(regex_match(Faker::random_decimal_string(3), regex("^[\\d]{3}$")));
  EXPECT_TRUE(regex_match(Faker::random_decimal_string(4), regex("^[\\d]{4}$")));
  EXPECT_TRUE(regex_match(Faker::random_decimal_string(5), regex("^[\\d]{5}$")));
}

TEST(faker, locale) {
  auto faker = Faker();
  EXPECT_EQ( "en_us", faker.locale() );
}

TEST(faker, sv_to_vector) {
  auto last_names = Faker::sv_to_vector(_last_name);

  EXPECT_EQ( 473, last_names.size() );
  EXPECT_EQ( "Abbott", last_names[0] );
  EXPECT_EQ( "Zulauf", last_names[472] );
}
