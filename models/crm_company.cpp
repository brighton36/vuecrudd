#include "crm_company.hpp"
#include "faker.hpp"

using namespace std;

PSYM_MODEL(CrmCompany);

void CrmCompany::Migrate() {
  CreateTable({
    #define COLUMN(a, _, t) {#a, t},
    CRM_COMPANY_COLUMNS
    #undef COLUMN
  });

  tm tm_time = Model::NowUTC();
  auto faker = Faker();

  for (unsigned int i = 0; i<10000; i++) {
    string company_name = faker.company();

    auto company = CrmCompany({
      {"name",            company_name},
      {"common_name",     company_name},
      {"company_type_id", (rand() % 3)+1},
      {"street_prefix_id", 1},
      {"city",             string(faker.city())},
      {"street",           string(faker.street_address())},
      {"email",            string(faker.email())},
      {"created_at",       tm_time},
      {"updated_at",       tm_time}
    });

    company.save();
  }
};
