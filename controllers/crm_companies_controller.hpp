#pragma once
#include "rest_controller.hpp"
#include "crm_company.hpp"

class CrmCompanyController : 
public Controller::RestInstance<CrmCompanyController, CrmCompany> { 
  public:
    static constexpr std::string_view rest_prefix = { "/api/crm/companies" };
    using Controller::RestInstance<CrmCompanyController, CrmCompany>::RestInstance;

  private:
    static ControllerRegister<CrmCompanyController> reg;
};
