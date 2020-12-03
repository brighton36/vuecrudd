#include "crm_company_file.hpp"

using namespace std;

PSYM_MODEL(CrmCompanyFile);

void CrmCompanyFile::Migrate() {
  CreateTable({
    #define COLUMN(a, _, t) {#a, t},
    CRM_COMPANY_FILE_COLUMNS
    #undef COLUMN
  });
};
