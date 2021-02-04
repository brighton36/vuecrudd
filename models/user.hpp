#pragma once
#include "model.hpp"
#include "model_factory.hpp"

// The auth_token is a bit sloppy, I should probably calculate the exact size. 
// But, roughly, 128 characters of randomness is 172 characters of base64
// Note that we omit password, since the accessor logic isn't as simple as the
// other columns
#define USER_COLUMNS                                      \
  COLUMN(name,             std::string,   "varchar(255)") \
  COLUMN(email,            std::string,   "varchar(255)") \
  COLUMN(initial_password, std::string,   "varchar(255)") \
  COLUMN(user_type_id,     long long int, "int")          \
  COLUMN(active,           int,           "int")          \
  COLUMN(auth_token,       std::string,   fmt::format("varchar({})",auth_token_size*2)) \
  COLUMN(auth_token_issued_at, std::tm,   "datetime")     \
  COLUMN(created_at,       std::tm,       "datetime")     \
  COLUMN(updated_at,       std::tm,       "datetime")

class User : public Model::Instance<User> { 
  public:
    using Model::Instance<User>::Instance;

    inline static const unsigned int auth_token_size = 128;

    MODEL_ACCESSOR(id, long long int)
    #define COLUMN(a, t, _) MODEL_ACCESSOR(a, t)
    USER_COLUMNS
    #undef COLUMN

    inline static const Model::Definition Definition {
      "id",
      "users", 
      Model::ColumnTypes({
        {"id",       COL_TYPE(long long int)},
        {"password", COL_TYPE(std::string)},
        #define COLUMN(a, t, _) {#a, COL_TYPE(t)},
        USER_COLUMNS
        #undef COLUMN
      }),
      Model::Validations( {
        Model::Validates::NotNull("name"),
        Model::Validates::MaxLength("name", 255),
        Model::Validates::NotNull("email"),
        Model::Validates::MaxLength("email", 255),
        Model::Validates::IsUnique("email"),
        Model::Validates::NotNull("user_type_id"),
        Model::Validates::NotNull("created_at"),
        Model::Validates::NotNull("updated_at")
      })
    };

    void generate_new_auth_token();
    void clear_auth_token();

    void password(const std::optional<Model::RecordValue> &val);
    std::optional<std::string> password();

    bool is_authorized(const std::string &, const std::string &);
    std::string authorizer_instance_label();
    static std::string Hash(const std::string &);

    static std::optional<User> FromLogin(const std::string &, const std::string &);
    static std::optional<User> FromHeader(std::optional<std::string>);
    static void Migrate();

  private:
    static ModelRegister<User> reg;
};
