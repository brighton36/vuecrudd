#pragma once
#include "rest_controller.hpp"
#include "user.hpp"
#include "vuecrudd_utilities.hpp"

class UsersController : 
public Controller::RestInstance<UsersController, User> { 
  public:
    static constexpr std::string_view rest_prefix = { "/api/admin/users" };
    static constexpr std::string_view rest_actions[]= { "index", "read" };
    using Controller::RestInstance<UsersController, User>::RestInstance;
    static void Routes(Pistache::Rest::Router&, std::shared_ptr<Controller::Instance>);

  private:
    void modelUpdate(User &model, Controller::PostBody &post, std::tm tm_time) {
      modelUpdateVuecrudd(model, post, tm_time);
 
      #define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
      USER_COLUMNS
      #undef COLUMN
    }
    static ControllerRegister<UsersController> reg;
};
