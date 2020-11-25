#pragma once
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
    MODEL_CONSTRUCTOR(CrmStreetPrefix)

    MODEL_ACCESSOR(id, long)
    #define COLUMN(a, t, _) MODEL_ACCESSOR(a, t)
    CRM_STREET_PREFIX_COLUMNS
    #undef COLUMN

    inline static const Model::Definition Definition {
      "id",
      "crm_street_prefixes", 
      Model::ColumnTypes({
        {"id", COL_TYPE(long)},
        #define COLUMN(a, t, _) {#a, COL_TYPE(t)},
        CRM_STREET_PREFIX_COLUMNS
        #undef COLUMN
      }),
      Model::Validations( {
        Model::Validates::NotNull("name"),
        Model::Validates::MaxLength("name", 10),
        Model::Validates::NotNull("description"),
        Model::Validates::MaxLength("description", 100),
        Model::Validates::IsBoolean("active"),
        Model::Validates::NotNull("created_at"),
        Model::Validates::NotNull("updated_at")
      })
    };

    static void Migrate();

  private:
    static ModelRegister<CrmStreetPrefix> reg;
};
