#include <random>
#include <array>

// This was very loosely based on https://github.com/fzaninotto/Faker, and mostly
// used in the CrmCompany migration.

class Faker { 
  public:
    // NOTE: locale isn't used for anything rn. It's mostly a stub for anyone
    // else who may care to flesh this out...
    explicit Faker(std::string locale = "en_US") : _locale(locale) {};
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
    std::string locale() { return _locale; };

  private: 
    typedef std::vector<std::string> LangStrings;

    std::string _locale;

    template <typename T>
    T random_element(std::vector<T> els) { return els[rand() % els.size()]; }

    std::string rand_string(LangStrings els) { return random_element<std::string>(els); }
    
    template <typename T>
    std::string rand_el(T els) { return els[rand() % els.size()]; }

};
