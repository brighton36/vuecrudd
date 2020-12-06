#pragma once
#include "rest_controller.hpp"
#include "crm_person.hpp"

class CrmPeopleController : 
public Controller::RestInstance<CrmPeopleController, CrmPerson> { 
  public:
    static constexpr std::string_view rest_prefix = { "/api/crm/people" };
    using Controller::RestInstance<CrmPeopleController, CrmPerson>::RestInstance;

  private:
		std::vector<CrmPerson> modelSelect(Controller::PostBody &);
    static ControllerRegister<CrmPeopleController> reg;
    // TODO: https://github.com/what-crud/laravel-crud-api/blob/master/templates/crm/app/Http/Controllers/Crm/PeopleController.php
};
