#include "faker.hpp"
#include "fmt/fmt.h"
#include "utilities.hpp"

#include "faker.en_us.hpp"

#define FUNC_RANDOM_STRING(m) \
  string Faker::m() { static vector<string> p = split(_##m, ":"); return rand_el(p); }

using namespace std;
using namespace prails::utilities;

string Faker::locale() { return {_locale->data(), _locale->size()}; }

FUNC_RANDOM_STRING(last_name)
FUNC_RANDOM_STRING(suffix)
FUNC_RANDOM_STRING(company_suffixes)
FUNC_RANDOM_STRING(city_prefix)
FUNC_RANDOM_STRING(city_suffix)
FUNC_RANDOM_STRING(street_suffix)
FUNC_RANDOM_STRING(free_email_domain)
FUNC_RANDOM_STRING(job_title)

string Faker::random_decimal_string(int nLength) {
	string ret;
	for(unsigned int i=0; i<nLength; i++) ret.append(to_string((rand() % 10)));
  return ret; 
}

string Faker::company() { 
  static vector<string> company_formats = split(_company_formats, ":");
  return fmt::format(rand_el(company_formats),
    fmt::arg("last_name", last_name()), 
    fmt::arg("company_suffix", company_suffixes()) );
}

string Faker::city() { 
  static vector<string> city_formats = split(_city_formats, ":");
  return fmt::format(rand_el(city_formats),
    fmt::arg("last_name", last_name()), 
    fmt::arg("first_name", first_name()), 
    fmt::arg("city_prefix", city_prefix()),
    fmt::arg("city_suffix", city_suffix()) );
}

string Faker::street_name() { 
  static vector<string> street_formats = split(_street_formats, ":");
  return fmt::format(rand_el(street_formats),
    fmt::arg("last_name", last_name()), 
    fmt::arg("first_name", first_name()), 
    fmt::arg("street_suffix", street_suffix()) );
}

string Faker::building_number() { 
  int num_digits = (rand() % 3)+1;
  return to_string(rand() % (int) pow(10, num_digits));
}

string Faker::secondary_address() { 
  static vector<string> secondary_address_formats = split(_secondary_address_formats, ":");
  return fmt::format(rand_el(secondary_address_formats),
    fmt::arg("num", building_number()) );
}

string Faker::street_address() { 
  static vector<string> street_address_formats = split(_street_address_formats, ":");
  return fmt::format(rand_el(street_address_formats),
    fmt::arg("building_number", building_number()), 
    fmt::arg("street_name", street_name()), 
    fmt::arg("secondary_address", secondary_address()) );
};

string Faker::domain_name() { 
  static vector<string> tlds = split(_tlds, ":");
  const string legal_chars = "abcdefghijklmnopqrstuvwxyz0123456789-";

  string domain = last_name();
  transform(domain.begin(), domain.end(), domain.begin(), ::tolower); 

  // Remove any weird chars:
  remove_if(domain.begin(), domain.end(), [legal_chars](char ch) -> bool {
    for (int i = 0; i < legal_chars.length(); ++i) 
      if (ch == legal_chars[i]) return false;
    return true;
  } );

  return fmt::format("{}.{}", domain, rand_el(tlds));
}

string Faker::user_name() { 
  static vector<string> user_name_formats = split(_user_name_formats, ":");

  char letter = 'a';
  letter = char((int)letter + rand() % 26);
  
  // Lowercase it:
  string fn = first_name();
  transform(fn.begin(), fn.end(), fn.begin(), ::tolower); 

  string ln = last_name();
  transform(ln.begin(), ln.end(), ln.begin(), ::tolower); 

  return fmt::format(rand_el(user_name_formats),
    fmt::arg("first_name", fn), 
    fmt::arg("last_name", ln), 
    fmt::arg("num", rand() % 100), 
    fmt::arg("letter", letter) );
}

string Faker::email() { 
  static vector<string> email_formats = split(_email_formats, ":");
  return fmt::format(rand_el(email_formats),
    fmt::arg("user_name", user_name()), 
    fmt::arg("domain_name", domain_name()), 
    fmt::arg("free_email_domain", free_email_domain()) );
};

string Faker::first_name() { 
  static vector<string> female_names = split(_firstname_female, ":");
  static vector<string> male_names = split(_firstname_male, ":");
  return rand_el((rand() % 2) ? female_names : male_names);
}

string Faker::phone_number() {
  static vector<string> phone_number_formats = split(_phone_number_formats, ":");
  return fmt::format( rand_el(phone_number_formats),
    fmt::arg("four_digits", Faker::random_decimal_string(4)), 
    fmt::arg("area_code", Faker::random_decimal_string(3)), 
    fmt::arg("exchange_code", Faker::random_decimal_string(3)) );
}
