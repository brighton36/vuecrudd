#pragma once
#include "rest_controller.hpp"
#include "crm_position_task.hpp"

class CrmPositionTasksController : 
public Controller::RestInstance<CrmPositionTasksController, CrmPositionTask> { 
  public:
    static constexpr std::string_view rest_prefix = { "/api/crm/position_tasks" };
    using Controller::RestInstance<CrmPositionTasksController, CrmPositionTask>::RestInstance;

  private:
		std::vector<CrmPositionTask> modelSelect(Controller::PostBody &);
    static ControllerRegister<CrmPositionTasksController> reg;
};
