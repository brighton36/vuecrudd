#include "crm_position_tasks_controller.hpp"
#include "utilities.hpp"

PSYM_CONTROLLER(CrmPositionTasksController)

using namespace std;
using namespace prails::utilities;

vector<CrmPositionTask> CrmPositionTasksController::modelSelect(Controller::PostBody &) {
	return CrmPositionTask::Select( fmt::format(
    "select * from {table_name} {joins} order by {order_by}", 
		fmt::arg("table_name", CrmPositionTask::Definition.table_name),
		fmt::arg("joins", join({
			"join positions on positions.id = position_tasks.position_id",
			"join companies on positions.company_id = companies.id",
			"join people on positions.person_id = people.id",
			"join tasks on tasks.id = position_tasks.task_id"}, " ")),
		fmt::arg("order_by", join({ "tasks.name asc", 
      "companies.common_name asc", "people.lastname asc", 
      "people.firstname asc"}, ", "))
    ));
}
