#include <random>

// This was very loosely based on https://github.com/fzaninotto/Faker, and mostly
// used in the CrmCompany migration.

class Faker { 
  public:
    // NOTE: locale isn't used for anything rn. It's mostly a stub for anyone
    // else who may care to flesh this out...
    explicit Faker(std::string locale = "en_US") : locale(locale)  {};
    std::string company();
    std::string city();
    std::string street_address();
    std::string email();
    std::string first_name();
    std::string last_name();

  private: 
    std::string locale;

    template <typename T>
    T random_element(std::vector<T> els) { return els[rand() % els.size()]; }

    // From: https://github.com/fzaninotto/Faker/blob/master/src/Faker/Provider/...
    static const std::vector<std::string> _first_name_male;
    static const std::vector<std::string> _first_name_female;
    static const std::vector<std::string> _last_name;
    static const std::vector<std::string> _suffixes;
    static const std::vector<std::string> _city_prefix;
    static const std::vector<std::string> _city_suffix;
};
