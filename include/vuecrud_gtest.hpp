#include "prails_gtest.hpp"
#include "user.hpp"

class VuecrudControllerTest : public PrailsControllerTest {
  virtual void SetUp() {
    PrailsControllerTest::SetUp();

    User account = jsmith();
    if (!account.isValid()) 
      throw std::runtime_error("Error creating user account");
    account.save();
  }

  virtual void TearDown() {
    PrailsControllerTest::TearDown();

    auto users = User::Select(fmt::format("select * from {}", 
      User::Definition.table_name)); 

    for (auto &user : users) user.remove();
  }

  protected:
    const std::string jsmith_password = "SuperSecret";

    Model::Record jsmith_record = {
      {"name",  "John Smith"},
      {"email", "jsmith@google.com"},
      {"user_type_id", 3},
      {"active",       1},
    };

    User jsmith() {
      std::tm now = Model::NowUTC();

      User john(jsmith_record);
      john.password(jsmith_password);
      john.updated_at(now);
      john.created_at(now);

      return john;
    }

    std::string login_user() {
      auto res = browser().Post("/api/auth/login", 
        fmt::format("email=jsmith%40google.com&password={}", jsmith_password), 
        "application/x-www-form-urlencoded");

      if (res->status != 200)
        throw std::runtime_error("Unable to login as jsmith...");

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
