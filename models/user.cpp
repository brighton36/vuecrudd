#include "user.hpp"

using namespace std;

PSYM_MODEL(User)

void User::Migrate() {
  CreateTable({
    #define COLUMN(a, _, t) {#a, t},
    USER_COLUMNS
    #undef COLUMN
    {"password", "varchar(100)"},
  });
};
