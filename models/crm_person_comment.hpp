#pragma once
#include <nlohmann/json.hpp>
#include "model.hpp"
#include "model_factory.hpp"

#define CRM_PERSON_COMMENT_COLUMNS                          \
  COLUMN(content,                std::string,   "text")     \
  COLUMN(user_id,                long long int, "int")      \
  COLUMN(person_id,              long long int, "int")      \
  COLUMN(person_comment_type_id, long long int, "int")      \
  COLUMN(active,                 int,           "int")      \
  COLUMN(created_at,             std::tm,       "datetime") \
  COLUMN(updated_at,             std::tm,       "datetime")

class CrmPersonComment : public Model::Instance<CrmPersonComment> { 
  public:
    using Model::Instance<CrmPersonComment>::Instance;

    MODEL_ACCESSOR(id, long long int)
    #define COLUMN(a, t, _) MODEL_ACCESSOR(a, t)
    CRM_PERSON_COMMENT_COLUMNS
    #undef COLUMN

    inline static const Model::Definition Definition {
      "id",
      "person_comments", 
      Model::ColumnTypes({
        {"id", COL_TYPE(long long int)},
        #define COLUMN(a, t, _) {#a, COL_TYPE(t)},
        CRM_PERSON_COMMENT_COLUMNS
        #undef COLUMN
      }),
      Model::Validations( {
        /*
         * TODO:
          'person_id' => 'required|exists:people,id',
          'person_comment_type_id' => 'required|exists:person_comment_types,id',
        */
        Model::Validates::NotNull("person_id"),
        Model::Validates::NotNull("person_comment_type_id"),
        Model::Validates::NotNull("content"),
        Model::Validates::MaxLength("content", 2000),
        Model::Validates::IsBoolean("active"),
        Model::Validates::NotNull("created_at"),
        Model::Validates::NotNull("updated_at")
      })
    };

    nlohmann::json to_json();
    static void Migrate();

  private:
    static ModelRegister<CrmPersonComment> reg;
};
