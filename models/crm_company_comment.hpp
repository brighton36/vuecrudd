#pragma once
#include "model.hpp"
#include "model_factory.hpp"

#define CRM_COMPANY_COMMENT_COLUMNS                        \
  COLUMN(content,                 std::string, "text")     \
  COLUMN(user_id,                 int,         "int")      \
  COLUMN(company_id,              int,         "int")      \
  COLUMN(company_comment_type_id, int,         "int")      \
  COLUMN(active,                  int,         "int")      \
  COLUMN(created_at,              std::tm,     "datetime") \
  COLUMN(updated_at,              std::tm,     "datetime")

class CrmCompanyComment : public Model::Instance<CrmCompanyComment> { 
  public:
    MODEL_CONSTRUCTOR(CrmCompanyComment)

    MODEL_ACCESSOR(id, long)
    #define COLUMN(a, t, _) MODEL_ACCESSOR(a, t)
      CRM_COMPANY_COMMENT_COLUMNS
    #undef COLUMN

    inline static const Model::Definition Definition {
      "id",
      "crm_company_comments", 
      Model::ColumnTypes({
        {"id", COL_TYPE(long)},
        #define COLUMN(a, t, _) {#a, COL_TYPE(t)},
        CRM_COMPANY_COMMENT_COLUMNS
        #undef COLUMN
      }),
      Model::Validations( {
        /*
        // TODO
        'company_id' => 'required|exists:companies,id',
        'company_comment_type_id' => 'required|exists:company_comment_types,id',
         */
        Model::Validates::NotNull("company_id"),
        Model::Validates::NotNull("company_comment_id"),
        Model::Validates::NotNull("content"),
        Model::Validates::MaxLength("content", 2000),

        Model::Validates::IsBoolean("active"),
        Model::Validates::NotNull("created_at"),
        Model::Validates::NotNull("updated_at")
      })
    };

    static void Migrate();

  private:
    static ModelRegister<CrmCompanyComment> reg;
};
