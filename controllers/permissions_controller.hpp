#pragma once
#include "vuecrud_controller.hpp"

#include "permission.hpp"

class PermissionsController : 
public VuecrudController<PermissionsController, Permission> { 
  public:
    static constexpr std::string_view basename = { "permissions" };
    using VuecrudController<PermissionsController, Permission>::VuecrudController;
  private:
    void modelUpdate(Permission &model, Controller::PostBody &post, std::tm tm_time) {
      modelUpdateVuecrud(model, post, tm_time);
      #define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
      PERMISSION_COLUMNS
      #undef COLUMN
    }
    static ControllerRegister<PermissionsController> reg;
};
