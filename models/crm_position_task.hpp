#pragma once
#include <nlohmann/json.hpp>
#include "model.hpp"
#include "model_factory.hpp"

#define CRM_POSITION_TASK_COLUMNS           \
  COLUMN(position_id, long long int, "int") \
  COLUMN(task_id,     long long int, "int") \
  COLUMN(active,      int,           "int") \
  COLUMN(created_at,  std::tm, "datetime")  \
  COLUMN(updated_at,  std::tm, "datetime")

class CrmPositionTask : public Model::Instance<CrmPositionTask> { 
  public:
    using Model::Instance<CrmPositionTask>::Instance;

    MODEL_ACCESSOR(id, long long int)
    #define COLUMN(a, t, _) MODEL_ACCESSOR(a, t)
    CRM_POSITION_TASK_COLUMNS
    #undef COLUMN

    inline static const Model::Definition Definition {
      "id",
      "position_tasks", 
      Model::ColumnTypes({
        {"id", COL_TYPE(long long int)},
        #define COLUMN(a, t, _) {#a, COL_TYPE(t)},
        CRM_POSITION_TASK_COLUMNS
        #undef COLUMN
      }),
      Model::Validations( {
        /*
         * TODO:
         'position_id' => 'required|exists:positions,id',
         'task_id' => 'required|exists:tasks,id',
        */
        Model::Validates::NotNull("position_id"),
        Model::Validates::NotNull("task_id"),
        Model::Validates::IsBoolean("active"),
        Model::Validates::NotNull("created_at"),
        Model::Validates::NotNull("updated_at")
      })
    };

    nlohmann::json to_json();
    static void Migrate();

  private:
    static ModelRegister<CrmPositionTask> reg;
};
