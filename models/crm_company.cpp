#include "crm_company.hpp"

using namespace std;

PSYM_MODEL(CrmCompany);

void CrmCompany::Migrate() {
  CreateTable({
    #define COLUMN(a, _, t) {#a, t},
    CRM_COMPANY_COLUMNS
    #undef COLUMN
  });
};
