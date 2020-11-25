#include "crm_street_prefix.hpp"

using namespace std;

REGISTER_MODEL(CrmStreetPrefix);

void CrmStreetPrefix::Migrate() {
  CreateTable({
    #define COLUMN(a, _, t) {#a, t},
    CRM_STREET_PREFIX_COLUMNS
    #undef COLUMN
  });
};
