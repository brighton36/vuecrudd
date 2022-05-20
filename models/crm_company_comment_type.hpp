#pragma once
#include <nlohmann/json.hpp>
#include "model.hpp"
#include "model_factory.hpp"

#define CRM_COMPANY_COMMENT_TYPE_COLUMNS          \
  COLUMN(name,       std::string, "varchar(200)") \
  COLUMN(active,     int,         "int")          \
  COLUMN(created_at, std::tm,     "datetime")     \
  COLUMN(updated_at, std::tm,     "datetime")

class CrmCompanyCommentType : public Model::Instance<CrmCompanyCommentType> { 
  public:
    using Model::Instance<CrmCompanyCommentType>::Instance;

    MODEL_ACCESSOR(id, long long int)
    #define COLUMN(a, t, _) MODEL_ACCESSOR(a, t)
    CRM_COMPANY_COMMENT_TYPE_COLUMNS
    #undef COLUMN

    inline static Model::Definition Definition {
      "id",
      "company_comment_types", 
      Model::ColumnTypes({
        {"id", COL_TYPE(long long int)},
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

    nlohmann::json to_json();
    static void Migrate(unsigned int);

  private:
    static ModelRegister<CrmCompanyCommentType> reg;
};
