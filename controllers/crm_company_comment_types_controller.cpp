#include "crm_company_comment_types_controller.hpp"

PSYM_CONTROLLER(CrmCompanyCommentTypesController)

void CrmCompanyCommentTypesController::Routes(
Pistache::Rest::Router& r, std::shared_ptr<Instance> controller) {
  using namespace Pistache::Rest::Routes;

  RestInstance<CrmCompanyCommentTypesController, CrmCompanyCommentType>::Routes(r,controller);
  //RoutesVuecrudd<CrmCompanyCommentTypesController>("/api/crud/crm/company-comment-types",r,controller);
}
