#include "gtest/gtest.h"

#include <pistache/http.h>
#include <pistache/stream.h>
#include <pistache/router.h>

#include "httplib.h"

#include "server.hpp"
#include "account.hpp"

using namespace std;

Pistache::Address addr;

class CustomerControllerTest : public ::testing::Test {
 protected:
  Model::Record jsmith_record = {
    {"first_name", "John"},
    {"last_name",  "Smith"},
    {"email",      "jsmith@google.com"},
  };

};

// TODO: We Should dry this up against the tasks
httplib::Client get_client() {
  return httplib::Client(addr.host().c_str(), (uint16_t) addr.port());
}

// TODO: We Should dry this up against the tasks
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  ConfigParser config(string(PROJECT_SOURCE_DIR)+"/config/test-server.yml");

  //spdlog::set_level(spdlog::level::debug);

  Model::Initialize(config);

  Account::Migrate();

  Server server(config);
  server.startThreaded();

  addr = Pistache::Address(config.address(), config.port());
  
  auto ret = RUN_ALL_TESTS();
  server.shutdown();

  return ret;
}

TEST(customer_controller_test, index) {
  auto client = get_client();
  auto res = client.Get("/customers");
  EXPECT_EQ(res->status, 200);
}

TEST_F(CustomerControllerTest, fail_post_parse) {
  // TODO: Send unparseable json just to see what happens, and that we get an error
}

TEST_F(CustomerControllerTest, fail_login) {
  auto client = get_client();
  // TODO: 405... or 403?
  // Code 405 - Method Not Allowed.
  // {"email":"test@test.com","password":"password"}
 /* 
  Account jsmith_account(jsmith_record);
  jsmith_account.password("SuperSecret");
  ASSERT_NO_THROW(jsmith_account.save());

  nlohmann::json body;
  body["email"] = "jsmith@google.com";
  body["password"] = "Invalid Password";

  auto res = client.Post("/customers/login", body.dump(), "text/plain");
  auto ret = nlohmann::json::parse(res->body);

  EXPECT_EQ(res->status, 405);
  EXPECT_EQ(ret["error"], "todo - no");
  */
}

TEST_F(CustomerControllerTest, success_login) {
  auto client = get_client();
  // Code 405 - Method Not Allowed.
  // {"email":"test@test.com","password":"password"}
  // TODO: Create the account I guess, and see how the server reacts to it
  /*
  Account jsmith_account(jsmith_record);
  jsmith_account.password("SuperSecret");
  ASSERT_NO_THROW(jsmith_account.save());

  nlohmann::json body;
  body["email"] = "jsmith@google.com";
  body["password"] = "SecretPassword";

  auto res = client.Post("/customers/login", body.dump(), "text/plain");

  std::cout << "The return: " << res->body << std::endl;
  ASSERT_EQ(res->status, 200);

  auto ret = nlohmann::json::parse(res->body);

  ASSERT_EQ(ret["auth_token"], "todo - yes");
  */
}


