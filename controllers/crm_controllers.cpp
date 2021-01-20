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

  return Response(ModelToJson(companies, std::vector<JsonDecorator<CrmCompany>>({
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
  return Response(ModelToJson(comments, std::vector<JsonDecorator<CrmCompanyComment>>({
    with<long long int, CrmCompany>(comments, "company", "company_id"),
    with<long long int, CrmCompanyCommentType>(
      comments, "company_comment_type", "company_comment_type_id")})));
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

void CrmPeopleController::Routes(
Pistache::Rest::Router& r, std::shared_ptr<Controller::Instance> controller) {
  using namespace Pistache::Rest::Routes;
  using namespace Controller;

  Post(r, "/api/crm/people/search",
    bind("search", &CrmPeopleController::search, controller));

  Options(r, "/api/crm/people/search", 
    bind("options_search", &CrmPeopleController::options, controller));
}

Controller::Response CrmPeopleController::search(const Pistache::Rest::Request& request) {
  ensure_content_type(request, MIME(Application, FormUrlEncoded));

  auto post = Controller::PostBody(request.body());

  // TODO:
/*
rowsPerPage: 20
page: 1
search: 
filterColumns[0][mode]: like
filterColumns[0][text]: Id
filterColumns[0][name]: id
filterColumns[0][column]: undefined
filterColumns[0][value]: 
filterColumns[1][mode]: like
filterColumns[1][text]: Lastname
filterColumns[1][name]: lastname
filterColumns[1][column]: lastname
filterColumns[1][value]: 
filterColumns[2][mode]: like
filterColumns[2][text]: Firstname
filterColumns[2][name]: firstname
filterColumns[2][column]: firstname
filterColumns[2][value]: 
filterColumns[3][mode]: like
filterColumns[3][text]: Distinction
filterColumns[3][name]: distinction
filterColumns[3][column]: distinction
filterColumns[3][value]: 
filterColumns[4][mode]: like
filterColumns[4][text]: Language
filterColumns[4][name]: language
filterColumns[4][column]: language_id
filterColumns[4][value]: 
filterColumns[5][mode]: like
filterColumns[5][text]: Sex
filterColumns[5][name]: sex
filterColumns[5][column]: sex_id
filterColumns[5][value]: 
filterColumns[6][mode]: like
filterColumns[6][text]: E-mail
filterColumns[6][name]: email
filterColumns[6][column]: email
filterColumns[6][value]: 
filterColumns[7][mode]: like
filterColumns[7][text]: Phone
filterColumns[7][name]: phone
filterColumns[7][column]: phone
filterColumns[7][value]: 
selectedStatuses[]: 1
deleteMode: soft
activeColumnName: active
mode: paginate
*/

  // NOTE: We assume proto here. I'm not sure how/if we'll want to support SSL
  auto host = request.headers().get<Pistache::Http::Header::Host>();
  string base_path = fmt::format("http://{}:{}{}", 
    host->host(), host->port(), request.resource());

  unsigned long page = 1;
  if (auto param_page = request.query().get("page"); !param_page.isEmpty()) {
    if (!regex_match(param_page.get(), regex("^[\\d]+$")))
      throw std::invalid_argument("invalid page parameter passed to CrmPeopleController::search");

    page = stoul(param_page.get());
  } else if (post.has_scalar("page"))
    page = post.operator[]<unsigned long>("page").value();

  unsigned long per_page = (post.has_scalar("rowsPerPage")) ? 
    post.operator[]<unsigned long>("rowsPerPage").value() : 20;

  unsigned long offset = (page-1) * per_page;

  vector<string> joins({
    "left join `sexes` on `sexes`.`id` = `people`.`sex_id`",
    "left join `languages` on `languages`.`id` = `people`.`language_id`"});

  long count_people = CrmPerson::Count( fmt::format(
    "select count(*) from {table_name} {joins} where {where}", 
    fmt::arg("table_name", CrmPerson::Definition.table_name),
    fmt::arg("joins", join(joins, " ")),
    fmt::arg("where", "`people`.active = 1") ));

  unsigned long last_page = (unsigned long) ceil(
    (float) count_people / (float) per_page);

  // TODO: Why is the id's all weird in here...
  vector<CrmPerson> people = CrmPerson::Select(fmt::format(
    "select * from {table_name} {joins} where {where} order by {order_by}"
    " limit {limit} offset {offset}", 
    fmt::arg("table_name", CrmPerson::Definition.table_name),
    fmt::arg("joins", join(joins, " ")),
    fmt::arg("where", "`people`.active = 1"),
    fmt::arg("order_by", "`people`.`lastname` asc"),
    fmt::arg("limit", per_page),
    fmt::arg("offset", offset)
    ));

  // TODO: why is it that the id is duplicated constantly here...

  // TODO: nix this debug stuff:
  logger->info("base: {} page: {}", base_path, page);
  logger->info("body: {}", request.body());
  logger->info("count: {}", count_people);
  logger->info("per_page: {}", per_page);
  logger->info("last_page: {}", last_page);

  nlohmann::json ret({
    {"current_page", page}, {"last_page", last_page},
    {"from", offset+1}, {"to", page*per_page},
    {"total", count_people}, {"per_page", per_page},
    {"path", base_path}, {"next_page_url", nullptr},
    {"prev_page_url", nullptr}, {"data", nlohmann::json::array()},
    });

  if (page < last_page)
    ret["next_page_url"] = fmt::format("{}?page={}", base_path, page + 1);
  if (page > 1)
    ret["prev_page_url"] = fmt::format("{}?page={}", base_path, page - 1);

  ret["data"] = ModelToJson(people, std::vector<JsonDecorator<CrmPerson>>({
    with<long long int, CrmLanguage>(people, "language", "language_id"),
    with<long long int, CrmSex>(people, "sex", "sex_id")
    }));

  return Controller::Response(ret);
}
