#pragma once
#include "rest_controller.hpp"
#include "permission.hpp"
#include "vuecrudd_utilities.hpp"

class PermissionsController : 
public Controller::RestInstance<PermissionsController, Permission> { 
  public:
    static constexpr std::string_view rest_prefix = { "/api/crud/admin/permissions" };
    static constexpr std::string_view rest_actions[]= { "index", "read" };
    using Controller::RestInstance<PermissionsController, Permission>::RestInstance;
    static void Routes(Pistache::Rest::Router&, std::shared_ptr<Controller::Instance>);

  private:
    void modelUpdate(Permission &model, Controller::PostBody &post, std::tm tm_time) {
      modelUpdateVuecrudd(model, post, tm_time);
 
      #define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
      PERMISSION_COLUMNS
      #undef COLUMN
    }
    static ControllerRegister<PermissionsController> reg;
};
