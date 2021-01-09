#include "crm_person_comments_controller.hpp"
#include "utilities.hpp"

PSYM_CONTROLLER(CrmPersonCommentsController)

void CrmPersonCommentsController::Routes(
Pistache::Rest::Router& r, std::shared_ptr<Instance> controller) {
  using namespace Pistache::Rest::Routes;

  RestInstance<CrmPersonCommentsController, CrmPersonComment>::Routes(r,controller);
  RoutesVuecrudd<CrmPersonCommentsController>("/api/crud/crm/person-comments",r,controller);
}
