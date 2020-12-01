#pragma once
#include "model.hpp"
#include "model_factory.hpp"

#define CRM_COMPANY_COMMENT_TYPE_COLUMNS          \
  COLUMN(name,       std::string, "varchar(200)") \
  COLUMN(active,     int,         "int")          \
  COLUMN(created_at, std::tm,     "datetime")     \
  COLUMN(updated_at, std::tm,     "datetime")

class CrmCompanyCommentType : public Model::Instance<CrmCompanyCommentType> { 
  public:
    MODEL_CONSTRUCTOR(CrmCompanyCommentType)

    MODEL_ACCESSOR(id, long)
    #define COLUMN(a, t, _) MODEL_ACCESSOR(a, t)
      CRM_COMPANY_COMMENT_TYPE_COLUMNS
    #undef COLUMN

    inline static const Model::Definition Definition {
      "id",
      "crm_company_comment_types", 
      Model::ColumnTypes({
        {"id", COL_TYPE(long)},
        #define COLUMN(a, t, _) {#a, COL_TYPE(t)},
        CRM_COMPANY_COMMENT_TYPE_COLUMNS
        #undef COLUMN
      }),
      Model::Validations( {
        Model::Validates::NotNull("name"),
        Model::Validates::MaxLength("name", 200),
        Model::Validates::IsBoolean("active"),
        Model::Validates::NotNull("created_at"),
        Model::Validates::NotNull("updated_at")
      })
    };

    static void Migrate();

  private:
    static ModelRegister<CrmCompanyCommentType> reg;
};