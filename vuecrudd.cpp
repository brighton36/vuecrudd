#include <filesystem>

#include "prails.hpp"
#include "faker.hpp"

// Included here for mode_seed:
#include "account.hpp"
#include "crm_company.hpp"
#include "crm_person.hpp"
#include "crm_position.hpp"

using namespace std;
using namespace prails;

unsigned int mode_seed(ConfigParser &, shared_ptr<spdlog::logger> logger, const vector<string> &args) {
  string program_name = filesystem::path(args[0]).filename();
  logger->info("{} seed started", program_name);

  tm tm_time = Model::NowUTC();
  auto faker = Faker();

  // Account:
  auto first_account = Account({
    {"first_name",  "Guest"},
    {"last_name",   "User"},
    {"email",       "guest@vue-crud.com"},
    {"created_at",  tm_time},
    {"updated_at",  tm_time}
  });
  first_account.password("demopassword123");
  first_account.save();

  // Crm/Company:
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

  // Crm/Person:
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

  // Crm/Position:
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

  return 0;
}

int main(int argc, char *argv[]) { 
  return prails::main(argc, argv, {{"seed", mode_seed}}); 
}
