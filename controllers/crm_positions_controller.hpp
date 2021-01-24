#pragma once
#include "vuecrud_controller.hpp"

#include "crm_position.hpp"

class CrmPositionsController : 
public VuecrudController<CrmPositionsController, CrmPosition> { 
  public:
    static constexpr std::string_view basename = { "positions" };
    static constexpr std::string_view rest_prefix = { "/api/crm" };
    using VuecrudController<CrmPositionsController, CrmPosition>::VuecrudController;
		// TODO: what's up with:
		// https://github.com/what-crud/laravel-crud-api/blob/master/templates/crm/app/Http/Controllers/Crm/PositionsController.php
    //	public function positionTasks(Request $request, $id)

  private:
    void modelUpdate(CrmPosition &model, Controller::PostBody &post, std::tm tm_time) {
      modelUpdateVuecrud(model, post, tm_time);
      #define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
      CRM_POSITION_COLUMNS
      #undef COLUMN
    }
		std::vector<CrmPosition> modelSelect(Controller::PostBody &);
    static ControllerRegister<CrmPositionsController> reg;
};
