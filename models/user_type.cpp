#include "user_type.hpp"

using namespace std;

PSYM_MODEL(UserType)

void UserType::Migrate() {
  CreateTable({
    #define COLUMN(a, _, t) {#a, t},
    USER_TYPE_COLUMNS
    #undef COLUMN
  });
};
