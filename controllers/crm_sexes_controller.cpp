#include "crm_sexes_controller.hpp"

PSYM_CONTROLLER(CrmSexesController)

void CrmSexesController::Routes(
Pistache::Rest::Router& r, std::shared_ptr<Instance> controller) {
  using namespace Pistache::Rest::Routes;

  RestInstance<CrmSexesController, CrmSex>::Routes(r,controller);
  RoutesVuecrudd<CrmSexesController>("/api/crud/crm/sexes",r,controller);
}
