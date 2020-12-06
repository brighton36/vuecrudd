#pragma once
#include "rest_controller.hpp"
#include "crm_company_file.hpp"

class CrmCompanyFilesController : 
public Controller::RestInstance<CrmCompanyFilesController, CrmCompanyFile> { 
  public:
    static constexpr std::string_view rest_prefix = { "/api/crm/company_files" };
    using Controller::RestInstance<CrmCompanyFilesController, CrmCompanyFile>::RestInstance;

  private:
    static ControllerRegister<CrmCompanyFilesController> reg;
};
