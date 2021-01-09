#pragma once
#include "rest_controller.hpp"
#include "crm_company_file.hpp"
#include "vuecrudd_utilities.hpp"

class CrmCompanyFilesController : 
public Controller::RestInstance<CrmCompanyFilesController, CrmCompanyFile> { 
  public:
    static constexpr std::string_view rest_prefix = { "/api/crm/company-files" };
    static constexpr std::string_view rest_actions[]= { "index", "read" };
    using Controller::RestInstance<CrmCompanyFilesController, CrmCompanyFile>::RestInstance;

    static void Routes(Pistache::Rest::Router&, std::shared_ptr<Controller::Instance>);
  private:
    void modelUpdate(CrmCompanyFile &model, Controller::PostBody &post, std::tm tm_time) {
      modelUpdateVuecrudd(model, post, tm_time);
      
      #define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
      CRM_COMPANY_FILE_COLUMNS
      #undef COLUMN
    }

    static ControllerRegister<CrmCompanyFilesController> reg;
};
