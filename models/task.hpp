#pragma once
#include "model.hpp"

class Task : public Model::Instance<Task> { 
  public:
    MODEL_CONSTRUCTOR(Task)

    MODEL_ACCESSOR(id, long)
    MODEL_ACCESSOR(name, std::string)
    MODEL_ACCESSOR(active, int)
    MODEL_ACCESSOR(description, std::string)
    MODEL_ACCESSOR(created_at, std::tm)
    MODEL_ACCESSOR(updated_at, std::tm)

    inline static const Model::Definition Definition {
      "id",
      "tasks", 
      Model::ColumnTypes({
        {"id",          COL_TYPE(long)},
        {"name",        COL_TYPE(std::string)},
        {"active",      COL_TYPE(int)},
        {"description", COL_TYPE(std::string)},
        {"created_at",  COL_TYPE(std::tm)},
        {"updated_at",  COL_TYPE(std::tm)}
      }),
      Model::Validations( {
        Model::Validates::NotNull("name"),
        Model::Validates::NotEmpty("name"),
        Model::Validates::NotNull("active"),
        Model::Validates::IsBoolean("active"),
        Model::Validates::NotNull("created_at"),
        Model::Validates::NotNull("updated_at")
      })
    };

    static void Migrate() {
      CreateTable({
        {"name", "varchar(100)"},
        {"active", "integer"},
        {"description", "varchar(300)"},
        {"created_at", "datetime"},
        {"updated_at", "datetime"},
      });
    };
};
