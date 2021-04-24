#pragma once
#include "vuecrud_controller.hpp"

#include "user.hpp"

class AdminUsersController : public VuecrudController<AdminUsersController, User> { 
  public:
    static constexpr std::string_view basename = { "users" };
    static constexpr std::string_view rest_prefix = { "/api/admin" };
    static constexpr std::string_view read_prefix = { "/api/admin" };
    using VuecrudController<AdminUsersController, User>::VuecrudController;
    Controller::Response index(const Pistache::Rest::Request&);

    static std::optional<std::string> prefix(const std::string &action) {
      // TODO: I'm not sure we're done yet. Keep testing...
      // This is just some odd exception to the routing patterns...
      if ((action == "read") || (action == "update") || (action == "multiple_update"))
        return "/api/crud/admin/users";
      return VuecrudController<AdminUsersController, User>::prefix(action);
    }
  private:
    void model_update(User &, Controller::PostBody &, std::tm);
    static ControllerRegister<AdminUsersController> reg;
};
