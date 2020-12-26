#pragma once
#include "rest_controller.hpp"
#include "permission.hpp"

class PermissionsController : 
public Controller::RestInstance<PermissionsController, Permission> { 
  public:
    static constexpr std::string_view rest_prefix = { "/api/crud/admin/permissions" };
    using Controller::RestInstance<PermissionsController, Permission>::RestInstance;

  private:
    static ControllerRegister<PermissionsController> reg;
};
