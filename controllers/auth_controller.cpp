#include "auth_controller.hpp"
#include "account.hpp"

using namespace std;
using namespace Pistache;
using namespace prails::utilities;

PSYM_CONTROLLER(AuthController)

void AuthController::Routes(Rest::Router& r, shared_ptr<Controller::Instance> controller) {
  using namespace Rest;

  Rest::Routes::Get(r, "/api/auth/user", 
    bind("user_show", &AuthController::user_show, controller));
  Rest::Routes::Post(r, "/api/auth/user", 
    bind("user_update", &AuthController::user_update, controller));
  Rest::Routes::Post(r, "/api/auth/user-password", 
    bind("user_password", &AuthController::user_password, controller));
  Rest::Routes::Post(r, "/api/auth/login",
    bind("login", &AuthController::login, controller));
  Rest::Routes::Post(r, "/api/auth/logout",
    bind("logout", &AuthController::logout, controller));

  // TODO: Maybe we can just create a more clever bind above...
  //       Like: bind_response("options", CorsOkResponse(), controller)
  if ( !Controller::GetConfig().cors_allow().empty() )
    Rest::Routes::Options(r, "/api/auth/*",
      bind("options", &AuthController::options, controller));
}

Controller::Response AuthController::options(const Rest::Request&) {
  return Controller::CorsOkResponse();
}

Controller::Response AuthController::login(const Rest::Request& request) {
  ensure_content_type(request, MIME(Application, FormUrlEncoded));

  auto post = Controller::PostBody(request.body());
  std::optional<Account> account = nullopt;

  if (post["email"] && post["password"])
    account = Account::FromLogin(*post["email"], *post["password"]);

  if (!account.has_value()) {
    spdlog::error("Invalid login attempted for user \"{}\" from {}.", 
      (post["email"]) ? *post["email"] : "" , request.address().host());
    return Controller::Response( nlohmann::json({{"error", "Invalid credentials"}}), 401 );
  }

  (*account).generate_new_auth_token();

  if (!(*account).isValid())
    throw RequestException("Authentication succeeded for user {}, but unable to"
      "persist token to database", *(*account).email());
  (*account).save();

  spdlog::info("User \"{}\" logged in successfully from {}.", 
    *(*account).email(), request.address().host());

  return Controller::Response(nlohmann::json::object({
    {"token", *(*account).auth_token()},
    {"user", {
      {"name", *(*account).first_name() }, 
      {"email", *(*account).email()}, 
      {"active", 1}
    } }, 
    {"permissions", nlohmann::json::array({"CRM", "ADMIN", "MODELAND"})}
  }));
}

Controller::Response AuthController::user_show(const Rest::Request& request) {
  Account account = ensure_authorization<Account>(request);
 
  auto user = nlohmann::json::object({
    {"id", *account.id()},
    {"name", *account.first_name()},
    {"email", *account.email()},
    {"created_at", tm_to_iso8601(*account.created_at())},
    {"updated_at", tm_to_iso8601(*account.updated_at())},
    // TODO: Do these fields really make sense...:
    {"active", 1},
    {"initial_password", "OTj2IDbS"},
    {"user_type_id", 3},
    {"user_permissions",  nlohmann::json::array({
      { nlohmann::json::object({ 
        {"id", 15},
        {"user_id", *account.id()},
        {"permission_id", 1},
        {"active", 1},
        {"created_at", "2020-03-20 21:59:07"},
        {"updated_at", "2020-03-20 21:59:07"},
        {"permission",  nlohmann::json::object({
          {"id", 1},
          {"name", "CRM"},
          {"code", "CRM"},
          {"active", 1},
          {"created_at", nullptr},
          {"updated_at", "2020-03-20 20:56:48"}
          }) }
        })
      },
      { nlohmann::json::object({ 
        {"id", 16},
        {"user_id", *account.id()},
        {"permission_id", 2},
        {"active", 1},
        {"created_at", "2020-03-20 21:59:07"},
        {"updated_at", "2020-03-20 21:59:07"},
        {"permission",  nlohmann::json::object({
          {"id", 2},
          {"name", "ADMIN"},
          {"code", "ADMIN"},
          {"active", 1},
          {"created_at", nullptr},
          {"updated_at", "2020-03-20 20:56:48"}
          }) }
        })
      },
      })
    }
  });

  return Controller::Response(user);
}

Controller::Response AuthController::user_update(const Rest::Request& request) {
  Account account = ensure_authorization<Account>(request);
  ensure_content_type(request, MIME(Application, FormUrlEncoded));

  auto post = Controller::PostBody(request.body());

  if (post["name"]) account.first_name(*post["name"]);
  if (post["email"]) account.email(*post["email"]);
  account.updated_at(Model::NowUTC());

  return render_model_save_js<Account>(account);
}

Controller::Response AuthController::user_password(const Rest::Request& request) {
  Account account = ensure_authorization<Account>(request);
  ensure_content_type(request, MIME(Application, FormUrlEncoded));

  auto post = Controller::PostBody(request.body());

  if ((!post["old"]) || (!post["new"]) || (!post["repeat"]))
    return Controller::Response(
      nlohmann::json({{"status", -1}, {"msg", {{"General", "missing required fields"}} }})
    );

  if (post["new"] != post["repeat"])
    return Controller::Response(
      nlohmann::json({{"status", -1}, {"msg", {{"repeat", "doesnt match new password"}} }})
    );

  if (*account.password() != Account::Hash(*post["old"]))
    return Controller::Response(
      nlohmann::json({{"status", -1}, {"msg", {{"old", "password invalid"}} }})
    );

  account.password(*post["new"]);
  account.updated_at(Model::NowUTC());

  return render_model_save_js<Account>(account, 1, -1);
}

Controller::Response AuthController::logout(const Rest::Request& request) {
  Account account = ensure_authorization<Account>(request);
  
  spdlog::info("User \"{}\" logged out {}.", *account.email(), request.address().host());

  account.clear_auth_token();

  if (!account.isValid())
    throw RequestException("Logout request succeeded for user {}, but unable to"
      "persist cleared token to database", *account.email());

  account.save();
  
  return Controller::Response( nlohmann::json({{"status", 0}}) );
}

