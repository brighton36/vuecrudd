#pragma once
#include "vuecrud_controller.hpp"

#include "crm_company_type.hpp"

class CrmCompanyTypeController : 
public VuecrudController<CrmCompanyTypeController, CrmCompanyType> { 
  public:
    static constexpr std::string_view read_prefix = { "/api/crud/crm" };
    static constexpr std::string_view basename = { "company-types" };
    using VuecrudController<CrmCompanyTypeController, CrmCompanyType>::VuecrudController;
  private:
    void model_update(CrmCompanyType &, Controller::PostBody &, std::tm);

    static ControllerRegister<CrmCompanyTypeController> reg;
};
