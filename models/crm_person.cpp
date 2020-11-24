#include "crm_person.hpp"
#include "faker.hpp"

using namespace std;

REGISTER_MODEL(CrmPerson);

void CrmPerson::Migrate() {
  CreateTable({
    #define COLUMN(a, _, t) {#a, t},
    CRM_PERSON_COLUMNS
    #undef COLUMN
  });

  tm tm_time = Model::NowUTC();
  auto faker = Faker();

  for (unsigned int i = 0; i<3000; i++) {
    auto person = CrmPerson({
      {"firstname",   faker.first_name()},
      {"lastname",    faker.last_name()},
      {"distinction", faker.suffix()},
      {"sex_id",      ((rand() % 2)+2)},
      {"language_id", 2},
      {"email",       faker.email()},
      {"phone",       faker.phone_number()},
      {"created_at",  tm_time},
      {"updated_at",  tm_time}
    });

    person.save();
  }
};
