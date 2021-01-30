#pragma once
#include "vuecrud_controller.hpp"

#include "user_permission.hpp"

class UserPermissionsController : 
public VuecrudController<UserPermissionsController, UserPermission> { 
  public:
    static constexpr std::string_view basename = { "user-permissions" };
    static constexpr std::string_view read_prefix = { "/api/admin" };
    static constexpr std::string_view rest_prefix = { "/api/crud/admin" };
    using VuecrudController<UserPermissionsController, UserPermission>::VuecrudController;
  private:
    void modelUpdate(UserPermission &, Controller::PostBody &, std::tm);
    static ControllerRegister<UserPermissionsController> reg;
};
