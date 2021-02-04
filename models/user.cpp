#include <random>
#include <climits>

#include "user.hpp"

#include "picosha2.h"
#include "base64.hpp"

using namespace std;

PSYM_MODEL(User)

void User::Migrate() {
  CreateTable({
    #define COLUMN(a, _, t) {#a, t},
    USER_COLUMNS
    #undef COLUMN
    {"password", "varchar(100)"},
  });
};

void User::password(const optional<Model::RecordValue> &val) { 
  recordSet("password", User::Hash(get<string>(*val))); 
};

optional<string> User::password() { 
  auto val = recordGet("password");
  return (val) ? make_optional(get<string>(*val)) : nullopt;
}

void User::generate_new_auth_token() { 
  using random_bytes_engine = independent_bits_engine<
    default_random_engine, CHAR_BIT, unsigned char>;

  random_bytes_engine rbe;
  vector<base64::byte> source(auth_token_size);
  generate(begin(source), end(source), ref(rbe));

  auth_token(base64::encode(source));
  auth_token_issued_at(Model::NowUTC());
}

void User::clear_auth_token() { 
  auth_token(nullopt);
  auth_token_issued_at(nullopt);
}

bool User::is_authorized(const string &, const string &) {
  // TODO: 
  return true;
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
    Model::Record({{"email", email}, {"password", hashed_password}}));
}

optional<User> User::FromHeader(optional<string> token) {
  smatch matches;
  if ( (!token.has_value()) || ((*token).empty()) || 
    (!regex_search(*token, matches, regex("^Bearer (.+)$"))) )
    return nullopt;

  // TODO: accomodate < ndays from generation date
  return Find("auth_token = :auth_token", {{"auth_token", matches[1]}});
}
