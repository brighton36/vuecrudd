#pragma once
#include "model.hpp"
#include "model_factory.hpp"

// The auth_token is a bit sloppy, I should probably calculate the exact size. 
// But, roughly, 128 characters of randomness is 172 characters of base64
// Note that we omit password, since the accessor logic isn't as simple as the
// other columns
#define ACCOUNT_COLUMNS                                     \
  COLUMN(first_name,           std::string, "varchar(100)") \
  COLUMN(last_name,            std::string, "varchar(100)") \
  COLUMN(email,                std::string, "varchar(100)") \
  COLUMN(auth_token,           std::string, fmt::format("varchar({})",auth_token_size*2)) \
  COLUMN(auth_token_issued_at, std::tm,     "datetime") \
  COLUMN(created_at,           std::tm,     "datetime") \
  COLUMN(updated_at,           std::tm,     "datetime")

class Account : public Model::Instance<Account> { 
  public:
    MODEL_CONSTRUCTOR(Account)

    MODEL_ACCESSOR(id, long)
    #define COLUMN(a, t, _) MODEL_ACCESSOR(a, t)
      ACCOUNT_COLUMNS
    #undef COLUMN

    inline static const unsigned int auth_token_size = 128;

    inline static const Model::Definition Definition {
      "id",
      "accounts", 
      Model::ColumnTypes({
        {"id",                   COL_TYPE(long)},
        {"password",             COL_TYPE(std::string)},
        #define COLUMN(a, t, _) {#a, COL_TYPE(t)},
          ACCOUNT_COLUMNS
        #undef COLUMN
      }),
      Model::Validations( {
        Model::Validates::NotNull("first_name"),
        Model::Validates::NotNull("last_name"),
        Model::Validates::NotNull("email"),
        Model::Validates::IsUnique("email"),
        Model::Validates::Matches("email", 
          prails::utilities::regex_from_string("/.+@.+/")), // TODO
        Model::Validates::NotNull("password"),
        Model::Validates::NotNull("created_at"),
        Model::Validates::NotNull("updated_at")
      })
    };

    static void Migrate();
    static std::optional<Account> FromLogin(const std::string &, const std::string &);
    static std::optional<Account> FromToken(const std::string &);
    static std::string Hash(const std::string &);

    std::optional<std::string> full_name();
    void password(const std::optional<Model::RecordValue> &);
    std::optional<std::string> password();
    void generate_new_auth_token();
    void clear_auth_token();

  private:
    static ModelRegister<Account> reg;
};
