#pragma once
#include "rest_controller.hpp"
#include "crm_company.hpp"
#include "vuecrudd_utilities.hpp"

class CrmCompanyController : 
public Controller::RestInstance<CrmCompanyController, CrmCompany> { 
  public:
    using Controller::RestInstance<CrmCompanyController, CrmCompany>::RestInstance;

    static constexpr std::string_view rest_prefix = { "/api/crm/companies" };
    static constexpr std::string_view rest_actions[]= { "index", "read" };

    Controller::Response index(const Pistache::Rest::Request&);
    static void Routes(Pistache::Rest::Router&, std::shared_ptr<Controller::Instance>);

  private:

    void modelUpdate(CrmCompany &model, Controller::PostBody &post, std::tm tm_time) {
      modelUpdateVuecrudd(model, post, tm_time);
      
      #define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
      CRM_COMPANY_COLUMNS
      #undef COLUMN
    }

    static ControllerRegister<CrmCompanyController> reg;
};
