#include "prails_gtest.hpp"
#include "user.hpp"

class VuecrudControllerTest : public PrailsControllerTest {
  std::map<std::string, std::string> passwords = {{"crm", "SuperSecret"}};
  std::map<std::string, Model::Record> accounts = {
    { "crm", { 
      {"name", "John Smith"}, {"email", "jsmith@google.com"},
      {"user_type_id", 1}, {"active", 1} } },
    { "admin", { 
      {"name", "Site Admin"}, {"email", "root@company.com"},
      {"user_type_id", 3}, {"active", 1} } }
  };
  std::map<std::string, Model::Record> permissions = {
    { "crm", 
      { {"name", "CRM"}, {"code", "CRM"}, {"path", "crm"}, {"active", 1} } },
    { "admin", 
      { {"name", "administracja"}, {"code", "ADMIN"}, {"path", "admin"}, 
        {"active", 1} } } 
  };

  virtual void SetUp() {
    PrailsControllerTest::SetUp();

    std::map<std::string,long long int> permission_key_to_id;
    std::map<std::string,long long int> user_key_to_id;

    std::tm now = Model::NowUTC();

    std::for_each(permissions.begin(), permissions.end(), [&](auto &p) { 
      Permission permission(p.second);
      permission.updated_at(now);
      permission.created_at(now);
      if (!permission.isValid()) throw std::runtime_error(
        fmt::format("Error creating permission: \"{}\"", p.first));
      permission.save();
      permission_key_to_id[p.first] = permission.id().value();
    });

    std::for_each(accounts.begin(), accounts.end(), [&](const auto &a) { 
      User user(a.second);
      user.password(passwords[a.first]);
      user.updated_at(now);
      user.created_at(now);
      if (!user.isValid()) throw std::runtime_error(
        fmt::format("Error creating user account: \"{}\"", a.first));
      user.save();
      user_key_to_id[a.first] = user.id().value();
    });

    // habtm association:
    std::vector<Model::Record> user_permissions = {
      { {"user_id", user_key_to_id["crm"]}, {"permission_id", permission_key_to_id["crm"]} },
      { {"user_id", user_key_to_id["admin"]}, {"permission_id", permission_key_to_id["crm"]} },
      { {"user_id", user_key_to_id["admin"]}, {"permission_id", permission_key_to_id["admin"]} }
    };

    std::for_each(user_permissions.begin(), user_permissions.end(), [&](const auto &up) { 
      UserPermission user_permission(up);
      user_permission.updated_at(now);
      user_permission.created_at(now);
      user_permission.active(1);

      if (!user_permission.isValid()) 
        throw std::runtime_error(fmt::format( 
          "Error creating user_permission: {}/{}", 
          user_permission.user_id().value(),
          user_permission.permission_id().value() )
        );

      user_permission.save();
    });
  }

  virtual void TearDown() {
    PrailsControllerTest::TearDown();

    for (auto &user : User::Select(fmt::format(
      "select * from {}", User::Definition.table_name))) user.remove();

    for (auto &perm : Permission::Select(fmt::format(
      "select * from {}", Permission::Definition.table_name))) perm.remove();

    for (auto &up : UserPermission::Select(fmt::format(
      "select * from {}", UserPermission::Definition.table_name))) up.remove();
  }

  protected:

    std::string login_user(std::string user_key) {
      std::string email = std::get<std::string>(accounts[user_key]["email"].value());

      auto res = browser().Post("/api/auth/login", fmt::format(
        "email={}&password={}", 
        httplib::detail::encode_url(email), 
        passwords[user_key]
        ), "application/x-www-form-urlencoded");

      if (res->status != 200)
        throw std::runtime_error(fmt::format("Unable to login as {}...", user_key));

      auto response = nlohmann::json::parse(res->body);

      return response["token"].get<std::string>();
    }

    std::shared_ptr<httplib::Response> 
    GetWithToken(std::string url, std::string token = "") {
      auto headers = user_headers("application/json; charset=utf8", token);
      return browser().Get(url.c_str(), headers);
    }

    std::shared_ptr<httplib::Response> 
    PostWithToken(std::string url, std::string body = "", std::string token = "") {
      auto headers = user_headers("application/x-www-form-urlencoded", token);
      return browser().Post(url.c_str(), headers, {});
    }

  private:
    httplib::Headers user_headers(std::string content_type, std::string token) {
      httplib::Headers headers({{"Content-Type", content_type}});

      if (!token.empty())
        headers.insert({"Authorization", fmt::format("Bearer {}", token)});

      return headers;
    }
};
