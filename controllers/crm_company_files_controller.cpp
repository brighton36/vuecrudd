#include "crm_company_files_controller.hpp"

PSYM_CONTROLLER(CrmCompanyFilesController)

void CrmCompanyFilesController::Routes(
Pistache::Rest::Router& r, std::shared_ptr<Instance> controller) {
  using namespace Pistache::Rest::Routes;

  RestInstance<CrmCompanyFilesController, CrmCompanyFile>::Routes(r,controller);
  RoutesVuecrudd<CrmCompanyFilesController>("/api/crud/crm/company-files",r,controller);
}
