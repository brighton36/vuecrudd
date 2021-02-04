#pragma once
#include "vuecrud_controller.hpp"

#include "crm_sex.hpp"

class CrmSexesController : 
public VuecrudController<CrmSexesController, CrmSex> { 
  public:
    static constexpr std::string_view basename = { "sexes" };
    using VuecrudController<CrmSexesController, CrmSex>::VuecrudController;
  private:
    void model_update(CrmSex &, Controller::PostBody &, std::tm);

    static ControllerRegister<CrmSexesController> reg;
};
