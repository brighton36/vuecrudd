#include "user_permission.hpp"

using namespace std;

PSYM_MODEL(UserPermission)

void UserPermission::Migrate() {
  CreateTable({
    #define COLUMN(a, _, t) {#a, t},
    USER_PERMISSION_COLUMNS
    #undef COLUMN
  });
};
