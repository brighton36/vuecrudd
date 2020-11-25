#include "crm_position_task.hpp"

using namespace std;

REGISTER_MODEL(CrmPositionTask);

void CrmPositionTask::Migrate() {
  CreateTable({
    #define COLUMN(a, _, t) {#a, t},
    CRM_POSITION_TASK_COLUMNS
    #undef COLUMN
  });

  // TODO:
  //$table->unique(['position_id', 'task_id']);
};
