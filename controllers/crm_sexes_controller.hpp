#pragma once
#include "vuecrud_controller.hpp"

#include "crm_sex.hpp"

class CrmSexesController : 
public VuecrudController<CrmSexesController, CrmSex> { 
  public:
    static constexpr std::string_view basename = { "sexes" };
    using VuecrudController<CrmSexesController, CrmSex>::VuecrudController;
  private:
    void modelUpdate(CrmSex &model, Controller::PostBody &post, std::tm tm_time) {
      modelUpdateVuecrud(model, post, tm_time);
 
      #define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
      CRM_SEX_COLUMNS
      #undef COLUMN
    }
    static ControllerRegister<CrmSexesController> reg;
};
