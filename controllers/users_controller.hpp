#pragma once
#include "rest_controller.hpp"
#include "user.hpp"

class UsersController : 
public Controller::RestInstance<UsersController, User> { 
  public:
    static constexpr std::string_view rest_prefix = { "/api/admin/users" };
    using Controller::RestInstance<UsersController, User>::RestInstance;

  private:
    static ControllerRegister<UsersController> reg;
};
