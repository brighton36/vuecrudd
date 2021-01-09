#include "crm_company_types_controller.hpp"

PSYM_CONTROLLER(CrmCompanyTypeController)

void CrmCompanyTypeController::Routes(
Pistache::Rest::Router& r, std::shared_ptr<Instance> controller) {
  using namespace Pistache::Rest::Routes;

  RestInstance<CrmCompanyTypeController, CrmCompanyType>::Routes(r,controller);
  //RoutesVuecrudd<CrmCompanyTypeController>("/api/crud/crm/company-types",r,controller);
}
