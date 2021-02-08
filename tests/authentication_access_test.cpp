#include "vuecrud_gtest.hpp"

#include "user.hpp"

using namespace std;

class AuthenticationAccess : public VuecrudControllerTest {
  protected:
};

PSYM_TEST_ENVIRONMENT();

TEST_F(AuthenticationAccess, test_user_index_access) {
  vector<string> indexes({"/api/crm/companies", "/api/crud/crm/company-types", 
    "/api/crm/company-files", "/api/crm/company-comments", 
    "/api/crud/crm/company-comment-types", "/api/crm/person-comments",
    "/api/crm/person-comment-types", "/api/crm/positions", 
    "/api/crm/position-tasks", "/api/crud/crm/tasks"});

  std::shared_ptr<httplib::Response> res;
  auto client = browser();

  // These should all be denied:
  for_each(indexes.begin(), indexes.end(), [&] (auto p) {
    res = GetWithToken(p);
    { SCOPED_TRACE(fmt::format("Un-Authenticated Get \"{}\"", p));
      EXPECT_EQ(res->status, 400);
      EXPECT_EQ(res->body, "{\"error\":\"token_not_provided\"}"); }
  });

  res = PostWithToken("/api/crm/people/search");
  EXPECT_EQ(res->status, 400);
  // NOTE: the body on this res is in html. No need to test it really...

  string token = login_user();

  // These should all suceed:
  for_each(indexes.begin(), indexes.end(), [&] (auto p) {
    res = GetWithToken(p, token);
    { SCOPED_TRACE(fmt::format("Authenticated Get \"{}\"", p));
      EXPECT_EQ(res->status, 200); }
  });

  res = PostWithToken("/api/crm/people/search", "", token);
  EXPECT_EQ(res->status, 200);
}

TEST_F(AuthenticationAccess, test_admin_index_access) {
  // Admin only: 
  vector<string> indexes({"/api/admin/users", "/api/crud/admin/permissions", 
    "/api/admin/user-permissions"});

  // TODO: both user and Admin user login
}

TEST_F(AuthenticationAccess, test_mutant_authorization_access) {
  // TODO: Test malformed authorization content (no bearer prefix), unavailable tokens, and expired tokens
}
