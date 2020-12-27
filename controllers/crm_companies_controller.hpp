#pragma once
#include "rest_controller.hpp"
#include "crm_company.hpp"

class CrmCompanyController : 
public Controller::RestInstance<CrmCompanyController, CrmCompany> { 
  public:
    using Controller::RestInstance<CrmCompanyController, CrmCompany>::RestInstance;

    static constexpr std::string_view rest_prefix = { "/api/crm/companies" };

    Controller::Response index(const Pistache::Rest::Request&);

  private:
    static ControllerRegister<CrmCompanyController> reg;
};
