#include "vuecrud_gtest.hpp"

#include "user.hpp"
#include <iostream> // TODO

using namespace std;

class AuthenticationAccess : public VuecrudControllerTest {
  protected:
};

PSYM_TEST_ENVIRONMENT();

TEST_F(AuthenticationAccess, test_user_index_access) {
  vector<string> crm_indexes({"/api/crm/companies", "/api/crud/crm/company-types", 
    "/api/crm/company-files", "/api/crm/company-comments", 
    "/api/crud/crm/company-comment-types", "/api/crm/person-comments",
    "/api/crm/person-comment-types", "/api/crm/positions", 
    "/api/crm/position-tasks", "/api/crud/crm/tasks"});

  vector<string> admin_indexes({"/api/admin/users", "/api/crud/admin/permissions", 
    "/api/admin/user-permissions"});

  shared_ptr<httplib::Response> res;
  auto client = browser();

  // These should all be denied:
  vector<string> indexes = crm_indexes;
  indexes.insert(indexes.end(), admin_indexes.begin(), admin_indexes.end());

  for_each(indexes.begin(), indexes.end(), [&] (auto p) {
    res = GetWithToken(p);
    { SCOPED_TRACE(fmt::format("Un-Authenticated Get \"{}\"", p));
      EXPECT_EQ(res->status, 400);
      EXPECT_EQ(res->body, "{\"error\":\"token_not_provided\"}"); }
  });

  res = PostWithToken("/api/crm/people/search");
  EXPECT_EQ(res->status, 400);
  // NOTE: the body on this res is in html. No need to test it really...

  string crm_token = login_user("crm");
  string admin_token = login_user("admin");

  for_each(crm_indexes.begin(), crm_indexes.end(), [&] (auto p) {
    res = GetWithToken(p, crm_token);
    { SCOPED_TRACE(fmt::format("Authenticated Get crm \"{}\"", p));
      EXPECT_EQ(res->status, 200); }

    res = GetWithToken(p, admin_token);
    { SCOPED_TRACE(fmt::format("Authenticated Get admin \"{}\"", p));
      EXPECT_EQ(res->status, 200); }

    // This should fail:
    res = GetWithToken(p);
    { SCOPED_TRACE(fmt::format("Un-Authenticated Get \"{}\"", p));
      EXPECT_EQ(res->status, 400);
      EXPECT_EQ(res->body, "{\"error\":\"token_not_provided\"}"); }
  });

  for_each(admin_indexes.begin(), admin_indexes.end(), [&] (auto p) {
    // This should succeed:
    res = GetWithToken(p, admin_token);
    { SCOPED_TRACE(fmt::format("Authenticated Get admin \"{}\"", p));
      EXPECT_EQ(res->status, 200); }

    res = GetWithToken(p, crm_token);
    { SCOPED_TRACE(fmt::format("Authenticated Get crm \"{}\"", p));
      EXPECT_EQ(res->status, 400); }

    // This should fail:
    res = GetWithToken(p);
    { SCOPED_TRACE(fmt::format("Un-Authenticated Get \"{}\"", p));
      EXPECT_EQ(res->status, 400);
      EXPECT_EQ(res->body, "{\"error\":\"token_not_provided\"}"); }
  });

  res = PostWithToken("/api/crm/people/search", "", crm_token);
  EXPECT_EQ(res->status, 200);
}

TEST_F(AuthenticationAccess, test_mutant_authorization_access) {
  // TODO: Test malformed authorization content (no bearer prefix), unavailable tokens, and expired tokens
}
