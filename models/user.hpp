#pragma once
#include "model.hpp"
#include "model_factory.hpp"

#define USER_COLUMNS                                    \
  COLUMN(name,             std::string, "varchar(255)") \
  COLUMN(email,            std::string, "varchar(255)") \
  COLUMN(initial_password, std::string, "varchar(255)") \
  COLUMN(user_type_id,     int,         "int")          \
  COLUMN(active,           int,         "int")          \
  COLUMN(created_at,       std::tm,     "datetime")     \
  COLUMN(updated_at,       std::tm,     "datetime")

class User : public Model::Instance<User> { 
  public:
    using Model::Instance<User>::Instance;

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

    static void Migrate();

  private:
    static ModelRegister<User> reg;
};
