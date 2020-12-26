#pragma once
#include "model.hpp"
#include "model_factory.hpp"

/*
 * Note that the laravel-crud-api uses "delete" instead of "del". We changed that
 * because delete() breaks the c++ compiler. 
 * Note that the laravel-crud-api uses "update" & "insert" instead of "upd" and 
 * "ins". We changed that because insert/update breaks the sqlite engine on
 * create table. 
 */
#define USER_TYPE_COLUMNS                         \
  COLUMN(name,       std::string, "varchar(255)") \
  COLUMN(read,       int,         "int")          \
  COLUMN(ins,        int,         "int")          \
  COLUMN(upd,        int,         "int")          \
  COLUMN(del,        int,         "int")          \
  COLUMN(admin,      int,         "int")          \
  COLUMN(active,     int,         "int")          \
  COLUMN(created_at, std::tm,     "datetime")     \
  COLUMN(updated_at, std::tm,     "datetime")

class UserType : public Model::Instance<UserType> { 
  public:
    using Model::Instance<UserType>::Instance;

    MODEL_ACCESSOR(id, long long int)
    #define COLUMN(a, t, _) MODEL_ACCESSOR(a, t)
    USER_TYPE_COLUMNS
    #undef COLUMN

    inline static const Model::Definition Definition {
      "id",
      "user_types", 
      Model::ColumnTypes({
        {"id", COL_TYPE(long long int)},
        #define COLUMN(a, t, _) {#a, COL_TYPE(t)},
        USER_TYPE_COLUMNS
        #undef COLUMN
      }),
      Model::Validations( {
        Model::Validates::NotNull("name"),
        Model::Validates::IsBoolean("read"),
        Model::Validates::IsBoolean("ins"),
        Model::Validates::IsBoolean("upd"),
        Model::Validates::IsBoolean("del"),
        Model::Validates::IsBoolean("admin"),
        Model::Validates::NotNull("created_at"),
        Model::Validates::NotNull("updated_at")
      })
    };

    static void Migrate();

  private:
    static ModelRegister<UserType> reg;
};
