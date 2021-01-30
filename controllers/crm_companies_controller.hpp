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
    void modelUpdate(CrmCompany &, Controller::PostBody &, std::tm);

    static ControllerRegister<CrmCompanyController> reg;
};
