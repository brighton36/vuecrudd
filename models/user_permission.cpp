#include "user_permission.hpp"

using namespace std;

PSYM_MODEL(UserPermission)

void UserPermission::Migrate(unsigned int) {
  CreateTable({
    #define COLUMN(a, _, t) {#a, t},
    USER_PERMISSION_COLUMNS
    #undef COLUMN
  });
};
