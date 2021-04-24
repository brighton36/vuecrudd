#pragma once
#include "vuecrud_controller.hpp"

#include "user_type.hpp"

class AdminUserTypesController : public VuecrudController<AdminUserTypesController, UserType> { 
  public:
    static constexpr std::string_view basename = { "user-types" };
    static constexpr std::string_view read_prefix = { "/api/crud/admin" };
    static constexpr std::string_view rest_actions[]= { "index", "read" };
    using VuecrudController<AdminUserTypesController, UserType>::VuecrudController;
  private:
    static ControllerRegister<AdminUserTypesController> reg;
};
