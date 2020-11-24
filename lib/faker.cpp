#include "faker.hpp"
#include "fmt/fmt.h"
#include "utilities.hpp"

#include "faker.en_us.hpp"

using namespace std;
using namespace prails::utilities;

string Faker::locale() { 
  return {_locale->data(), _locale->size()};
}

string Faker::company() { 
  static vector<string> company_formats = split(_company_formats, ":");
  return fmt::format(rand_el(company_formats),
    fmt::arg("last_name", last_name()), 
    fmt::arg("company_suffix", company_suffixes()) );
}

string Faker::company_suffixes() { 
  static vector<string> company_suffixes = split(_company_suffixes, ":");
  return rand_el(company_suffixes);
}

string Faker::city_prefix() { 
  static vector<string> city_prefixes = split(_city_prefix, ":");
  return rand_el(city_prefixes);
}

string Faker::city_suffix() { 
  static vector<string> city_suffixes = split(_city_suffix, ":");
  return rand_el(city_suffixes);
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

string Faker::street_suffix() { 
  static vector<string> street_suffixes = split(_street_suffixes, ":");
  return rand_el(street_suffixes);
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

string Faker::free_email_domain() { 
  static vector<string> free_email_domains = split(_free_email_domains, ":");
  return rand_el(free_email_domains);
}

string Faker::domain_name() { 
  static vector<string> tlds = split(_tlds, ":");
	const string legal_chars = "abcdefghijklmnopqrstuvwxyz0123456789-";

  string domain = last_name();

  // Lowercase it:
  transform(domain.begin(), domain.end(), domain.begin(), ::tolower); 

  // Remove any weird chars:
	remove_if(domain.begin(), domain.end(), [legal_chars](char ch) -> bool {
    for (int i = 0; i < legal_chars.length(); ++i) 
      if (ch == legal_chars[i]) 
        return false;
    return true;
  } );

  return domain+"."+rand_el(tlds);
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
  static vector<string> female_names = split(_first_name_female, ":");
  static vector<string> male_names = split(_first_name_male, ":");
  return rand_el((rand() % 2) ? female_names : male_names);
}

string Faker::last_name() { 
  static vector<string> last_names = split(_last_names, ":");
  return rand_el(last_names);
}

