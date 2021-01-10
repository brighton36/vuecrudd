#pragma once
#include "vuecrud_controller.hpp"

#include "crm_company.hpp"

class CrmCompanyController : 
public VuecrudController<CrmCompanyController, CrmCompany> { 
  public:
    static constexpr std::string_view basename = { "companies" };
    using VuecrudController<CrmCompanyController, CrmCompany>::VuecrudController;
    Controller::Response index(const Pistache::Rest::Request&);
  private:
    void modelUpdate(CrmCompany &model, Controller::PostBody &post, std::tm tm_time) {
      modelUpdateVuecrud(model, post, tm_time);
      #define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
      CRM_COMPANY_COLUMNS
      #undef COLUMN
    }

    static ControllerRegister<CrmCompanyController> reg;
};
