#include "prails_gtest.hpp"

#include "account.hpp"

using namespace std;

class AuthControllerTest : public PrailsControllerTest {
  public:
    // TODO: We probably don't need this
    void SetUp() override {
      PrailsControllerTest::SetUp();
    }

  protected:
    Model::Record jsmith_record = {
      {"first_name", "John"},
      {"last_name",  "Smith"},
      {"email",      "jsmith@google.com"}
    };

    Account jsmith() {
      std::tm now = Model::NowUTC();

      Account account(jsmith_record);
      account.password("SuperSecret");
      account.updated_at(now);
      account.created_at(now);

      return account;
    }
};

PSYM_TEST_ENVIRONMENT();

TEST_F(AuthControllerTest, fail_post_parse) {
  Account account = jsmith();
  EXPECT_TRUE(account.isValid());
  ASSERT_NO_THROW(account.save());

  auto res = browser().Post("/api/auth/login", 
    "unparseablemuck", 
    "application/x-www-form-urlencoded");

  ASSERT_EQ(res->status, 401);

  auto response = nlohmann::json::parse(res->body);
  ASSERT_EQ(response["error"], "Invalid credentials");

  account.remove();
}

TEST_F(AuthControllerTest, fail_login) {
  nlohmann::json response;
  std::shared_ptr<httplib::Response> res;

  Account account = jsmith();
  EXPECT_TRUE(account.isValid());
  ASSERT_NO_THROW(account.save());

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

  account.remove();
}

TEST_F(AuthControllerTest, success_login) {
  Account account = jsmith();
  EXPECT_TRUE(account.isValid());
  ASSERT_NO_THROW(account.save());

  auto res = browser().Post("/api/auth/login", 
    "email=jsmith%40google.com&password=SuperSecret", 
    "application/x-www-form-urlencoded");

  ASSERT_EQ(res->status, 200);

  auto response = nlohmann::json::parse(res->body);

  Account retrieved_account = *Account::Find(account.id().value());

  ASSERT_EQ(response["permissions"].get<vector<string>>(), 
    vector<string>({"CRM","ADMIN","MODELAND"}));
  ASSERT_EQ(response["token"], *retrieved_account.auth_token());
  ASSERT_EQ(response["user"]["active"], 1);
  ASSERT_EQ(response["user"]["email"], "jsmith@google.com");
  ASSERT_EQ(response["user"]["name"], "John"); // TODO?

  retrieved_account.remove();
}
