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

  protected:
    std::vector<CrmPosition> model_index(User &);
    void model_update(CrmPosition &, Controller::PostBody &, std::tm);

    static ControllerRegister<CrmPositionsController> reg;
};
