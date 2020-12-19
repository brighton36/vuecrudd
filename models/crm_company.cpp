#include "crm_company.hpp"

using namespace std;

PSYM_MODEL(CrmCompany);

string CrmCompany::address() {
  string addr;

  if (street().has_value())
    addr.append(street().value());

  // This is how the reference worked... (shrug)
  addr.append(" ");

  if (house_number().has_value())
    addr.append(house_number().value());

  if (apartment_number().has_value() && (!(*apartment_number()).empty()))
    addr.append(" lok. "+*apartment_number());

  // Remove excess whitespace:
  string ret;
  regex_replace(back_inserter(ret), addr.begin(), addr.end(), regex("\\s+"), " ");

  return ret;
}

void CrmCompany::Migrate() {
  CreateTable({
    #define COLUMN(a, _, t) {#a, t},
    CRM_COMPANY_COLUMNS
    #undef COLUMN
  });
};
