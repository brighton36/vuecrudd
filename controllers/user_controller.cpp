#include "users_controller.hpp"

PSYM_CONTROLLER(UsersController)

void UsersController::Routes(
Pistache::Rest::Router& r, std::shared_ptr<Instance> controller) {
  using namespace Pistache::Rest::Routes;

  RestInstance<UsersController, User>::Routes(r,controller);
  // RoutesVuecrudd<UsersController>("/api/crud/users",r,controller);
}
