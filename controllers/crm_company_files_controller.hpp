#pragma once
#include "vuecrud_controller.hpp"

#include "crm_company_file.hpp"

class CrmCompanyFilesController : 
public VuecrudController<CrmCompanyFilesController, CrmCompanyFile> { 
  public:
    static constexpr std::string_view basename = { "company-files" };
    using VuecrudController<CrmCompanyFilesController, CrmCompanyFile>::VuecrudController;
  private:
    void modelUpdate(CrmCompanyFile &, Controller::PostBody &, std::tm);

    static ControllerRegister<CrmCompanyFilesController> reg;
};
