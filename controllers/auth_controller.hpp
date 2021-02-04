#pragma once
#include "controller.hpp"
#include "controller_factory.hpp"

class AuthController : public Controller::Instance { 
  public:
    using Instance::Instance;
    static void Routes(Pistache::Rest::Router&, std::shared_ptr<Controller::Instance>);

    Controller::Response user_show(const Pistache::Rest::Request&);
    Controller::Response user_update(const Pistache::Rest::Request&);
    Controller::Response user_password(const Pistache::Rest::Request&);
    Controller::Response options(const Pistache::Rest::Request&);
    Controller::Response login(const Pistache::Rest::Request&);
    Controller::Response logout(const Pistache::Rest::Request&);

  private:
    static ControllerRegister<AuthController> reg;
};

