#include "crm_person_comment_types_controller.hpp"

PSYM_CONTROLLER(CrmPersonCommentTypesController)

void CrmPersonCommentTypesController::Routes(
Pistache::Rest::Router& r, std::shared_ptr<Instance> controller) {
  using namespace Pistache::Rest::Routes;

  RestInstance<CrmPersonCommentTypesController, CrmPersonCommentType>::Routes(r,controller);
  RoutesVuecrudd<CrmPersonCommentTypesController>("/api/crud/crm/person-comment-type",r,controller);
}
