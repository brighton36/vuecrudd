#pragma once
#include "model.hpp"
#include "model_factory.hpp"

#define CRM_COMPANY_TYPE_COLUMNS                  \
  COLUMN(name,       std::string, "varchar(200)") \
  COLUMN(code,       std::string, "varchar(3)")   \
  COLUMN(active,     int,         "int")          \
  COLUMN(created_at, std::tm,     "datetime")     \
  COLUMN(updated_at, std::tm,     "datetime")

class CrmCompanyType : public Model::Instance<CrmCompanyType> { 
  public:
    using Model::Instance<CrmCompanyType>::Instance;

    MODEL_ACCESSOR(id, long long int)
    #define COLUMN(a, t, _) MODEL_ACCESSOR(a, t)
      CRM_COMPANY_TYPE_COLUMNS
    #undef COLUMN

    inline static const Model::Definition Definition {
      "id",
      "company_types", 
      Model::ColumnTypes({
        {"id", COL_TYPE(long long int)},
        #define COLUMN(a, t, _) {#a, COL_TYPE(t)},
        CRM_COMPANY_TYPE_COLUMNS
        #undef COLUMN
      }),
      Model::Validations( {
        Model::Validates::NotNull("name"),
        Model::Validates::MaxLength("name", 200),
        Model::Validates::NotNull("code"),
        // TODO: What's this supposed to be?
        //Model::Validates::MaxLength("name", 3),

        Model::Validates::IsBoolean("active"),
        Model::Validates::NotNull("created_at"),
        Model::Validates::NotNull("updated_at")
      })
    };

    static void Migrate();

  private:
    static ModelRegister<CrmCompanyType> reg;
};
