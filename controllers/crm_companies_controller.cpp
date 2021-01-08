#include "crm_companies_controller.hpp"
#include "crm_company_type.hpp"
#include "crm_street_prefix.hpp"
#include "utilities.hpp"

using namespace std;
using namespace Pistache::Rest;
using namespace prails::utilities;
using namespace Controller;

PSYM_CONTROLLER(CrmCompanyController)

// TODO:
// This is kind of a temporary idea atm. Probably this should get put into either
// the controller or the model somewhere... Tests would be good...
template<typename T, class U, class V>
std::map<T, U> with(std::vector<V> &records, const std::string & foreign_key_attr) {

  // Let's first collect a vector of the unique fkey values in the provided recordset
  std::vector<T> values;
  for (auto &r: records) {
    Model::RecordValueOpt val = r.recordGet(foreign_key_attr);
		if (val.has_value()) {
      T cast_val = std::get<T>(*val);
      if (find(values.begin(), values.end(), cast_val) == values.end())
        values.push_back(cast_val);
    }
  }

  // Now we'll construct a query
  Model::Record params;
  string in_attr_keys;
  for (unsigned int i = 0; i < values.size(); i++) {
    string key = "id"+to_string(i);
    params[key] = values[i];
    in_attr_keys.append(((i == 0) ? ":" : ", :")+key);
  }

  string query = fmt::format(
		"select * from `{table_name}` where `{table_name}`.`id` in ({in_attr_keys})", 
		fmt::arg("table_name", U::Definition.table_name),
		fmt::arg("in_attr_keys", in_attr_keys)
	);

  std::map<T, U> ret; 
  for(auto &sr: U::Select(query, params))
    ret[std::get<T>(*sr.recordGet("id"))] = sr;

  return ret;
}

void CrmCompanyController::Routes(
Pistache::Rest::Router& r, std::shared_ptr<Instance> controller) {
  using namespace Pistache::Rest::Routes;

  RestInstance<CrmCompanyController, CrmCompany>::Routes(r,controller);

  // For some reason, this update action doesnt use the same prefix...
  Put(r, "/api/crud/crm/companies/:id", bind("update", 
    &CrmCompanyController::create_or_update, controller));
  Post(r, "/api/crud/crm/companies", bind("create", 
    &CrmCompanyController::create_or_update, controller));
  Post(r, "/api/crud/crm/companies/multiple-update", 
    bind("multiple_update", &CrmCompanyController::multiple_update, controller));

  if ( !GetConfig().cors_allow().empty() ) {
    Options(r, "/api/crud/crm/companies/*",
      bind("api_options_actions", &CrmCompanyController::options, controller));
    Options(r, "/api/crud/crm/companies",
      bind("api_options_index", &CrmCompanyController::options, controller));
  }
}

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
