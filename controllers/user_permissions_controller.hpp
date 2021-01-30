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

    Controller::Response index(const Pistache::Rest::Request&);

    static std::optional<std::string> prefix(const std::string &action) {
      // TODO: I'm not sure we're done yet. Keep testing...
      // This is just some odd exception to the routing patterns...
      if ((action == "read")) return "/api/crud/admin/user-permissions";
      return VuecrudController<UserPermissionsController, UserPermission>::prefix(action);
    }
  private:
    void modelUpdate(UserPermission &, Controller::PostBody &, std::tm);
    static ControllerRegister<UserPermissionsController> reg;
};
