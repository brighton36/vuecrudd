#include "crm_companies_controller.hpp"

using namespace std;

PSYM_CONTROLLER(CrmCompanyController)

vector<CrmCompany> CrmCompanyController::modelSelect(Controller::PostBody &) {
  logger->info("TODO: Testing the modelSelect");

  vector<CrmCompany> companies = CrmCompany::Select( fmt::format(
    "select * from `{table_name}` order by `common_name` asc", 
    fmt::arg("table_name", CrmCompany::Definition.table_name)));

/* 
select * from `companies` order by `common_name` asc []
select * from `company_types` where `company_types`.`id` in (?, ?, ?, ?) [1, 2, 3, 4]
select * from `street_prefixes` where `street_prefixes`.`id` in (?) [1]
*/
  return companies;
}

Controller::Response 
CrmCompanyController::index(const Pistache::Rest::Request& request) {

  logger->info("TODO: Testing the index");
  auto post = Controller::PostBody(request.body());
  auto ret = nlohmann::json::array();

  for (auto &m: modelSelect(post)) ret.push_back(Controller::ModelToJson(m));

  return Controller::Response(ret);
}
