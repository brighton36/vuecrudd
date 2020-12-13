#include "prails_gtest.hpp"

#include "crm_company.hpp"
#include "crm_company_type.hpp"
#include "crm_street_prefix.hpp"

#include <iostream> // TODO

using namespace std;

class CrmCompanyControllerTest : public PrailsControllerTest {
  protected:
    // This output was copy and pasted from a faker seed in the laravel 
    // implementation:
    static constexpr std::string_view _agag_company { 
      "{\"id\":10,\"name\":\"Abbott, Glover and Green\",\"common_name\":\"Abbo"
      "tt, Glover and Green\",\"company_type_id\":2,\"nip\":null,\"regon\":nul"
      "l,\"krs\":null,\"street_prefix_id\":1,\"street\":\"926 Swift Hill\",\"h"
      "ouse_number\":null,\"apartment_number\":null,\"zip_code\":null,\"city\""
      ":\"Bethelbury\",\"borough\":null,\"county\":null,\"voivodship\":null,\""
      "email\":\"olangworth@yahoo.com\",\"web_page\":null,\"fax\":null,\"coord"
      "inates_lat\":null,\"coordinates_lng\":null,\"coordinates_checked\":null"
      ",\"google_map_place\":null,\"parent_company_id\":null,\"correspondence_"
      "street_prefix_id\":1,\"correspondence_street\":null,\"correspondence_ho"
      "use_number\":null,\"correspondence_apartment_number\":null,\"correspond"
      "ence_zip_code\":null,\"correspondence_city\":null,\"correspondence_boro"
      "ugh\":null,\"correspondence_county\":null,\"correspondence_voivodship\""
      ":null,\"active\":1,\"created_at\":null,\"updated_at\":null,\"address\":"
      "\"926 Swift Hill \",\"company_type\":{\"id\":2,\"name\":\"Firmy IT\",\""
      "code\":\"FIT\",\"active\":1,\"created_at\":null,\"updated_at\":null},\""
      "street_prefix\":{\"id\":1,\"name\":\"\",\"description\":\"-\",\"active\""
      ":1,\"created_at\":null,\"updated_at\":null}}"
    };
};

class CrmCompaniesControllerEnvironment : public PrailsEnvironment {
  private:
    Model::Record agag_company_record = {
      {"id", 10},
      {"name", "Abbott, Glover and Green"},
      {"common_name", "Abbott, Glover and Green"},
      {"company_type_id", 2},
      {"street_prefix_id", 1},
      {"street", "926 Swift Hill"},
      {"city", "Bethelbury"},
      {"email", "olangworth@yahoo.com"},
      {"correspondence_street_prefix_id", 1},
      {"active", 1},
      {"address", "926 Swift Hill "}
    };
    Model::Record agag_company_type_record = {
      {"id", 2}, {"name", "Firmy IT"}, {"code", "FIT"}, {"active", 1},
      {"updated_at", nullopt}, {"created_at", nullopt}
    };
    Model::Record agag_street_prefix_record = {
      {"id", 1}, {"name", ""}, {"description", "-"}, {"active", 1},
      {"updated_at", nullopt}, {"created_at", nullopt}
    };
  public:
    void SetUp() override {
      PrailsEnvironment::SetUp();
      
      tm now = Model::NowUTC();

      cout << "Inside setup, running the pseudo-migration" << endl;
      // This is something of a migration:
      CrmCompanyType agag_company_type(agag_company_type_record);
      agag_company_type.updated_at(now);
      agag_company_type.created_at(now);
      if (!agag_company_type.isValid())
        throw runtime_error("Unable to setup agag company_type");
      agag_company_type.save();

      CrmStreetPrefix agag_street_prefix(agag_street_prefix_record);
      agag_street_prefix.updated_at(now);
      agag_street_prefix.created_at(now);
      if (!agag_street_prefix.isValid())
        throw runtime_error("Unable to setup agag street_prefix");
      agag_street_prefix.save();
    }
};

PSYM_TEST_ENVIRONMENT_WITH(CrmCompaniesControllerEnvironment);

// Here, we're mostly just testing that our frameworks provide expected results:
TEST_F(CrmCompanyControllerTest, test_json_parse) {
  auto agag_json = nlohmann::json::parse(_agag_company);

  EXPECT_EQ(agag_json["id"], 10);
  EXPECT_EQ(agag_json["name"], "Abbott, Glover and Green");
  EXPECT_EQ(agag_json["common_name"], "Abbott, Glover and Green");
  EXPECT_EQ(agag_json["company_type_id"], 2);
  EXPECT_EQ(agag_json["street_prefix_id"], 1);
  EXPECT_EQ(agag_json["street"], "926 Swift Hill");
  EXPECT_EQ(agag_json["city"], "Bethelbury");
  EXPECT_EQ(agag_json["email"], "olangworth@yahoo.com");
  EXPECT_EQ(agag_json["correspondence_street_prefix_id"], 1);
  EXPECT_EQ(agag_json["active"], 1);
  EXPECT_EQ(agag_json["address"], "926 Swift Hill ");

  vector<string> null_attrs = { "nip","regon", "krs", "house_number", 
    "apartment_number", "zip_code", "borough", "county", "voivodship", 
    "web_page", "fax", "coordinates_lat", "coordinates_lng", 
    "coordinates_checked", "google_map_place", "parent_company_id", 
    "correspondence_street", "correspondence_house_number", 
    "correspondence_apartment_number", "correspondence_zip_code", 
    "correspondence_city", "correspondence_borough", "correspondence_county", 
    "correspondence_voivodship", "created_at", "updated_at"};

  for (const auto &attr : null_attrs) {
    ASSERT_NO_THROW(agag_json.at(attr));
    EXPECT_TRUE(agag_json[attr].is_null());
  }

  ASSERT_TRUE(agag_json["company_type"].is_object());
  EXPECT_EQ(agag_json["company_type"]["id"], 2);
  EXPECT_EQ(agag_json["company_type"]["name"], "Firmy IT");
  EXPECT_EQ(agag_json["company_type"]["code"], "FIT");
  EXPECT_EQ(agag_json["company_type"]["active"], 1);
  EXPECT_TRUE(agag_json["company_type"]["updated_at"].is_null());
  EXPECT_TRUE(agag_json["company_type"]["created_at"].is_null());

  ASSERT_TRUE(agag_json["street_prefix"].is_object());
  EXPECT_EQ(agag_json["street_prefix"]["id"], 1);
  EXPECT_EQ(agag_json["street_prefix"]["name"], "");
  EXPECT_EQ(agag_json["street_prefix"]["description"], "-");
  EXPECT_EQ(agag_json["street_prefix"]["active"], 1);
  EXPECT_TRUE(agag_json["street_prefix"]["updated_at"].is_null());
  EXPECT_TRUE(agag_json["street_prefix"]["created_at"].is_null());
}

TEST_F(CrmCompanyControllerTest, success) {
  // TODO: We'll need to authenticate ...

  auto res = browser().Get("/api/crm/companies");

  ASSERT_EQ(res->status, 200);

  //cout << "Companies: \n" << res->body << endl;
  // TODO
  auto response = nlohmann::json::parse(res->body);
  //ASSERT_EQ(response["error"], "Invalid credentials");
  ASSERT_EQ(nlohmann::json::parse(_agag_company), response);

}
