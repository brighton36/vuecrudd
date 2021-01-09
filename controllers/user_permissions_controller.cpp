#include "user_permissions_controller.hpp"

PSYM_CONTROLLER(UserPermissionsController)

void UserPermissionsController::Routes(
Pistache::Rest::Router& r, std::shared_ptr<Instance> controller) {
  using namespace Pistache::Rest::Routes;

  RestInstance<UserPermissionsController, UserPermission>::Routes(r,controller);
  //RoutesVuecrudd<UserPermissionsController>("/api/crud/user-permissions",r,controller);
}
