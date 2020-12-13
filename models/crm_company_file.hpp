#pragma once
#include "model.hpp"
#include "model_factory.hpp"

#define CRM_COMPANY_FILE_COLUMNS                   \
  COLUMN(file,        std::string, "varchar(200)") \
  COLUMN(file_2,      std::string, "varchar(200)") \
  COLUMN(company_id,  int,         "int")          \
  COLUMN(description, std::string, "varchar(200)") \
  COLUMN(active,      int,         "int")          \
  COLUMN(created_at,  std::tm,     "datetime")     \
  COLUMN(updated_at,  std::tm,     "datetime")

class CrmCompanyFile : public Model::Instance<CrmCompanyFile> { 
  public:
    using Model::Instance<CrmCompanyFile>::Instance;

    MODEL_ACCESSOR(id, long long int)
    #define COLUMN(a, t, _) MODEL_ACCESSOR(a, t)
      CRM_COMPANY_FILE_COLUMNS
    #undef COLUMN

    inline static const Model::Definition Definition {
      "id",
      "company_files", 
      Model::ColumnTypes({
        {"id", COL_TYPE(long long int)},
        #define COLUMN(a, t, _) {#a, COL_TYPE(t)},
        CRM_COMPANY_FILE_COLUMNS
        #undef COLUMN
      }),
      Model::Validations( {
        /*
        // TODO
        'company_id' => 'required|exists:companies,id',
         */
        Model::Validates::NotNull("company_id"),
        Model::Validates::NotNull("file"),

        Model::Validates::IsBoolean("active"),
        Model::Validates::NotNull("created_at"),
        Model::Validates::NotNull("updated_at")
      })
    };

    static void Migrate();

  private:
    static ModelRegister<CrmCompanyFile> reg;
};
