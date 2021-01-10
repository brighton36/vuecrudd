#include "utilities.hpp"

#include "crm_company_comments_controller.hpp"
#include "crm_company_comment_types_controller.hpp"
#include "crm_company_files_controller.hpp"
#include "crm_company_types_controller.hpp"
#include "crm_people_controller.hpp"
#include "crm_person_comments_controller.hpp"
#include "crm_person_comment_types_controller.hpp"
#include "crm_positions_controller.hpp"
#include "crm_position_tasks_controller.hpp"
#include "crm_sexes_controller.hpp"
#include "crm_street_prefixes_controller.hpp"
#include "crm_companies_controller.hpp"
#include "crm_company_type.hpp"
#include "crm_street_prefix.hpp"

using namespace std;
using namespace Pistache::Rest;
using namespace prails::utilities;
using namespace Controller;

PSYM_CONTROLLER(CrmCompanyController)
PSYM_CONTROLLER(CrmCompanyCommentsController)
PSYM_CONTROLLER(CrmCompanyCommentTypesController)
PSYM_CONTROLLER(CrmCompanyFilesController)
PSYM_CONTROLLER(CrmCompanyTypeController)
PSYM_CONTROLLER(CrmPeopleController)
PSYM_CONTROLLER(CrmPersonCommentsController)
PSYM_CONTROLLER(CrmPersonCommentTypesController)
PSYM_CONTROLLER(CrmPositionsController)
PSYM_CONTROLLER(CrmPositionTasksController)
PSYM_CONTROLLER(CrmSexesController)
PSYM_CONTROLLER(CrmStreetPrefixesController)

Response CrmCompanyController::index(const Request& request) {
  auto post = PostBody(request.body());
  auto ret = nlohmann::json::array();

  // TODO: I'm thinking we could support a rowjoin parameter, that took the with()'s as input
  // with maybe a keyname map<string,T> ("company_type", with<....>(...)
  // or, perhaps a ModelToJson with a parameter....
  // We could also speed this up a bit if we called this method once maybe, with a vector of joins...
  auto companies = modelSelect(post);
  auto company_types = with<long long int, CrmCompanyType>(companies, "company_type_id");
  auto street_prefixes = with<long long int, CrmStreetPrefix>(companies, "street_prefix_id");

  for (auto &m: companies) {
    auto model_as_json = ModelToJson(m);

    model_as_json["address"] = m.address();
    model_as_json["company_type"] = nullptr;
    model_as_json["street_prefix"] = nullptr;

    if (m.company_type_id().has_value()) { 
      long long int company_type_id = m.company_type_id().value();
      if (company_types.count(company_type_id))
        model_as_json["company_type"] = ModelToJson(company_types[company_type_id]);
    }

    if (m.street_prefix_id().has_value()) { 
      long long int street_prefix_id = m.street_prefix_id().value();
      if (street_prefixes.count(street_prefix_id))
        model_as_json["street_prefix"] = ModelToJson(street_prefixes[street_prefix_id]);
    }

    ret.push_back(model_as_json);
  }

  return Response(ret);
}

std::vector<CrmPerson> CrmPeopleController::modelSelect(Controller::PostBody &) {
	return CrmPerson::Select( fmt::format(
    "select * from {table_name} order by {order_by}", 
		fmt::arg("table_name", CrmPerson::Definition.table_name),
		fmt::arg("order_by", join({ 
      "lastname asc", "firstname asc", "distinction asc"}, ", "))
    ));
}

vector<CrmPosition> CrmPositionsController::modelSelect(Controller::PostBody &) {
	return CrmPosition::Select( fmt::format(
    "select * from {table_name} {joins} order by {order_by}", 
		fmt::arg("table_name", CrmPosition::Definition.table_name),
		fmt::arg("joins", join({
      "join companies on positions.company_id = companies.id", 
      "join people on positions.person_id = people.id"}, " ")),
		fmt::arg("order_by", join({ "companies.common_name asc", 
      "people.lastname asc", "people.firstname asc"}, ", "))
    ));
}

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

