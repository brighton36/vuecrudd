#pragma once
#include "model.hpp"
#include "model_factory.hpp"

#define USER_PERMISSION_COLUMNS              \
  COLUMN(user_id,       int,     "int")      \
  COLUMN(permission_id, int,     "int")      \
  COLUMN(active,        int,     "int")      \
  COLUMN(created_at,    std::tm, "datetime") \
  COLUMN(updated_at,    std::tm, "datetime")

class UserPermission : public Model::Instance<UserPermission> { 
  public:
    using Model::Instance<UserPermission>::Instance;

    MODEL_ACCESSOR(id, long long int)
    #define COLUMN(a, t, _) MODEL_ACCESSOR(a, t)
    USER_PERMISSION_COLUMNS
    #undef COLUMN

    inline static const Model::Definition Definition {
      "id",
      "user_permissions", 
      Model::ColumnTypes({
        {"id", COL_TYPE(long long int)},
        #define COLUMN(a, t, _) {#a, COL_TYPE(t)},
        USER_PERMISSION_COLUMNS
        #undef COLUMN
      }),
      Model::Validations( {
        Model::Validates::NotNull("user_id"),
        Model::Validates::NotNull("permission_id"),
        Model::Validates::IsBoolean("active"),
        Model::Validates::NotNull("created_at"),
        Model::Validates::NotNull("updated_at")
      })
    };

    static void Migrate();

  private:
    static ModelRegister<UserPermission> reg;
};
