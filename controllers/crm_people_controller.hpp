#pragma once
#include "rest_controller.hpp"
#include "crm_person.hpp"
#include "vuecrudd_utilities.hpp"

class CrmPeopleController : 
public Controller::RestInstance<CrmPeopleController, CrmPerson> { 
  public:
    static constexpr std::string_view rest_prefix = { "/api/crm/people" };
    static constexpr std::string_view rest_actions[]= { "index", "read" };
    using Controller::RestInstance<CrmPeopleController, CrmPerson>::RestInstance;

    static void Routes(Pistache::Rest::Router&, std::shared_ptr<Controller::Instance>);
  private:
    void modelUpdate(CrmPerson &model, Controller::PostBody &post, std::tm tm_time) {
      modelUpdateVuecrudd(model, post, tm_time);
 
      #define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
      CRM_PERSON_COLUMNS
      #undef COLUMN
    }
		std::vector<CrmPerson> modelSelect(Controller::PostBody &);

    static ControllerRegister<CrmPeopleController> reg;
    // TODO: https://github.com/what-crud/laravel-crud-api/blob/master/templates/crm/app/Http/Controllers/Crm/PeopleController.php
};
