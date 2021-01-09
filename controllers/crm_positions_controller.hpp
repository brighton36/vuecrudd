#pragma once
#include "rest_controller.hpp"
#include "crm_position.hpp"
#include "vuecrudd_utilities.hpp"

class CrmPositionsController : 
public Controller::RestInstance<CrmPositionsController, CrmPosition> { 
  public:
    static constexpr std::string_view rest_prefix = { "/api/crm/positions" };
    static constexpr std::string_view rest_actions[]= { "index", "read" };
    using Controller::RestInstance<CrmPositionsController, CrmPosition>::RestInstance;
		// TODO: what's up with:
		// https://github.com/what-crud/laravel-crud-api/blob/master/templates/crm/app/Http/Controllers/Crm/PositionsController.php
    //	public function positionTasks(Request $request, $id)
    static void Routes(Pistache::Rest::Router&, std::shared_ptr<Controller::Instance>);

  private:
    void modelUpdate(CrmPosition &model, Controller::PostBody &post, std::tm tm_time) {
      modelUpdateVuecrudd(model, post, tm_time);
 
      #define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
      CRM_POSITION_COLUMNS
      #undef COLUMN
    }
		std::vector<CrmPosition> modelSelect(Controller::PostBody &);
    static ControllerRegister<CrmPositionsController> reg;
};
