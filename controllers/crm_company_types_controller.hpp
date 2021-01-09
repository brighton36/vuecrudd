#pragma once
#include "rest_controller.hpp"
#include "crm_company_type.hpp"
#include "vuecrudd_utilities.hpp"

class CrmCompanyTypeController : 
public Controller::RestInstance<CrmCompanyTypeController, CrmCompanyType> { 
  public:
    static constexpr std::string_view rest_prefix = { "/api/crud/crm/company-types" };
    static constexpr std::string_view rest_actions[]= { "index", "read" };
    using Controller::RestInstance<CrmCompanyTypeController, CrmCompanyType>::RestInstance;

    static void Routes(Pistache::Rest::Router&, std::shared_ptr<Controller::Instance>);
  private:
    void modelUpdate(CrmCompanyType &model, Controller::PostBody &post, std::tm tm_time) {
      modelUpdateVuecrudd(model, post, tm_time);
 
      #define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
      CRM_COMPANY_TYPE_COLUMNS
      #undef COLUMN
    }

    static ControllerRegister<CrmCompanyTypeController> reg;
};
