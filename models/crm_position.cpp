#include "crm_position.hpp"
#include "faker.hpp"

using namespace std;

PSYM_MODEL(CrmPosition);

void CrmPosition::Migrate() {
  CreateTable({
    #define COLUMN(a, _, t) {#a, t},
    CRM_POSITION_COLUMNS
    #undef COLUMN
  });

  tm tm_time = Model::NowUTC();
  auto faker = Faker();

  for (unsigned int i = 0; i<2000; i++) {
    auto position = CrmPosition({
      {"company_id", ((rand() % 323)+1)},
      {"person_id",  ((rand() % 3002)+1)},
      {"name",       faker.job_title()},
      {"phone",      faker.phone_number()},
      {"created_at",  tm_time},
      {"updated_at",  tm_time}
    });

    position.save();
  }
};
