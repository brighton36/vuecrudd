#include "utilities.hpp"

#include "crm_language.hpp"

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

#include <iostream> // todo

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
  auto companies = modelSelect(post);

  return Response(ModelToJson(companies, vector<JsonDecorator<CrmCompany>>({
    with<long long int, CrmCompanyType>(companies, "company_type", "company_type_id"),
    with<long long int, CrmStreetPrefix>(companies, "street_prefix", "street_prefix_id"),
    [](CrmCompany &m, nlohmann::json &json) { json["address"] = m.address(); }
    })));
}

Response CrmCompanyCommentsController::index(const Request& request) {
  auto post = PostBody(request.body());

  auto comments = modelSelect(post);
  // TODO: Get the user id working:
  // select * from `users` where `users`.`id` in (?) [1]
  return Response(ModelToJson(comments, vector<JsonDecorator<CrmCompanyComment>>({
    with<long long int, CrmCompany>(comments, "company", "company_id"),
    with<long long int, CrmCompanyCommentType>(
      comments, "company_comment_type", "company_comment_type_id")})));
}

vector<CrmPerson> CrmPeopleController::modelSelect(Controller::PostBody &) {
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

void CrmPeopleController::Routes(
Pistache::Rest::Router& r, shared_ptr<Controller::Instance> controller) {
  using namespace Pistache::Rest::Routes;
  using namespace Controller;

  VuecrudController<CrmPeopleController, CrmPerson>::Routes(r, controller);

  // This search is used by the GUI:
  Post(r, "/api/crm/people/search",
    bind("search", &CrmPeopleController::search, controller));
  Options(r, "/api/crm/people/search", 
    bind("options_search", &CrmPeopleController::options, controller));

  // TODO: 
  // This address is used by the 'export to excel' function
  Post(r, "/api/crud/crm/people/search",
    bind("put_search", &CrmPeopleController::search, controller));
  Options(r, "/api/crud/crm/people/search", 
    bind("options_put_search", &CrmPeopleController::options, controller));
}

Controller::Response CrmPeopleController::search(const Pistache::Rest::Request& request) {

  map<string,string> filter_columns = {
    {"id",        "`people`.id"}, 
    {"firstname", "`people`.firstname"},
    {"lastname",  "`people`.lastname"},
    {"email",     "`people`.email"},
    {"phone",     "`people`.phone"},
    {"sex",       "`sexes`.name"},
    {"language",  "`languages`.name"}};

  vector<string> joins({
    "left join `sexes` on `sexes`.`id` = `people`.`sex_id`",
    "left join `languages` on `languages`.`id` = `people`.`language_id`"});

  ensure_content_type(request, MIME(Application, FormUrlEncoded));

  auto post = Controller::PostBody(request.body());

  // NOTE: We assume proto here. I'm not sure how/if we'll want to support SSL
  auto host = request.headers().get<Pistache::Http::Header::Host>();
  string base_path = fmt::format("http://{}:{}{}", 
    host->host(), host->port(), request.resource());

  unsigned long page = 1;
  if (auto param_page = request.query().get("page"); !param_page.isEmpty()) {
    if (!regex_match(param_page.get(), regex("^[\\d]+$")))
      throw invalid_argument("invalid page parameter passed to search");

    page = stoul(param_page.get());
  } else if (post.has_scalar("page"))
    page = post.operator[]<unsigned long>("page").value();

  unsigned long per_page = (post.has_scalar("rowsPerPage")) ? 
    post.operator[]<unsigned long>("rowsPerPage").value() : 20;

  unsigned long offset = (page-1) * per_page;

  Model::Record select_params;
  vector<string> where;

  if (post.has_collection("selectedStatuses")) {
    vector<string> status_keys;

    for (int i = 0; i < post.size("selectedStatuses"); i++) {
      string key = fmt::format("selected_status{}", i);
      status_keys.push_back(":"+key);
      select_params[key] = stoul(*post("selectedStatuses", i));
    }

    if (status_keys.size() > 0)
      where.push_back( (status_keys.size() == 1) ?
        fmt::format("`people`.active = {}", status_keys[0]) : 
        fmt::format("`people`.active in ({})", join(status_keys, ", ")));
  }

  // Column-specific Search
  for (const string &n : post.keys("filterColumns")) {
    auto mode = post("filterColumns", n, "mode");
    auto name = post("filterColumns", n, "name");
    auto value = post("filterColumns", n, "value");
    
    if ( mode.has_value() && name.has_value() && value.has_value() && 
      (!(*mode).empty()) && (!(*name).empty()) && (!(*value).empty()) &&
      (filter_columns.count(*name) > 0) ) {

      if (!regex_match(*name, regex("^[a-z]+$")))
        throw invalid_argument(
            "invalid filterColumns[][name] parameter passed to search");
      if (!regex_match(*mode, regex("^(?:like|equals|list)$")))
        throw invalid_argument(
            "invalid filterColumns[][mode] parameter passed to search");

      if (*mode == "like") {
        string key = fmt::format("filter_{}",*name);
        where.push_back(fmt::format("{} like {}", filter_columns[*name], ":"+key));
        select_params[key] = "%"+*value+"%";
      } else if (*mode == "equals") {
        string key = fmt::format("filter_{}",*name);
        where.push_back(fmt::format("{} = {}", filter_columns[*name], ":"+key));
        select_params[key] = *value;
      } else if (*mode == "list") {
        // NOTE: Possibly someone will need (backslash?) escaped ,'s. But, I 
        // don't feel like writng that now. Maybe a chomping of spaces around
        // the comma's...
        vector<string> values = split(*value, ",");
        vector<string> keys;
        for (unsigned int i = 0; i<values.size(); i++) {
          string key = fmt::format("filter_{}{}",*name,i);
          select_params[key] = values[i]; 
          keys.push_back(":"+key);
        }
        where.push_back(
          fmt::format("{} in ({})", filter_columns[*name], join(keys, ", ")));
      }
    }
  }

  // All-columns Search:
  if (post.has_scalar("search") && (!(*post["search"]).empty())) {
    vector<string> search_where_pairs;
    transform(filter_columns.begin(), filter_columns.end(), 
      back_inserter(search_where_pairs), 
      [](auto col) { return fmt::format("{} like :search", col.second); } );

    select_params["search"] = "%"+*post["search"]+"%"; 
    where.push_back(fmt::format("({})", join(search_where_pairs, " or ")));
  }

  // Sorting:
  vector<string> order_by;
  if ( post.has_collection("sortBy") && post.has_collection("sortDesc") && 
    (post.size("sortBy") == post.size("sortDesc") ) ) {

    for (int i = 0; i < post.size("sortBy"); i++) {
      auto col = post("sortBy", i);
      auto dir = post("sortDesc", i);
      if ( col.has_value() && !(*col).empty() && filter_columns.count(*col) &&
        dir.has_value() && ( ((*dir) == "true") || ((*dir) == "false") ) )
        order_by.push_back(fmt::format("{} {}", filter_columns[*col], 
          ((*dir) == "true") ? "desc" : "asc"));
    }
  }

/*
TODO: wtf do these do:
deleteMode: soft
activeColumnName: active
mode: paginate
*/

  string sqlWhere = (where.size() > 0) ? 
    fmt::format("where {}", join(where, " and ")) : "";

  long count_people = CrmPerson::Count( fmt::format(
    "select count(*) from {table_name} {joins} {where}", 
    fmt::arg("table_name", CrmPerson::Definition.table_name),
    fmt::arg("joins", join(joins, " ")),
    fmt::arg("where", sqlWhere)),
    select_params );

  unsigned long last_page = (unsigned long) ceil(
    (float) count_people / (float) per_page);

  vector<CrmPerson> people = CrmPerson::Select(fmt::format(
    "select {columns} from {table_name} {joins} {where}{order_by}"
    " limit {limit} offset {offset}", 
    // TODO: This should be a parameter..
    fmt::arg("columns", join({"people.*", "sexes.name", "languages.name"}, ", ")),
    fmt::arg("table_name", CrmPerson::Definition.table_name),
    fmt::arg("joins", join(joins, " ")),
    fmt::arg("where", sqlWhere),
    fmt::arg("order_by", ((order_by.size() > 0)) ? 
      fmt::format(" order by {}", join(order_by, ", ")) : ""),
    fmt::arg("limit", per_page),
    fmt::arg("offset", offset)),
    select_params );

  nlohmann::json ret({
    {"current_page", page}, 
    {"last_page", last_page},
    {"from", offset+1},
    {"to", page*per_page},
    {"total", count_people},
    {"per_page", per_page},
    {"path", base_path},
    {"next_page_url", nullptr},
    {"prev_page_url", nullptr},
    // TODO: Decorators should be a parameter...
    {"data", ModelToJson(people, vector<JsonDecorator<CrmPerson>>({
      with<long long int, CrmLanguage>(people, "language", "language_id"),
      with<long long int, CrmSex>(people, "sex", "sex_id")
      }))},
    });

  if (page < last_page)
    ret["next_page_url"] = fmt::format("{}?page={}", base_path, page + 1);
  if (page > 1)
    ret["prev_page_url"] = fmt::format("{}?page={}", base_path, page - 1);

  return Controller::Response(ret);
}
