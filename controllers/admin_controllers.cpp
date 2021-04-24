#include "users_controller.hpp"
#include "user_types_controller.hpp"
#include "permissions_controller.hpp"
#include "user_permissions_controller.hpp"

using namespace std;
using namespace Pistache::Rest;
using namespace prails::utilities;
using namespace Controller;

PSYM_CONTROLLER(AdminUsersController)
PSYM_CONTROLLER(AdminUserTypesController)
PSYM_CONTROLLER(AdminPermissionsController)
PSYM_CONTROLLER(AdminUserPermissionsController)

#define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
CONTROLLER_UPDATE(AdminUsersController, User, USER_COLUMNS)
CONTROLLER_UPDATE(AdminPermissionsController, Permission, PERMISSION_COLUMNS)
CONTROLLER_UPDATE(AdminUserPermissionsController, UserPermission, USER_PERMISSION_COLUMNS)
#undef COLUMN

Response AdminUsersController::index(const Request& request) {
  User user = ensure_authorization<User>(request, "index");
  auto post = PostBody(request.body());
  auto users = model_index(user);

  return Response(ModelToJson(users, vector<JsonDecorator<User>>({
    with<long long int, UserType>(users, "user_type", "user_type_id")
    })));
}

Response AdminUserPermissionsController::index(const Request& request) {
  User user = ensure_authorization<User>(request, "index");
  auto post = PostBody(request.body());
  auto user_permissions = model_index(user);

  return Response(ModelToJson(user_permissions, vector<JsonDecorator<UserPermission>>({
    with<long long int, User>(user_permissions, "user", "user_id"),
    with<long long int, Permission>(user_permissions, "permission", "permission_id")
    })));
}
