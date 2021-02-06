#include "vuecrud_gtest.hpp"

#include "user.hpp"

using namespace std;

class AuthControllerTest : public VuecrudControllerTest {};

PSYM_TEST_ENVIRONMENT();

TEST_F(AuthControllerTest, fail_post_parse) {
  auto res = browser().Post("/api/auth/login", "unparseablemuck", 
    "application/x-www-form-urlencoded");

  ASSERT_EQ(res->status, 401);

  auto response = nlohmann::json::parse(res->body);
  ASSERT_EQ(response["error"], "Invalid credentials");
}

TEST_F(AuthControllerTest, fail_login) {
  nlohmann::json response;
  std::shared_ptr<httplib::Response> res;

  // First, let's try a valid username, and invalid password:
  res = browser().Post("/api/auth/login", 
    "email=jsmith%40google.com&password=InvalidPassword", 
    "application/x-www-form-urlencoded");

  ASSERT_EQ(res->status, 401);

  response = nlohmann::json::parse(res->body);
  ASSERT_EQ(response["error"], "Invalid credentials");

  // Next, let's try an invalid username, and valid password:
  res = browser().Post("/api/auth/login", 
    "email=user%40google.com&password=SuperSecret", 
    "application/x-www-form-urlencoded");

  ASSERT_EQ(res->status, 401);

  response = nlohmann::json::parse(res->body);
  ASSERT_EQ(response["error"], "Invalid credentials");
}

TEST_F(AuthControllerTest, success_login) {
  auto res = browser().Post("/api/auth/login", 
    "email=jsmith%40google.com&password=SuperSecret", 
    "application/x-www-form-urlencoded");

  ASSERT_EQ(res->status, 200);

  auto response = nlohmann::json::parse(res->body);

  ASSERT_EQ(response["permissions"].get<vector<string>>(), 
    vector<string>({"CRM","ADMIN","MODELAND"}));
  ASSERT_TRUE(response["token"].get<string>().length() > 0);
  ASSERT_EQ(response["user"]["active"], 1);
  ASSERT_EQ(response["user"]["email"], "jsmith@google.com");
  ASSERT_EQ(response["user"]["name"], "John Smith");
}
