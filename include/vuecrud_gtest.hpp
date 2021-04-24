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

  virtual void SetUp() {
    PrailsControllerTest::SetUp();

    std::tm now = Model::NowUTC();

    std::for_each(accounts.begin(), accounts.end(), [&](const auto &account) { 
      std::string key = account.first;

      User user(account.second);

      user.password(passwords[key]);
      user.updated_at(now);
      user.created_at(now);

      if (!user.isValid()) 
        throw std::runtime_error(fmt::format("Error creating user account: \"{}\"", key));
      user.save();
    });
  }

  virtual void TearDown() {
    PrailsControllerTest::TearDown();

    auto users = User::Select(fmt::format(
      "select * from {}", User::Definition.table_name)); 

    for (auto &user : users) user.remove();
  }

  protected:

    std::string login_user(std::string user_key = "crm") {
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
