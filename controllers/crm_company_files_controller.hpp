#pragma once
#include "vuecrud_controller.hpp"

#include "crm_company_file.hpp"

class CrmCompanyFilesController : 
public VuecrudController<CrmCompanyFilesController, CrmCompanyFile> { 
  public:
    static constexpr std::string_view basename = { "company-files" };
    using VuecrudController<CrmCompanyFilesController, CrmCompanyFile>::VuecrudController;
  private:
    void modelUpdate(CrmCompanyFile &model, Controller::PostBody &post, std::tm tm_time) {
      modelUpdateVuecrud(model, post, tm_time);
      
      #define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
      CRM_COMPANY_FILE_COLUMNS
      #undef COLUMN
    }

    static ControllerRegister<CrmCompanyFilesController> reg;
};
