#pragma once
#include <nlohmann/json.hpp>
#include "model.hpp"
#include "model_factory.hpp"

#define CRM_STREET_PREFIX_COLUMNS                  \
  COLUMN(name,        std::string, "varchar(10)")  \
  COLUMN(description, std::string, "varchar(100)") \
  COLUMN(active,      int,         "int")          \
  COLUMN(created_at,  std::tm,     "datetime")     \
  COLUMN(updated_at,  std::tm,     "datetime")

class CrmStreetPrefix : public Model::Instance<CrmStreetPrefix> { 
  public:
    using Model::Instance<CrmStreetPrefix>::Instance;

    MODEL_ACCESSOR(id, long long int)
    #define COLUMN(a, t, _) MODEL_ACCESSOR(a, t)
    CRM_STREET_PREFIX_COLUMNS
    #undef COLUMN

    inline static Model::Definition Definition {
      "id",
      "street_prefixes", 
      Model::ColumnTypes({
        {"id", COL_TYPE(long long int)},
        #define COLUMN(a, t, _) {#a, COL_TYPE(t)},
        CRM_STREET_PREFIX_COLUMNS
        #undef COLUMN
      }),
      Model::Validations( {
        Model::Validates::NotNull("name"),
        Model::Validates::MaxLength("name", 10),
        Model::Validates::MaxLength("description", 100),
        Model::Validates::IsBoolean("active"),
        Model::Validates::NotNull("created_at"),
        Model::Validates::NotNull("updated_at")
      })
    };

    nlohmann::json to_json();
    static void Migrate(unsigned int);

  private:
    static ModelRegister<CrmStreetPrefix> reg;
};
