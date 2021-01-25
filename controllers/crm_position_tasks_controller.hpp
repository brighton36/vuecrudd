#pragma once
#include "vuecrud_controller.hpp"

#include "crm_position_task.hpp"

class CrmPositionTasksController : 
public VuecrudController<CrmPositionTasksController, CrmPositionTask> { 
  public:
    static constexpr std::string_view basename = { "position-tasks" };
    using VuecrudController<CrmPositionTasksController, CrmPositionTask>::VuecrudController;
  private:
    void modelUpdate(CrmPositionTask &model, Controller::PostBody &post, std::tm tm_time) {
      modelUpdateVuecrud(model, post, tm_time);
      #define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
      CRM_POSITION_TASK_COLUMNS
      #undef COLUMN
    }
    static ControllerRegister<CrmPositionTasksController> reg;
};
