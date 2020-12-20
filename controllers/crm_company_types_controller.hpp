#pragma once
#include "rest_controller.hpp"
#include "crm_company_type.hpp"

class CrmCompanyTypeController : 
public Controller::RestInstance<CrmCompanyTypeController, CrmCompanyType> { 
  public:
    using Controller::RestInstance<CrmCompanyTypeController, CrmCompanyType>::RestInstance;

    static constexpr std::string_view rest_prefix = { "/api/crud/crm/company-types" };

  private:
    static ControllerRegister<CrmCompanyTypeController> reg;
};
