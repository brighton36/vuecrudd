#pragma once
#include "model.hpp"
#include "model_factory.hpp"

#define CRM_SEX_COLUMNS                          \
  COLUMN(name,       std::string, "varchar(50)") \
  COLUMN(code,       std::string, "varchar(1)")  \
  COLUMN(eng_name,   std::string, "varchar(50)") \
  COLUMN(eng_code,   std::string, "varchar(1)")  \
  COLUMN(priority,   int,         "int")         \
  COLUMN(active,     int,         "int")         \
  COLUMN(created_at, std::tm,     "datetime")    \
  COLUMN(updated_at, std::tm,     "datetime")

class CrmSex : public Model::Instance<CrmSex> { 
  public:
    using Model::Instance<CrmSex>::Instance;

    MODEL_ACCESSOR(id, long long int)
    #define COLUMN(a, t, _) MODEL_ACCESSOR(a, t)
    CRM_SEX_COLUMNS
    #undef COLUMN

    inline static const Model::Definition Definition {
      "id",
      "sexes", 
      Model::ColumnTypes({
        {"id", COL_TYPE(long long int)},
        #define COLUMN(a, t, _) {#a, COL_TYPE(t)},
        CRM_SEX_COLUMNS
        #undef COLUMN
      }),
      Model::Validations( {
        Model::Validates::MaxLength("name", 50),
        Model::Validates::MaxLength("code", 1),
        Model::Validates::MaxLength("eng_name", 50),
        Model::Validates::MaxLength("eng_code", 1),
        Model::Validates::IsBoolean("active"),
        Model::Validates::NotNull("created_at"),
        Model::Validates::NotNull("updated_at")
      })
    };

    static void Migrate();

  private:
    static ModelRegister<CrmSex> reg;
};
