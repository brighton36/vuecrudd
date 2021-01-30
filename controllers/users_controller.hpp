#pragma once
#include "vuecrud_controller.hpp"

#include "user.hpp"

class UsersController : public VuecrudController<UsersController, User> { 
  public:
    static constexpr std::string_view basename = { "users" };
    static constexpr std::string_view rest_prefix = { "/api/admin" };
    static constexpr std::string_view read_prefix = { "/api/admin" };
    using VuecrudController<UsersController, User>::VuecrudController;
  private:
    void modelUpdate(User &, Controller::PostBody &, std::tm);
    static ControllerRegister<UsersController> reg;
};
