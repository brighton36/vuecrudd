#pragma once
#include "vuecrud_controller.hpp"

#include "permission.hpp"

class PermissionsController : 
public VuecrudController<PermissionsController, Permission> { 
  public:
    static constexpr std::string_view basename = { "permissions" };
    static constexpr std::string_view read_prefix = { "/api/crud/admin" };
    static constexpr std::string_view rest_prefix = { "/api/crud/admin" };
    using VuecrudController<PermissionsController, Permission>::VuecrudController;
  private:
    void model_update(Permission &, Controller::PostBody &, std::tm);
    static ControllerRegister<PermissionsController> reg;
};
