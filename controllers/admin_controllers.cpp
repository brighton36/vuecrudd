#include "users_controller.hpp"
#include "user_types_controller.hpp"
#include "permissions_controller.hpp"
#include "user_permissions_controller.hpp"

using namespace std;
using namespace Pistache::Rest;
using namespace prails::utilities;
using namespace Controller;

PSYM_CONTROLLER(UsersController)
PSYM_CONTROLLER(UserTypesController)
PSYM_CONTROLLER(PermissionsController)
PSYM_CONTROLLER(UserPermissionsController)

#define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
CONTROLLER_UPDATE(UsersController, User, USER_COLUMNS)
CONTROLLER_UPDATE(PermissionsController, Permission, PERMISSION_COLUMNS)
CONTROLLER_UPDATE(UserPermissionsController, UserPermission, USER_PERMISSION_COLUMNS)
#undef COLUMN

Response UsersController::index(const Request& request) {
	User user = ensure_authorization<User>(request, "index");
  auto post = PostBody(request.body());
  auto users = model_index(user);

  return Response(ModelToJson(users, vector<JsonDecorator<User>>({
    with<long long int, UserType>(users, "user_type", "user_type_id")
    })));
}

Response UserPermissionsController::index(const Request& request) {
	User user = ensure_authorization<User>(request, "index");
  auto post = PostBody(request.body());
  auto user_permissions = model_index(user);

  return Response(ModelToJson(user_permissions, vector<JsonDecorator<UserPermission>>({
    with<long long int, User>(user_permissions, "user", "user_id"),
    with<long long int, Permission>(user_permissions, "permission", "permission_id")
    })));
}
