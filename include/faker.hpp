#pragma once
#include <random>
#include <array>

// This was very loosely based on https://github.com/fzaninotto/Faker, and mostly
// used in the CrmCompany migration.

class Faker { 
  public:
    explicit Faker() {};
    std::string locale();
    std::string company();
    std::string company_suffixes();
    std::string city_prefix();
    std::string city_suffix();
    std::string city();
    std::string building_number();
    std::string street_name();
    std::string street_address();
    std::string street_suffix();
    std::string secondary_address();
    std::string user_name();
    std::string free_email_domain();
    std::string domain_name();
    std::string email();
    std::string first_name();
    std::string last_name();
    std::string suffix();
    std::string phone_number();
    std::string job_title();

    static std::string random_decimal_string(int nLength);

    template <std::size_t N>
    static std::vector<std::string> sv_to_vector(const std::string_view (&sv)[N]) {
      std::vector<std::string> ret;
      for(unsigned int i = 0; i < N; i++) 
        ret.push_back(std::string({sv[i].data(), sv[i].size()}));
      return ret;
    }

  private: 
    template <typename T>
    std::string rand_el(T els) { return els[rand() % els.size()]; }
};
