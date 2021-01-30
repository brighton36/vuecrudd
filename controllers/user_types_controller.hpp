#pragma once
#include "vuecrud_controller.hpp"

#include "user_type.hpp"

class UserTypesController : public VuecrudController<UserTypesController, UserType> { 
  public:
    static constexpr std::string_view basename = { "user-types" };
    static constexpr std::string_view read_prefix = { "/api/crud/admin" };
    static constexpr std::string_view rest_actions[]= { "index", "read" };
    using VuecrudController<UserTypesController, UserType>::VuecrudController;
  private:
    static ControllerRegister<UserTypesController> reg;
};
