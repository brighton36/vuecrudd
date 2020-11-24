#pragma once
#include "model.hpp"
#include "model_factory.hpp"

#define CRM_PERSON_COLUMNS                                            \
  COLUMN(firstname,   std::string, "varchar(128)") \
  COLUMN(lastname,    std::string, "varchar(128)") \
  COLUMN(distinction, std::string, "varchar(128)") \
  COLUMN(sex_id,      int,				 "int") \
  COLUMN(language_id, int,				 "int") \
  COLUMN(email,       std::string, "varchar(128)") \
  COLUMN(phone,       std::string, "varchar(128)") \
  COLUMN(active,      int,         "int")          \
  COLUMN(created_at,  std::tm,     "datetime")     \
  COLUMN(updated_at,  std::tm,     "datetime")

class CrmPerson : public Model::Instance<CrmPerson> { 
  public:
    MODEL_CONSTRUCTOR(CrmPerson)

    MODEL_ACCESSOR(id, long)
    #define COLUMN(a, t, _) MODEL_ACCESSOR(a, t)
      CRM_PERSON_COLUMNS
    #undef COLUMN

    inline static const Model::Definition Definition {
      "id",
      "crm_people", 
      Model::ColumnTypes({
        {"id",                   COL_TYPE(long)},
        #define COLUMN(a, t, _) {#a, COL_TYPE(t)},
          CRM_PERSON_COLUMNS
        #undef COLUMN
      }),
      Model::Validations( {
        /*
        // TODO
        'sex_id' => 'exists:sexes,id',
        'language_id' => 'exists:languages,id',
         */
        Model::Validates::Matches("email", 
          prails::utilities::regex_from_string("/.+@.+/")),
        Model::Validates::NotNull("firstname"),
        Model::Validates::NotNull("lastname"),
        Model::Validates::NotNull("sex_id"),
        Model::Validates::NotNull("language_id"),
        Model::Validates::IsBoolean("active"),
        Model::Validates::NotNull("created_at"),
        Model::Validates::NotNull("updated_at")
      })
    };

    static void Migrate();

  private:
    static ModelRegister<CrmPerson> reg;
};
