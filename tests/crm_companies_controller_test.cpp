#include "prails_gtest.hpp"

#include "crm_company.hpp"
#include "crm_company_type.hpp"
#include "crm_street_prefix.hpp"

using namespace std;

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
      {"active", 1}
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
    tm now = Model::NowUTC();

    void SetUp() override {
      PrailsEnvironment::SetUp();
      

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

      CrmCompany agag(agag_company_record);
      agag.updated_at(now);
      agag.created_at(now);
      if (!agag.isValid())
        throw runtime_error("Unable to setup agag");
      agag.save();
    }
};

class CrmCompanyControllerTest : public PrailsControllerTest {
  protected:
    // This output was copy and pasted from a faker seed in the laravel 
    // implementation:
    static constexpr string_view _agag_company { 
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
  
    // This ensures the provided json, matches what we expect to see in the 
    // above _agag_company:
    void assert_agag(nlohmann::json agag_json, bool is_timestamped = false) {
      vector<string> agag_attrs;

      for (auto& i : agag_json.items()) 
        agag_attrs.push_back(i.key());

      vector<string> expected_null_attrs = { "nip","regon", "krs", "house_number", 
        "apartment_number", "zip_code", "borough", "county", "voivodship", 
        "web_page", "fax", "coordinates_lat", "coordinates_lng", 
        "coordinates_checked", "google_map_place", "parent_company_id", 
        "correspondence_street", "correspondence_house_number", 
        "correspondence_apartment_number", "correspondence_zip_code", 
        "correspondence_city", "correspondence_borough", "correspondence_county", 
        "correspondence_voivodship"};

      vector<string> expected_agag_attrs = {"id", "name", "common_name", 
        "company_type_id", "street_prefix_id", "street", "city", "email", 
        "correspondence_street_prefix_id", "active", "address", "company_type",
        "street_prefix"};

      if (is_timestamped) {
        // This (probably) means agag_json was pulled from the database, and has
        // a timestamp
        expected_agag_attrs.push_back("created_at");
        expected_agag_attrs.push_back("updated_at");
      } else {
        expected_null_attrs.push_back("created_at");
        expected_null_attrs.push_back("updated_at");
      }

      expected_agag_attrs.insert(expected_agag_attrs.begin(), 
        expected_null_attrs.begin(), expected_null_attrs.end());

      sort(agag_attrs.begin(), agag_attrs.end());
      sort(expected_agag_attrs.begin(), expected_agag_attrs.end());

      EXPECT_EQ(agag_attrs, expected_agag_attrs);

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


      for (const auto &attr : expected_null_attrs) {
        ASSERT_NO_THROW(agag_json.at(attr));
        EXPECT_TRUE(agag_json[attr].is_null());
      }

      ASSERT_TRUE(agag_json["company_type"].is_object());

      vector<string> agag_company_type_attrs;
      for (auto& i : agag_json["company_type"].items()) 
        agag_company_type_attrs.push_back(i.key());

      sort(agag_company_type_attrs.begin(), agag_company_type_attrs.end());

      EXPECT_EQ(vector<string>({"active", "code", "created_at", "id", "name", 
        "updated_at"}), agag_company_type_attrs);
      EXPECT_EQ(agag_json["company_type"]["id"], 2);
      EXPECT_EQ(agag_json["company_type"]["name"], "Firmy IT");
      EXPECT_EQ(agag_json["company_type"]["code"], "FIT");
      EXPECT_EQ(agag_json["company_type"]["active"], 1);
      if (is_timestamped) {
        EXPECT_FALSE(agag_json["company_type"]["updated_at"].is_null());
        EXPECT_FALSE(agag_json["company_type"]["created_at"].is_null());
      } else {
        EXPECT_TRUE(agag_json["company_type"]["updated_at"].is_null());
        EXPECT_TRUE(agag_json["company_type"]["created_at"].is_null());
      }

      ASSERT_TRUE(agag_json["street_prefix"].is_object());

      vector<string> agag_street_prefix_attrs;
      for (auto& i : agag_json["street_prefix"].items()) 
        agag_street_prefix_attrs.push_back(i.key());

      sort(agag_street_prefix_attrs.begin(), agag_street_prefix_attrs.end());

      EXPECT_EQ(vector<string>({"active", "created_at", "description", "id",
        "name", "updated_at"}), agag_street_prefix_attrs);

      EXPECT_EQ(agag_json["street_prefix"]["id"], 1);
      EXPECT_EQ(agag_json["street_prefix"]["name"], "");
      EXPECT_EQ(agag_json["street_prefix"]["description"], "-");
      EXPECT_EQ(agag_json["street_prefix"]["active"], 1);
      if (is_timestamped) {
        EXPECT_FALSE(agag_json["street_prefix"]["updated_at"].is_null());
        EXPECT_FALSE(agag_json["street_prefix"]["created_at"].is_null());
      } else {
        EXPECT_TRUE(agag_json["street_prefix"]["updated_at"].is_null());
        EXPECT_TRUE(agag_json["street_prefix"]["created_at"].is_null());
      }
    }
};

PSYM_TEST_ENVIRONMENT_WITH(CrmCompaniesControllerEnvironment);

// Here, we're mostly just testing that our frameworks provide expected results:
TEST_F(CrmCompanyControllerTest, test_json_parse) {
  { 
    SCOPED_TRACE("Laravel-reference implementation output");
    assert_agag(nlohmann::json::parse(_agag_company)); 
  }
}

TEST_F(CrmCompanyControllerTest, success) {
  // TODO: We'll need to authenticate ...

  auto res = browser().Get("/api/crm/companies");

  ASSERT_EQ(res->status, 200);

  auto json_body = nlohmann::json::parse(res->body);

  ASSERT_TRUE(json_body.is_array());
  ASSERT_EQ(json_body.size(), 1);

  { 
    SCOPED_TRACE("CrmCompanyController controller produced output");
    assert_agag(json_body[0], true);

    EXPECT_TRUE(json_body[0]["updated_at"].is_string());
    EXPECT_FALSE(json_body[0]["updated_at"].empty());
    EXPECT_TRUE(json_body[0]["created_at"].is_string());
    EXPECT_FALSE(json_body[0]["created_at"].empty());
  }
}
