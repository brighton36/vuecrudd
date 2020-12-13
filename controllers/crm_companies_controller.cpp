#include "crm_companies_controller.hpp"

using namespace std;

PSYM_CONTROLLER(CrmCompanyController)

vector<CrmCompany> CrmCompanyController::modelSelect(Controller::PostBody &) {
  logger->info("TODO: Testing the modelSelect");

  vector<CrmCompany> companies = CrmCompany::Select( 
    "select * from `companies` order by `common_name` asc");
  return companies;
}

Controller::Response 
CrmCompanyController::index(const Pistache::Rest::Request& request) {

  logger->info("TODO: Testing the index");
  auto post = Controller::PostBody(request.body());
  auto ret = nlohmann::json::array();


/* 
select * from `companies` order by `common_name` asc []
select * from `company_types` where `company_types`.`id` in (?, ?, ?, ?) [1, 2, 3, 4]
select * from `street_prefixes` where `street_prefixes`.`id` in (?) [1]
*/

  for (auto &m: modelSelect(post)) {
    // TODO: Join the company_types and street_prefixes
    ret.push_back(Controller::ModelToJson(m));
  }

  return Controller::Response(ret);
}
