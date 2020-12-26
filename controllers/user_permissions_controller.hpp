#pragma once
#include "rest_controller.hpp"
#include "user_permission.hpp"

class UserPermissionsController : 
public Controller::RestInstance<UserPermissionsController, UserPermission> { 
  public:
    static constexpr std::string_view rest_prefix = { "/api/admin/user-permissions" };
    using Controller::RestInstance<UserPermissionsController, UserPermission>::RestInstance;

  private:
    static ControllerRegister<UserPermissionsController> reg;
};
