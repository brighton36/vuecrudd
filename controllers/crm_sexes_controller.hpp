#pragma once
#include "rest_controller.hpp"
#include "crm_sex.hpp"
#include "vuecrudd_utilities.hpp"

class CrmSexesController : 
public Controller::RestInstance<CrmSexesController, CrmSex> { 
  public:
    static constexpr std::string_view rest_prefix = { "/api/crm/sexes" };
    static constexpr std::string_view rest_actions[] = { "index" }; 
    using Controller::RestInstance<CrmSexesController, CrmSex>::RestInstance;
    static void Routes(Pistache::Rest::Router&, std::shared_ptr<Controller::Instance>);
  private:
    void modelUpdate(CrmSex &model, Controller::PostBody &post, std::tm tm_time) {
      modelUpdateVuecrudd(model, post, tm_time);
 
      #define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
      CRM_SEX_COLUMNS
      #undef COLUMN
    }
    static ControllerRegister<CrmSexesController> reg;
};
