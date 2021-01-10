#pragma once
#include "vuecrud_controller.hpp"

#include "crm_company_type.hpp"

class CrmCompanyTypeController : 
public VuecrudController<CrmCompanyTypeController, CrmCompanyType> { 
  public:
    static constexpr std::string_view basename = { "company-types" };
    using VuecrudController<CrmCompanyTypeController, CrmCompanyType>::VuecrudController;
  private:
    void modelUpdate(CrmCompanyType &model, Controller::PostBody &post, std::tm tm_time) {
      modelUpdateVuecrud(model, post, tm_time);
      #define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
      CRM_COMPANY_TYPE_COLUMNS
      #undef COLUMN
    }

    static ControllerRegister<CrmCompanyTypeController> reg;
};
