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

using namespace std;
using namespace prails;

unsigned int mode_seed(ConfigParser &, shared_ptr<spdlog::logger> logger, 
  const vector<string> &args) {
  
  string program_name = filesystem::path(args[0]).filename();
  logger->info("{} seed started", program_name);

  tm tm_time = Model::NowUTC();
  auto faker = Faker();

  /*
  // TODO:
  DB::table('user_types')->insert([ 
    ['name' => 'Guest',
    'read' => true,
    'insert' => false,
    'update' => false,
    'delete' => false,
    'admin' => false,
    'active' => true],

    ['name' => 'User',
    'read' => true,
    'insert' => true,
    'update' => true,
    'delete' => true,
    'admin' => false,
    'active' => true],
    ['name' => 'Admin',
    'read' => true,
    'insert' => true,
    'update' => true,
    'delete' => true,
    'admin' => true,
    'active' => true]
  ]);

  DB::table('users')->insert([[
    'name' => 'admin',
    'email' => 'admin@admin.com',
    'password' => bcrypt('1234'),
    'user_type_id' => 3
  ]]);

  DB::table('permissions')->insert([
    ['name' => 'CRM',
    'code' => 'CRM',
    'path' => 'crm'
    ],
    ['name' => 'administracja',
    'code' => 'ADMIN',
    'path' => 'admin'
    ]
  ]);

  DB::table('user_permissions')->insert([
    ['user_id' => 1,'permission_id' => 1],
    ['user_id' => 1,'permission_id' => 2]
  ]);
  */
  // Account:
  auto first_account = Account({
    {"first_name",  "Admin"},
    {"last_name",   "User"},
    {"email",       "admin@admin.com"},
    {"created_at",  tm_time},
    {"updated_at",  tm_time}
  });
  first_account.password("1234");
  first_account.save();

  // Company Types
  map<string,string> company_types = { {"Media", "MED"}, {"Firmy IT", "FIT"},
    {"Sklepy", "SKL"}, {"Organizacje", "ORG"} };

  for_each(company_types.begin(), company_types.end(), [&](const auto &p) { 
    auto company_type = CrmCompanyType({
      {"name",       p.first},
      {"code",       p.second},
      {"active",     1},
      {"created_at", tm_time},
      {"updated_at", tm_time}
    });
    company_type.save();
  });

  // Sexes
  auto sex_dash = CrmSex({ 
    {"name",       "-"}, 
    {"code",       "-"}, 
    {"eng_name",   "-"}, 
    {"eng_code",   "-"}, 
    {"priority",   1},
    {"active",     1},
    {"created_at", tm_time},
    {"updated_at", tm_time}
    });
  sex_dash.save();

  auto sex_female = CrmSex({ 
    {"name",       "Kobieta"},
    {"code",       "K"},
    {"eng_name",   "Female"}, 
    {"eng_code",   "F"},
    {"priority",   2},
    {"active",     1},
    {"created_at", tm_time},
    {"updated_at", tm_time}
  });
  sex_female.save();

  auto sex_male = CrmSex({ 
    {"name",       "Mężczyzna"},
    {"code",       "M"},
    {"eng_name",   "Male"}, 
    {"eng_code",   "M"}, 
    {"priority",   3},
    {"active",     1},
    {"created_at", tm_time},
    {"updated_at", tm_time}
  });
  sex_male.save();

  // Street Prefixes
  map<string,string> street_prefixes = { {"", "-"}, {"ul", "Ulica"}, 
    {"al", "Aleja"}, {"pl", "Plac"}, {"os.", "Osiedle"} };

  for_each(street_prefixes.begin(), street_prefixes.end(), [&](const auto &p) { 
    auto sp = CrmStreetPrefix({
      {"name",       p.first},
      {"code",       p.second},
      {"active",     1},
      {"created_at", tm_time},
      {"updated_at", tm_time}
    });
    sp.save();
  });

  // Languages:
  map<string,unsigned int> languages = { {"Polski", 1}, {"Angielski", 2} }; 

  for_each(street_prefixes.begin(), street_prefixes.end(), [&](const auto &p) { 
    auto language = CrmLanguage({
      {"name",       p.first},
      {"priority",   p.second},
      {"active",     1},
      {"created_at", tm_time},
      {"updated_at", tm_time}
    });
    language.save();
  });

  // Crm/Company:
  for (unsigned int i = 0; i<1000; i++) {
    string company_name = faker.company();

    auto company = CrmCompany({
      {"name",             company_name},
      {"common_name",      company_name},
      {"company_type_id",  (rand() % 3)+1},
      {"street_prefix_id", 1},
      {"city",             string(faker.city())},
      {"street",           string(faker.street_address())},
      {"email",            string(faker.email())},
      {"active",           1},
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
      {"active",           1},
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
      {"active",           1},
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
