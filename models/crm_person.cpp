#include "crm_person.hpp"

using namespace std;

PSYM_MODEL(CrmPerson);

void CrmPerson::Migrate() {
  CreateTable({
    #define COLUMN(a, _, t) {#a, t},
    CRM_PERSON_COLUMNS
    #undef COLUMN
  });
};
