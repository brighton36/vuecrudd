#pragma once
#include "rest_controller.hpp"
#include "crm_position.hpp"

class CrmPositionsController : 
public Controller::RestInstance<CrmPositionsController, CrmPosition> { 
  public:
    static constexpr std::string_view rest_prefix = { "/api/crm/positions" };
    using Controller::RestInstance<CrmPositionsController, CrmPosition>::RestInstance;
		// TODO: what's up with:
		// https://github.com/what-crud/laravel-crud-api/blob/master/templates/crm/app/Http/Controllers/Crm/PositionsController.php
    //	public function positionTasks(Request $request, $id)

  private:
		std::vector<CrmPosition> modelSelect(Controller::PostBody &);
    static ControllerRegister<CrmPositionsController> reg;
};
