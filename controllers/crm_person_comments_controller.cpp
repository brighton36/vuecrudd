#include "crm_person_comments_controller.hpp"
#include "utilities.hpp"

PSYM_CONTROLLER(CrmPersonCommentsController)

using namespace std;
using namespace prails::utilities;

std::vector<CrmPersonComment> modelSelect(Controller::PostBody &) {
	return CrmPersonComment::Select( fmt::format(
    "select * from {table_name} {joins} order by {order_by}", 
		fmt::arg("table_name", CrmPersonComment::Definition.table_name),
		fmt::arg("joins", join({
			"join crm_positions on crm_positions.id = crm_position_tasks.position_id"
			"join crm_companies on crm_positions.company_id = crm_companies.id"
			"join crm_people on crm_positions.person_id = crm_people.id"
			"join crm_tasks on crm_tasks.id = crm_position_tasks.task_id"}, ",")),
		fmt::arg("order_by", join({ "crm_tasks.name asc", 
      "crm_companies.common_name asc", "crm_people.lastname asc", 
      "crm_people.firstname asc"}, ", "))
    ));
}
