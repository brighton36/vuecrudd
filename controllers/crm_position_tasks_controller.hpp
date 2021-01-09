#pragma once
#include "rest_controller.hpp"
#include "crm_position_task.hpp"
#include "vuecrudd_utilities.hpp"

class CrmPositionTasksController : 
public Controller::RestInstance<CrmPositionTasksController, CrmPositionTask> { 
  public:
    static constexpr std::string_view rest_prefix = { "/api/crm/position-tasks" };
    static constexpr std::string_view rest_actions[]= { "index", "read" };
    using Controller::RestInstance<CrmPositionTasksController, CrmPositionTask>::RestInstance;
    static void Routes(Pistache::Rest::Router&, std::shared_ptr<Controller::Instance>);

  private:
    void modelUpdate(CrmPositionTask &model, Controller::PostBody &post, std::tm tm_time) {
      modelUpdateVuecrudd(model, post, tm_time);
 
      #define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
      CRM_POSITION_TASK_COLUMNS
      #undef COLUMN
    }
		std::vector<CrmPositionTask> modelSelect(Controller::PostBody &);
    static ControllerRegister<CrmPositionTasksController> reg;
};
