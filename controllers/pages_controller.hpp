#pragma once
#include "controller.hpp"
#include "controller_factory.hpp"

class PagesController : public Controller::Instance { 
  public:
    using Instance::Instance;
    static void Routes(Pistache::Rest::Router&, std::shared_ptr<Controller::Instance>);

    Controller::Response index(const Pistache::Rest::Request&);
    Controller::Response login(const Pistache::Rest::Request&);
    Controller::Response app(const Pistache::Rest::Request&);
  private:
    static ControllerRegister<PagesController> reg;
};

