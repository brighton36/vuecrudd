#pragma once
#include <nlohmann/json.hpp>
#include "model.hpp"
#include "model_factory.hpp"

#define CRM_LANGUAGE_COLUMNS                     \
  COLUMN(name,       std::string, "varchar(50)") \
  COLUMN(priority,   int,         "int")         \
  COLUMN(active,     int,         "int")         \
  COLUMN(created_at, std::tm,     "datetime")    \
  COLUMN(updated_at, std::tm,     "datetime")

class CrmLanguage : public Model::Instance<CrmLanguage> { 
  public:
    using Model::Instance<CrmLanguage>::Instance;

    MODEL_ACCESSOR(id, long long int)
    #define COLUMN(a, t, _) MODEL_ACCESSOR(a, t)
    CRM_LANGUAGE_COLUMNS
    #undef COLUMN

    inline static const Model::Definition Definition {
      "id",
      "languages", 
      Model::ColumnTypes({
        {"id", COL_TYPE(long long int)},
        #define COLUMN(a, t, _) {#a, COL_TYPE(t)},
        CRM_LANGUAGE_COLUMNS
        #undef COLUMN
      }),
      Model::Validations( {
        Model::Validates::NotNull("name"),
        Model::Validates::MaxLength("name", 50),
        Model::Validates::IsBoolean("active"),
        Model::Validates::NotNull("created_at"),
        Model::Validates::NotNull("updated_at")
      })
    };

    nlohmann::json to_json();
    static void Migrate();

  private:
    static ModelRegister<CrmLanguage> reg;
};
