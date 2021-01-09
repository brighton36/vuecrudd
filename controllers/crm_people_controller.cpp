#include "crm_people_controller.hpp"

using namespace std;
using namespace prails::utilities;

PSYM_CONTROLLER(CrmPeopleController)

std::vector<CrmPerson> CrmPeopleController::modelSelect(Controller::PostBody &) {
	return CrmPerson::Select( fmt::format(
    "select * from {table_name} order by {order_by}", 
		fmt::arg("table_name", CrmPerson::Definition.table_name),
		fmt::arg("order_by", join({ "lastname asc", 
      "firstname asc", "distinction asc"}, ", "))
    ));
}

void CrmPeopleController::Routes(
Pistache::Rest::Router& r, std::shared_ptr<Instance> controller) {
  using namespace Pistache::Rest::Routes;

  RestInstance<CrmPeopleController, CrmPerson>::Routes(r,controller);
  RoutesVuecrudd<CrmPeopleController>("/api/crud/crm/people",r,controller);
}
