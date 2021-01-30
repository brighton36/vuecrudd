#include "users_controller.hpp"
#include "permissions_controller.hpp"
#include "user_permissions_controller.hpp"

PSYM_CONTROLLER(UsersController)
PSYM_CONTROLLER(PermissionsController)
PSYM_CONTROLLER(UserPermissionsController)

#define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
CONTROLLER_UPDATE(UsersController, User, USER_COLUMNS)
CONTROLLER_UPDATE(PermissionsController, Permission, PERMISSION_COLUMNS)
CONTROLLER_UPDATE(UserPermissionsController, UserPermission, USER_PERMISSION_COLUMNS)
#undef COLUMN
