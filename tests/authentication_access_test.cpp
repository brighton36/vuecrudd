#include "vuecrud_gtest.hpp"

#include "user.hpp"
#include <iostream> //todo

using namespace std;

class AuthenticationAccess : public VuecrudControllerTest {
  protected:
    shared_ptr<httplib::Response> GetWithToken(string url, string token = "") {
      httplib::Headers headers({{"Content-Type", "application/json; charset=utf8"}});

      if (!token.empty())
        headers.insert({"Authorization", fmt::format("Bearer {}", token)});

      return browser().Get(url.c_str(), headers);
    }
};

PSYM_TEST_ENVIRONMENT();

TEST_F(AuthenticationAccess, test_user_index_access) {
  vector<string> indexes({"/api/crm/companies", "/api/crud/crm/company-types", 
    "/api/crm/company-files", "/api/crm/company-comments", 
    "/api/crud/crm/company-comment-types", "/api/crm/person-comments",
    "/api/crm/person-comment-types", "/api/crm/positions", 
    "/api/crm/position-tasks", "/api/crud/crm/tasks"});

  auto client = browser();

  // These should all be denied:
  for_each(indexes.begin(), indexes.end(), [&] (auto p) {
    auto res = GetWithToken(p);
    { SCOPED_TRACE(fmt::format("Un-Authenticated Get \"{}\"", p));
      ASSERT_EQ(res->status, 500);
      ASSERT_EQ(res->body, "{\"error\":\"token_not_provided\"}"); }
  });

  string token = login_user();

  // TODO: Post : /api/crm/people/search

  // These should all suceed:
  for_each(indexes.begin(), indexes.end(), [&] (auto p) {
    auto res = GetWithToken(p, token);
    { SCOPED_TRACE(fmt::format("Authenticated Get \"{}\"", p));
      ASSERT_EQ(res->status, 200); }
  });
  
  // TODO: Post : /api/crm/people/search
}

TEST_F(AuthenticationAccess, test_admin_index_access) {
  // Admin only: 
  vector<string> indexes({"/api/admin/users", "/api/crud/admin/permissions", 
    "/api/admin/user-permissions"});
  // TODO: both user and Admin user login
}
