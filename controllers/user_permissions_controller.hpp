#pragma once
#include "vuecrud_controller.hpp"

#include "user_permission.hpp"

class UserPermissionsController : 
public VuecrudController<UserPermissionsController, UserPermission> { 
  public:
    static constexpr std::string_view basename = { "user-permissions" };
    using VuecrudController<UserPermissionsController, UserPermission>::VuecrudController;
  private:
    void modelUpdate(UserPermission &model, Controller::PostBody &post, std::tm tm_time) {
      modelUpdateVuecrud(model, post, tm_time);
      #define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
      USER_PERMISSION_COLUMNS
      #undef COLUMN
    }
    static ControllerRegister<UserPermissionsController> reg;
};
