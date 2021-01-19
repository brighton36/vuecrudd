#pragma once
#include "vuecrud_controller.hpp"

#include "crm_person.hpp"

class CrmPeopleController : 
public VuecrudController<CrmPeopleController, CrmPerson> { 
  public:
    static constexpr std::string_view basename = { "people" };
    using VuecrudController<CrmPeopleController, CrmPerson>::VuecrudController;
    // TODO: Do we even need an index() ?
    Controller::Response search(const Pistache::Rest::Request&);

    static void Routes(Pistache::Rest::Router&, std::shared_ptr<Controller::Instance>);
  private:
    void modelUpdate(CrmPerson &model, Controller::PostBody &post, std::tm tm_time) {
      modelUpdateVuecrud(model, post, tm_time);
 
      #define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
      CRM_PERSON_COLUMNS
      #undef COLUMN
    }
		std::vector<CrmPerson> modelSelect(Controller::PostBody &);

    static ControllerRegister<CrmPeopleController> reg;
    // TODO: https://github.com/what-crud/laravel-crud-api/blob/master/templates/crm/app/Http/Controllers/Crm/PeopleController.php
};
