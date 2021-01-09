#include "crm_company_comments_controller.hpp"

PSYM_CONTROLLER(CrmCompanyCommentsController)

void CrmCompanyCommentsController::Routes(
Pistache::Rest::Router& r, std::shared_ptr<Instance> controller) {
  using namespace Pistache::Rest::Routes;

  RestInstance<CrmCompanyCommentsController, CrmCompanyComment>::Routes(r,controller);
  RoutesVuecrudd<CrmCompanyCommentsController>("/api/crud/crm/company-comments",r,controller);
}
