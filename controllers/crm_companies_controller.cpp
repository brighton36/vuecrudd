#include "crm_companies_controller.hpp"
#include "crm_company_type.hpp"
#include "crm_street_prefix.hpp"
#include "utilities.hpp"

using namespace std;
using namespace Pistache::Rest;
using namespace prails::utilities;

PSYM_CONTROLLER(CrmCompanyController)

// TODO: Do we even need this?
vector<CrmCompany> CrmCompanyController::modelSelect(Controller::PostBody &) {
  logger->info("TODO: Testing the modelSelect");

  vector<CrmCompany> companies = CrmCompany::Select( 
    "select * from `companies` order by `common_name` asc");
  return companies;
}

Controller::Response CrmCompanyController::index(const Request& request) {
  auto post = Controller::PostBody(request.body());
  auto ret = nlohmann::json::array();

  auto companies = modelSelect(post);

  vector<long long int> company_type_ids;
  vector<long long int> street_prefix_ids;
  map<long long int, nlohmann::json> company_types;
  map<long long int, nlohmann::json> street_prefixes;

	auto includes = [](long long int needle, vector<long long int> haystack) {
    return find(haystack.begin(), haystack.end(), needle) != haystack.end();
	};

  // Scan the results, to see what other table records we'll be referencing:
  for (auto &m: companies) {
		if (m.company_type_id().has_value()) {
      long long int company_type_id = *m.company_type_id();

      if (!includes(company_type_id, company_type_ids))
        company_type_ids.push_back(company_type_id);
		}

		if (m.street_prefix_id().has_value()) {
      long long int street_prefix_id = *m.street_prefix_id();

      if (!includes(street_prefix_id, street_prefix_ids))
        street_prefix_ids.push_back(street_prefix_id);
		}
  }
  
  string select_company_types = fmt::format(
		"select * from `{table_name}` where `{table_name}`.`id` in (:1)", 
		fmt::arg("table_name", "company_types")
	);

  string select_street_prefixes = fmt::format(
		"select * from `{table_name}` where `{table_name}`.`id` in (:1)", 
		fmt::arg("table_name", "street_prefixes")
	);

  vector<Model::RecordValueOpt> select_args;
  transform(company_type_ids.cbegin(), company_type_ids.cend(), 
    back_inserter(select_args), [](const auto& id) { return Model::RecordValueOpt(id); });

  for(auto &ct: CrmCompanyType::Select(select_company_types, company_type_ids))
    company_types[*ct.id()] = Controller::ModelToJson(ct);

  for(auto &sp: CrmStreetPrefix::Select(select_street_prefixes, street_prefix_ids))
    street_prefixes[*sp.id()] = Controller::ModelToJson(sp);

  for (auto &m: companies) {
    // TODO: Join the company_types and street_prefixes
    auto model_as_json = Controller::ModelToJson(m);

    model_as_json["address"] = m.address();

    model_as_json["company_type"] = nullptr;
    model_as_json["street_prefix"] = nullptr;

    if (m.company_type_id().has_value()) { 
      long long int company_type_id = m.company_type_id().value();
      if (company_types.count(company_type_id))
        model_as_json["company_type"] = company_types[company_type_id];
    }

    if (m.street_prefix_id().has_value()) { 
      long long int street_prefix_id = m.street_prefix_id().value();
      if (street_prefixes.count(street_prefix_id))
        model_as_json["street_prefix"] = street_prefixes[street_prefix_id];
    }

    ret.push_back(model_as_json);
  }

  return Controller::Response(ret);
}
