#pragma once
#include "model.hpp"
#include "model_factory.hpp"

#define CRM_POSITION_COLUMNS                                            \
  COLUMN(person_id,   int,				 "int") \
  COLUMN(company_id,  int,				 "int") \
  COLUMN(name,        std::string, "varchar(255)") \
  COLUMN(phone,       std::string, "varchar(50)") \
  COLUMN(phone_2,     std::string, "varchar(50)") \
  COLUMN(phone_3,     std::string, "varchar(50)") \
  COLUMN(email,       std::string, "varchar(255)") \
  COLUMN(email_2,     std::string, "varchar(255)") \
  COLUMN(comment,     std::string, "varchar(500)") \
  COLUMN(active,      int,         "int")          \
  COLUMN(created_at,  std::tm,     "datetime")     \
  COLUMN(updated_at,  std::tm,     "datetime")

class CrmPosition : public Model::Instance<CrmPosition> { 
  public:
    MODEL_CONSTRUCTOR(CrmPosition)

    MODEL_ACCESSOR(id, long)
    #define COLUMN(a, t, _) MODEL_ACCESSOR(a, t)
      CRM_POSITION_COLUMNS
    #undef COLUMN

    inline static const Model::Definition Definition {
      "id",
      "crm_positions", 
      Model::ColumnTypes({
        {"id",                   COL_TYPE(long)},
        #define COLUMN(a, t, _) {#a, COL_TYPE(t)},
          CRM_POSITION_COLUMNS
        #undef COLUMN
      }),
      Model::Validations( {
        /*
        // TODO
        'person_id' => 'required|exists:people,id',
        'company_id' => 'required|exists:companies,id',
        'name' => 'string|max:255|nullable',
        'phone' => 'string|max:50|nullable',
        'phone_2' => 'string|max:50|nullable',
        'phone_3' => 'string|max:50|nullable',
        'email' => 'string|max:255|nullable',
        'email_2' => 'string|max:255|nullable',
        'comment' => 'string|max:500|nullable',
         */
        Model::Validates::NotNull("person_id"),
        Model::Validates::NotNull("company_id"),
        Model::Validates::Matches("email", 
          prails::utilities::regex_from_string("/.+@.+/")),
        Model::Validates::Matches("email2", 
          prails::utilities::regex_from_string("/.+@.+/")),
        Model::Validates::IsBoolean("active"),
        Model::Validates::NotNull("created_at"),
        Model::Validates::NotNull("updated_at")
      })
    };

    static void Migrate();

  private:
    static ModelRegister<CrmPosition> reg;
};
