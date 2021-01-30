#pragma once
#include "vuecrud_controller.hpp"

#include "crm_person.hpp"

class CrmPeopleController : 
public VuecrudController<CrmPeopleController, CrmPerson> { 
  public:
    static constexpr std::string_view basename = { "people" };
    using VuecrudController<CrmPeopleController, CrmPerson>::VuecrudController;
    Controller::Response search(const Pistache::Rest::Request&);

    static void Routes(Pistache::Rest::Router&, std::shared_ptr<Controller::Instance>);
    Controller::Response index(const Pistache::Rest::Request&);
  private:
    void modelUpdate(CrmPerson &, Controller::PostBody &, std::tm);

    static ControllerRegister<CrmPeopleController> reg;
};
