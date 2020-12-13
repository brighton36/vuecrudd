#include "crm_position.hpp"

using namespace std;

PSYM_MODEL(CrmPosition);

void CrmPosition::Migrate() {
  CreateTable({
    #define COLUMN(a, _, t) {#a, t},
    CRM_POSITION_COLUMNS
    #undef COLUMN
  });
};
