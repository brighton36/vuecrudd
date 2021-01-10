#pragma once
#include "vuecrud_controller.hpp"

#include "user.hpp"

class UsersController : 
public VuecrudController<UsersController, User> { 
  public:
    static constexpr std::string_view basename = { "users" };
    using VuecrudController<UsersController, User>::VuecrudController;
  private:
    void modelUpdate(User &model, Controller::PostBody &post, std::tm tm_time) {
      modelUpdateVuecrud(model, post, tm_time);
      #define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
      USER_COLUMNS
      #undef COLUMN
    }
    static ControllerRegister<UsersController> reg;
};
