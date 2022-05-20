#include "permission.hpp"

using namespace std;

PSYM_MODEL(Permission)

void Permission::Migrate(unsigned int) {
  CreateTable({
    #define COLUMN(a, _, t) {#a, t},
    PERMISSION_COLUMNS
    #undef COLUMN
  });
};
