#include "crm_positions_controller.hpp"
#include "utilities.hpp"

PSYM_CONTROLLER(CrmPositionsController)

using namespace std;
using namespace prails::utilities;

std::vector<CrmPosition> modelSelect(Controller::PostBody &) {
	return CrmPosition::Select( fmt::format(
    "select * from {table_name} {joins} order by {order_by}", 
		fmt::arg("table_name", CrmPosition::Definition.table_name),
		fmt::arg("joins", join({
      "join crm_companies on crm_positions.company_id = companies.id", 
      "join crm_people on positions.person_id = people.id"}, ",")),
		fmt::arg("order_by", join({ "companies.common_name asc", 
      "people.lastname asc", "people.firstname asc"}, ", "))
    ));
}
