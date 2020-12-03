#include "crm_sex.hpp"

using namespace std;

PSYM_MODEL(CrmSex);

void CrmSex::Migrate() {
  CreateTable({
    #define COLUMN(a, _, t) {#a, t},
    CRM_SEX_COLUMNS
    #undef COLUMN
  });
};
