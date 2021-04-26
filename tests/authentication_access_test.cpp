#include "vuecrud_gtest.hpp"

#include "user.hpp"

using namespace std;

class AuthenticationAccess : public VuecrudControllerTest {};

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

  string crm_token = login_user("crm");
  string admin_token = login_user("admin");

  for_each(crm_indexes.begin(), crm_indexes.end(), [&] (auto p) {
    // This should  be denied:
    res = GetWithToken(p);
    { SCOPED_TRACE(fmt::format("Un-Authenticated Get \"{}\"", p));
      EXPECT_EQ(res->status, 400);
      EXPECT_EQ(res->body, "{\"error\":\"token_not_provided\"}"); }

    // This should succeed:
    res = GetWithToken(p, crm_token);
    { SCOPED_TRACE(fmt::format("Authenticated Get crm \"{}\"", p));
      EXPECT_EQ(res->status, 200); }

    res = GetWithToken(p, admin_token);
    { SCOPED_TRACE(fmt::format("Authenticated Get admin \"{}\"", p));
      EXPECT_EQ(res->status, 200); }

  });

  for_each(admin_indexes.begin(), admin_indexes.end(), [&] (auto p) {
    // This should  be denied:
    res = GetWithToken(p);
    { SCOPED_TRACE(fmt::format("Un-Authenticated Get \"{}\"", p));
      EXPECT_EQ(res->status, 400);
      EXPECT_EQ(res->body, "{\"error\":\"token_not_provided\"}"); }

    // This should fail:
    res = GetWithToken(p, crm_token);
    { SCOPED_TRACE(fmt::format("Authenticated Get crm \"{}\"", p));
      EXPECT_EQ(res->status, 400); }

    // This should succeed:
    res = GetWithToken(p, admin_token);
    { SCOPED_TRACE(fmt::format("Authenticated Get admin \"{}\"", p));
      EXPECT_EQ(res->status, 200); }
  });

  // TODO: Maybe we can try the crm/admin tokens here, and check for 200
  //       and maybe this should go into its own test function, out of the get/index tests...
  res = PostWithToken("/api/crm/people/search");
  EXPECT_EQ(res->status, 400);

  // NOTE: the body on this res is in html. No need to test it really...
  res = PostWithToken("/api/crm/people/search", "", crm_token);
  EXPECT_EQ(res->status, 200);
}

TEST_F(AuthenticationAccess, test_mutant_authorization_access) {
  // TODO: Test malformed authorization content (no bearer prefix), unavailable tokens, and expired tokens
}
