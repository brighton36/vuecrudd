#include <random>

#include "picosha2.h"
#include "base64.hpp"

#include "account.hpp"

using namespace std;

REGISTER_MODEL(Account)

void Account::Migrate() {
  CreateTable({
    {"first_name", "varchar(100)"},
    {"last_name", "varchar(100)"},
    {"email", "varchar(100)"},
    {"password", "varchar(100)"},
    // This is a bit sloppy, I should probably calculate the exact size. But, 
    // roughly, 128 characters of randomness is 172 characters of base64
    {"auth_token", fmt::format("varchar({})",auth_token_size*2)},
    {"auth_token_issued_at", "datetime"},
    {"created_at", "datetime"},
    {"updated_at", "datetime"}
  });

  tm tm_time = Model::NowUTC();
  auto first_account = Account({
    {"first_name",  "Guest"},
    {"last_name",   "User"},
    {"email",       "guest@vue-crud.com"},
    {"created_at",  tm_time},
    {"updated_at",  tm_time}
  });
  first_account.password("demopassword123");
  first_account.save();
};

optional<string> Account::full_name() { 
  auto fname = first_name();
  auto lname = last_name();

  if (!fname && !lname) return nullopt;

  string ret;
  if (fname) ret.append(*fname);
  if (fname && lname) ret.append(" ");
  if (lname) ret.append(*lname);

  return make_optional(ret);
};

void Account::password(const optional<Model::RecordValue> &val) { 
  recordSet("password", Account::Hash(get<string>(*val))); 
};
optional<string> Account::password() { 
  auto val = recordGet("password");
  return (val) ? make_optional(get<string>(*val)) : nullopt;
}

void Account::generate_new_auth_token() { 
  using random_bytes_engine = independent_bits_engine<
    default_random_engine, CHAR_BIT, unsigned char>;

  random_bytes_engine rbe;
  vector<base64::byte> source(auth_token_size);
  generate(begin(source), end(source), ref(rbe));

  auth_token(base64::encode(source));
  auth_token_issued_at(Model::NowUTC());
}

void Account::clear_auth_token() { 
  auth_token(nullopt);
  auth_token_issued_at(nullopt);
}

optional<Account> Account::FromLogin(const string &email, 
  const string &password) {
  return Account::Find("email = :email and password = :password", 
    Model::Record({{"email", email}, {"password", Account::Hash(password)}}));
}

optional<Account> Account::FromToken(const string &token) {
  if (token.empty()) return nullopt;

  // TODO: accomodate < ndays from generation date
  return Account::Find("auth_token = :auth_token", Model::Record({{"auth_token", token}}));
}

string Account::Hash(const string &unsalted_phrase) {
  // TODO: store the salt in a config: Controller::GetConfig().cors_allow()
  string salt = "I94anUGppukOHVAmL2a_kSmS"; // A random string
  string source = salt+unsalted_phrase;

  vector<unsigned char> hash(picosha2::k_digest_size);
  picosha2::hash256(source.begin(), source.end(), hash.begin(), hash.end());

  return picosha2::bytes_to_hex_string(hash.begin(), hash.end());
}
