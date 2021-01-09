#include "permissions_controller.hpp"

PSYM_CONTROLLER(PermissionsController)

void PermissionsController::Routes(
Pistache::Rest::Router& r, std::shared_ptr<Instance> controller) {
  using namespace Pistache::Rest::Routes;

  RestInstance<PermissionsController, Permission>::Routes(r,controller);
  // RoutesVuecrudd<PermissionsController>("/api/crud/permissions",r,controller);
}
