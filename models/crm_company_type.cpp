#include "crm_company_type.hpp"

using namespace std;

PSYM_MODEL(CrmCompanyType);

void CrmCompanyType::Migrate() {
  CreateTable({
    #define COLUMN(a, _, t) {#a, t},
    CRM_COMPANY_TYPE_COLUMNS
    #undef COLUMN
  });
};
