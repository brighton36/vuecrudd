#pragma once
#include "rest_controller.hpp"
#include "user_permission.hpp"
#include "vuecrudd_utilities.hpp"

class UserPermissionsController : 
public Controller::RestInstance<UserPermissionsController, UserPermission> { 
  public:
    static constexpr std::string_view rest_prefix = { "/api/admin/user-permissions" };
    static constexpr std::string_view rest_actions[]= { "index", "read" };
    using Controller::RestInstance<UserPermissionsController, UserPermission>::RestInstance;
    static void Routes(Pistache::Rest::Router&, std::shared_ptr<Controller::Instance>);

  private:
    void modelUpdate(UserPermission &model, Controller::PostBody &post, std::tm tm_time) {
      modelUpdateVuecrudd(model, post, tm_time);
 
      #define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
      USER_PERMISSION_COLUMNS
      #undef COLUMN
    }
    static ControllerRegister<UserPermissionsController> reg;
};
