#include "model.hpp"
#include <random>

#include "picosha2.h"
#include "base64.hpp"

class Account : public Model::Instance<Account> { 
  public:
    MODEL_CONSTRUCTOR(Account)

    MODEL_ACCESSOR(id, long)
    MODEL_ACCESSOR(first_name, std::string)
    MODEL_ACCESSOR(last_name, std::string)
    MODEL_ACCESSOR(email, std::string)
    MODEL_ACCESSOR(auth_token, std::string)
    MODEL_ACCESSOR(auth_token_issued_at, std::tm)
    MODEL_ACCESSOR(send_offers, int)
    MODEL_ACCESSOR(send_service_updates, int)
    MODEL_ACCESSOR(created_at, std::tm)
    MODEL_ACCESSOR(updated_at, std::tm)

    inline static const unsigned int auth_token_size = 128;

    inline static const Model::Definition Definition {
      "id",
      "accounts", 
      Model::ColumnTypes({
        {"id",                   COL_TYPE(long)},
        {"first_name",           COL_TYPE(std::string)},
        {"last_name",            COL_TYPE(std::string)},
        {"email",                COL_TYPE(std::string)},
        {"password",             COL_TYPE(std::string)},
        {"auth_token",           COL_TYPE(std::string)},
        {"auth_token_issued_at", COL_TYPE(std::tm)},
        {"send_offers",          COL_TYPE(int)},
        {"send_service_updates", COL_TYPE(int)},
        {"created_at", COL_TYPE(std::tm)},
        {"updated_at", COL_TYPE(std::tm)}
      }),
      Model::Validations( {
        Model::Validates::NotNull("first_name"),
        Model::Validates::NotNull("last_name"),
        Model::Validates::NotNull("email"),
        Model::Validates::IsUnique("email"),
        Model::Validates::Matches("email", regex_from_string("/.+@.+/")), // TODO
        Model::Validates::NotNull("password"),
        Model::Validates::IsBoolean("send_offers"),
        Model::Validates::IsBoolean("send_service_updates"),
        Model::Validates::NotNull("created_at"),
        Model::Validates::NotNull("updated_at")
      })
    };

    static void Migrate() {
      CreateTable({
        {"first_name", "varchar(100)"},
        {"last_name", "varchar(100)"},
        {"email", "varchar(100)"},
        {"password", "varchar(100)"},
        // This is a bit sloppy, I should probably calculate the exact size. But, 
        // roughly, 128 characters of randomness is 172 characters of base64
        {"auth_token", fmt::format("varchar({})",auth_token_size*2)},
        {"auth_token_issued_at", "datetime"},
        {"send_offers", "integer"},
        {"send_service_updates", "integer"},
        {"created_at", "datetime"},
        {"updated_at", "datetime"}
      });

      std::tm tm_time = Model::NowUTC();
      auto first_account = Account({
        {"first_name",  "Guest"},
        {"last_name",   "User"},
        {"email",       "guest@vue-crud.com"},
        {"created_at",  tm_time},
        {"updated_at",  tm_time}
      });
      first_account.password("guest_password");
      first_account.save();
    };

    std::optional<std::string> full_name() { 
      auto fname = first_name();
      auto lname = last_name();

      if (!fname && !lname) return std::nullopt;

      std::string ret;
      if (fname) ret.append(*fname);
      if (fname && lname) ret.append(" ");
      if (lname) ret.append(*lname);

      return std::make_optional(ret);
    };

    void password(const std::optional<Model::RecordValue> &val) { 
      recordSet("password", Account::Hash(std::get<std::string>(*val))); 
    };
    std::optional<std::string> password() { 
      auto val = recordGet("password");
      return (val) ? make_optional(std::get<std::string>(*val)) : std::nullopt;
    }

    void generate_new_auth_token() { 
      using random_bytes_engine = std::independent_bits_engine<
        std::default_random_engine, CHAR_BIT, unsigned char>;

      random_bytes_engine rbe;
      std::vector<base64::byte> source(auth_token_size);
      std::generate(begin(source), end(source), std::ref(rbe));

      auth_token(base64::encode(source));
      auth_token_issued_at(Model::NowUTC());
    }

    void clear_auth_token() { 
      auth_token(std::nullopt);
      auth_token_issued_at(std::nullopt);
    }

    static std::optional<Account> FromLogin(const std::string &email, 
      const std::string &password) {
      return Account::Find("email = :email and password = :password", 
        Model::Record({{"email", email}, {"password", Account::Hash(password)}}));
    }

    static std::optional<Account> FromToken(const std::string &token) {
      if (token.empty()) return std::nullopt;

      // TODO: accomodate < ndays from generation date
      return Account::Find("auth_token = :auth_token", Model::Record({{"auth_token", token}}));
    }

    static std::string Hash(const std::string &unsalted_phrase) {
      // TODO: store the salt in a config: Controller::GetConfig().cors_allow()
      std::string salt = "I94anUGppukOHVAmL2a_kSmS"; // A random string
      std::string source = salt+unsalted_phrase;

      std::vector<unsigned char> hash(picosha2::k_digest_size);
      picosha2::hash256(source.begin(), source.end(), hash.begin(), hash.end());

      return picosha2::bytes_to_hex_string(hash.begin(), hash.end());
    }
};
