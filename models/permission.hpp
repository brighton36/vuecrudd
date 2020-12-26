#pragma once
#include "model.hpp"
#include "model_factory.hpp"

#define PERMISSION_COLUMNS                        \
  COLUMN(name,       std::string, "varchar(255)") \
  COLUMN(code,       std::string, "varchar(10)")  \
  COLUMN(path,       std::string, "varchar(10)")  \
  COLUMN(active,     int,         "int")          \
  COLUMN(created_at, std::tm,     "datetime")     \
  COLUMN(updated_at, std::tm,     "datetime")

class Permission : public Model::Instance<Permission> { 
  public:
    using Model::Instance<Permission>::Instance;

    MODEL_ACCESSOR(id, long long int)
    #define COLUMN(a, t, _) MODEL_ACCESSOR(a, t)
    PERMISSION_COLUMNS
    #undef COLUMN

    inline static const Model::Definition Definition {
      "id",
      "permissions", 
      Model::ColumnTypes({
        {"id", COL_TYPE(long long int)},
        #define COLUMN(a, t, _) {#a, COL_TYPE(t)},
        PERMISSION_COLUMNS
        #undef COLUMN
      }),
      Model::Validations( {
        Model::Validates::NotNull("name"),
        Model::Validates::IsUnique("name"),
        Model::Validates::NotNull("code"),
        Model::Validates::IsUnique("code"),
        Model::Validates::MaxLength("code", 10),
        Model::Validates::IsUnique("path"),
        Model::Validates::NotNull("created_at"),
        Model::Validates::NotNull("updated_at")
      })
    };

    static void Migrate();

  private:
    static ModelRegister<Permission> reg;
};
