#include "crm_person.hpp"
#include "utilities.hpp"

using namespace std;

PSYM_MODEL(CrmPerson);

void CrmPerson::Migrate() {
  CreateTable({
    #define COLUMN(a, _, t) {#a, t},
    CRM_PERSON_COLUMNS
    #undef COLUMN
  });
};

string CrmPerson::fullname() {
  vector<string> fullname;
  if (firstname().has_value()) fullname.push_back(*firstname());
  if (lastname().has_value()) fullname.push_back(*lastname());
  return prails::utilities::join(fullname, " ");
}
