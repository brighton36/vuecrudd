#include "model.hpp"
#include "model_factory.hpp"

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

    static void Migrate();
    static std::optional<Account> FromLogin(const std::string &email, const std::string &password);
    static std::optional<Account> FromToken(const std::string &token);
    static std::string Hash(const std::string &unsalted_phrase);

    std::optional<std::string> full_name();
    void password(const std::optional<Model::RecordValue> &val);
    std::optional<std::string> password();
    void generate_new_auth_token();
    void clear_auth_token();

  private:
    static ModelRegister<Account> reg;
};
