#pragma once
#include "vuecrud_controller.hpp"

#include "permission.hpp"

class AdminPermissionsController : 
public VuecrudController<AdminPermissionsController, Permission> { 
  public:
    static constexpr std::string_view basename = { "permissions" };
    static constexpr std::string_view read_prefix = { "/api/crud/admin" };
    static constexpr std::string_view rest_prefix = { "/api/crud/admin" };
    using VuecrudController<AdminPermissionsController, Permission>::VuecrudController;
  private:
    void model_update(Permission &, Controller::PostBody &, std::tm);
    static ControllerRegister<AdminPermissionsController> reg;
};
