#include <random>
#include <climits>

#include "picosha2.h"
#include "base64.hpp"

#include "user.hpp"
#include "permission.hpp"
#include "vuecrud_utilities.hpp"

using namespace std;

PSYM_MODEL(User)

void User::Migrate(unsigned int) {
  CreateTable({
    #define COLUMN(a, _, t) {#a, t},
    USER_COLUMNS
    #undef COLUMN
    {"password", "varchar(100)"},
  });
};

vector<Permission> User::permissions(bool reload_cache = false) {
  if ((reload_cache || (!_permissions.has_value())) && (id().has_value())) {
    std::string query = fmt::format(
      "select distinct p.* from {table_name} as p {join} where {where}",
      fmt::arg("table_name", Permission::Definition.table_name()),
      fmt::arg("join", 
        "join user_permissions as u_p on "
        "u_p.permission_id = p.id and u_p.user_id = :user_id"),
      fmt::arg("where", "p.active = :active") );

    Model::Record query_params({{"active", 1}, {"user_id", id().value()} });

    _permissions = make_optional<vector<Permission>>(Permission::Select(query, query_params));
  }
  
  return (_permissions.has_value()) ? _permissions.value() : vector<Permission>();
}
void User::password(const optional<Model::RecordValue> &val) { 
  recordSet("password", User::Hash(get<string>(*val))); 
};

optional<string> User::password() { 
  auto val = recordGet("password");
  return (val) ? make_optional(get<string>(*val)) : nullopt;
}

void User::generate_new_auth_token() { 
  random_device rd;
  using random_bytes_engine = independent_bits_engine<
    default_random_engine, CHAR_BIT, unsigned char>;

  random_bytes_engine rbe(rd());

  vector<base64::byte> source(auth_token_size);
  generate(begin(source), end(source), ref(rbe));

  auth_token(base64::encode(source));
  auth_token_issued_at(Model::NowUTC());
}

void User::clear_auth_token() { 
  auth_token(nullopt);
  auth_token_issued_at(nullopt);
}

nlohmann::json User::to_json() {
  return nlohmann::json({
    {"id", ColumnValueToJson(id())},
    {"name", ColumnValueToJson(name())},
    {"email", ColumnValueToJson(email())},
    {"user_type_id", ColumnValueToJson(user_type_id())},
    {"active", ColumnValueToJson(active())},
    {"created_at", ColumnValueToJson(created_at())},
    {"updated_at", ColumnValueToJson(updated_at())}
  });
}

bool User::is_authorized(const string &controller, const string &action) {
  // We kind of take advantage of camel case here to take the first word, and
  // compare that to the path
  smatch matches;

  // I don't actually think this regex should ever fail....
  if (!regex_search(controller, matches, regex("^(.[^A-Z]*)"))) return false;

  string controller_prefix = matches[1];

  // Lowercase it:
  transform(controller_prefix.begin(), controller_prefix.end(), 
    controller_prefix.begin(), [](unsigned char c){ return tolower(c); });

  for(auto &perm : permissions())
    if (perm.path().has_value() && (perm.path().value() == controller_prefix))
      return true;
  
  return false;
}

string User::authorizer_instance_label() { 
  return fmt::format("{}:{}", *id(), *name()); 
}

string User::Hash(const string &unsalted_phrase) {
  // TODO: store the salt in a config: Controller::GetConfig().cors_allow()
  string salt = "I94anUGppukOHVAmL2a_kSmS"; // A random string
  string source = salt+unsalted_phrase;

  vector<unsigned char> hash(picosha2::k_digest_size);
  picosha2::hash256(source.begin(), source.end(), hash.begin(), hash.end());

  return picosha2::bytes_to_hex_string(hash.begin(), hash.end());
}

optional<User> User::FromLogin(const string &email, const string &password) {
  string hashed_password = User::Hash(password);
  return User::Find("email = :email and password = :password", 
    {{"email", email}, {"password", hashed_password}});
}

optional<User> User::FromHeader(optional<string> token) {
  smatch matches;
  if ( (!token.has_value()) || ((*token).empty()) || 
    (!regex_search(*token, matches, regex("^Bearer (.+)$"))) )
    return nullopt;

  // TODO: accomodate < ndays from generation date
  return Find("auth_token = :auth_token", {{"auth_token", matches[1]}});
}
