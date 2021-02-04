#pragma once
#include "vuecrud_controller.hpp"

#include "crm_position_task.hpp"

class CrmPositionTasksController : 
public VuecrudController<CrmPositionTasksController, CrmPositionTask> { 
  public:
    static constexpr std::string_view basename = { "position-tasks" };
    static constexpr std::string_view rest_prefix = { "/api/crm" };
    using VuecrudController<CrmPositionTasksController, CrmPositionTask>::VuecrudController;
    Controller::Response index(const Pistache::Rest::Request&);
  private:
    void model_update(CrmPositionTask &, Controller::PostBody &, std::tm);

    static ControllerRegister<CrmPositionTasksController> reg;
};
