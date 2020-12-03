#include "crm_language.hpp"

using namespace std;

PSYM_MODEL(CrmLanguage);

void CrmLanguage::Migrate() {
  CreateTable({
    #define COLUMN(a, _, t) {#a, t},
    CRM_LANGUAGE_COLUMNS
    #undef COLUMN
  });
};
