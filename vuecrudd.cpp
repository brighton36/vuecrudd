#include <filesystem>

#include "prails.hpp"
#include "faker.hpp"

// Included here for mode_seed:
#include "account.hpp"
#include "crm_company.hpp"
#include "crm_company_type.hpp"
#include "crm_sex.hpp"
#include "crm_street_prefix.hpp"
#include "crm_language.hpp"
#include "crm_person.hpp"
#include "crm_position.hpp"
#include "permission.hpp"
#include "user.hpp"
#include "user_permission.hpp"
#include "user_type.hpp"

using namespace std;
using namespace prails;

unsigned int mode_seed(ConfigParser &, shared_ptr<spdlog::logger> logger, 
  const vector<string> &args) {
  
  string program_name = filesystem::path(args[0]).filename();
  logger->info("{} seed started", program_name);

  tm now = Model::NowUTC();
  auto faker = Faker();

  auto uts = vector<Model::Record>({
    { {"name", "Guest"}, {"read", 1}, {"ins", 0}, {"upd", 0}, {"del", 0}, 
      {"admin", 0}, {"active", 1}, {"updated_at", now}, {"created_at", now} },
    { {"name", "User"}, {"read", 1}, {"ins", 1}, {"upd", 1}, {"del", 1}, 
      {"admin", 0}, {"active", 1}, {"updated_at", now}, {"created_at", now} },
    { {"name", "Admin"}, {"read", 1}, {"ins", 1}, {"upd", 1}, {"del", 1}, 
      {"admin", 1}, {"active", 1}, {"updated_at", now}, {"created_at", now} }
    });

  for_each(uts.begin(), uts.end(), [&](const auto &u) { UserType(u).save(); });

  // User:
  auto admin_user = User({
    {"name",        "Admin"},
    {"email",       "admin@admin.com"},
    {"user_type_id", 3},
    {"created_at",  now},
    {"updated_at",  now}
  });
  // TODO: admin_user.password("1234");
  admin_user.save();
  
  // Permissions:
  Permission({ {"name", "CRM"}, {"code", "CRM"}, {"path", "crm"},
    {"active", 1}, {"created_at", now}, {"updated_at", now} }).save();

  Permission({ {"name", "administracja"}, {"code", "ADMIN"}, {"path", "admin"},
    {"active", 1}, {"created_at", now}, {"updated_at", now} }).save();

  // User Permissions:
  UserPermission({ {"user_id", 1}, {"permission_id", 1},
    {"active", 1}, {"created_at", now}, {"updated_at", now} }).save();

  UserPermission({ {"user_id", 1}, {"permission_id", 2},
    {"active", 1}, {"created_at", now}, {"updated_at", now} }).save();

  // Account:
  // TODO: I think we can remove this in liue of users...
  auto first_account = Account({ {"first_name", "Admin"}, {"last_name", "User"},
    {"email", "admin@admin.com"}, {"created_at", now}, {"updated_at", now} });
  first_account.password("1234");
  first_account.save();

  // Company Types
  map<string,string> company_types = { {"Media", "MED"}, {"Firmy IT", "FIT"},
    {"Sklepy", "SKL"}, {"Organizacje", "ORG"} };

  for_each(company_types.begin(), company_types.end(), [&](const auto &p) { 
    CrmCompanyType({ {"name", p.first}, {"code", p.second}, {"active", 1}, 
    {"created_at", now}, {"updated_at", now}}).save();
  });

  // Sexes
  CrmSex({ {"name", "-"}, {"code", "-"}, {"eng_name", "-"}, {"eng_code", "-"}, 
    {"priority", 1}, {"active", 1}, {"created_at", now}, 
    {"updated_at", now} }).save();

  CrmSex({ {"name", "Kobieta"}, {"code", "K"}, {"eng_name", "Female"}, 
    {"eng_code", "F"}, {"priority", 2}, {"active", 1}, {"created_at", now},
    {"updated_at", now} }).save();

  CrmSex({ {"name", "Mężczyzna"}, {"code", "M"}, {"eng_name", "Male"}, 
    {"eng_code", "M"}, {"priority", 3}, {"active", 1}, {"created_at", now},
    {"updated_at", now} }).save();

  // Street Prefixes
  map<string,string> street_prefixes = { {"", "-"}, {"ul", "Ulica"}, 
    {"al", "Aleja"}, {"pl", "Plac"}, {"os.", "Osiedle"} };

  for_each(street_prefixes.begin(), street_prefixes.end(), [&](const auto &p) { 
    CrmStreetPrefix({ {"name", p.first}, {"code", p.second}, {"active", 1}, 
      {"created_at", now}, {"updated_at", now} }).save();
  });

  // Languages:
  map<string,unsigned int> languages = { {"Polski", 1}, {"Angielski", 2} }; 

  for_each(street_prefixes.begin(), street_prefixes.end(), [&](const auto &p) { 
    CrmLanguage({{"name", p.first}, {"priority", p.second}, {"active", 1},
      {"created_at", now}, {"updated_at", now} }).save();
  });

  // Crm/Company:
  for (unsigned int i = 0; i<1000; i++) {
    string company_name = faker.company();

    CrmCompany({
      {"name",             company_name},
      {"common_name",      company_name},
      {"company_type_id",  (rand() % 3)+1},
      {"street_prefix_id", 1},
      {"city",             string(faker.city())},
      {"street",           string(faker.street_address())},
      {"email",            string(faker.email())},
      {"active",           1},
      {"created_at",       now},
      {"updated_at",       now}
    }).save();
  }

  // Crm/Person:
  for (unsigned int i = 0; i<3000; i++)
    CrmPerson({
      {"firstname",   faker.first_name()},
      {"lastname",    faker.last_name()},
      {"distinction", faker.suffix()},
      {"sex_id",      ((rand() % 2)+2)},
      {"language_id", 2},
      {"email",       faker.email()},
      {"phone",       faker.phone_number()},
      {"active",      1},
      {"created_at",  now},
      {"updated_at",  now}
    }).save();

  // Crm/Position:
  for (unsigned int i = 0; i<2000; i++)
    CrmPosition({
      {"company_id", ((rand() % 323)+1)},
      {"person_id",  ((rand() % 3002)+1)},
      {"name",       faker.job_title()},
      {"phone",      faker.phone_number()},
      {"active",     1},
      {"created_at", now},
      {"updated_at", now}
    }).save();

  return 0;
}

int main(int argc, char *argv[]) { 
  return prails::main(argc, argv, {{"seed", mode_seed}}); 
}
