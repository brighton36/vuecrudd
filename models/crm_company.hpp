#pragma once
#include "model.hpp"
#include "model_factory.hpp"

#define CRM_COMPANY_COLUMNS                                             \
  COLUMN(name,                            std::string,   "varchar(1000)") \
  COLUMN(common_name,                     std::string,   "varchar(150)")  \
  COLUMN(company_type_id,                 long long int, "int")           \
  COLUMN(nip,                             std::string,   "varchar(10)")   \
  COLUMN(regon,                           std::string,   "varchar(9)")    \
  COLUMN(krs,                             std::string,   "varchar(10)")   \
  COLUMN(street_prefix_id,                long long int, "int")           \
  COLUMN(street,                          std::string,   "varchar(255)")  \
  COLUMN(house_number,                    std::string,   "varchar(50)")   \
  COLUMN(apartment_number,                std::string,   "varchar(50)")   \
  COLUMN(zip_code,                        std::string,   "varchar(6)")    \
  COLUMN(city,                            std::string,   "varchar(255)")  \
  COLUMN(borough,                         std::string,   "varchar(255)")  \
  COLUMN(county,                          std::string,   "varchar(255)")  \
  COLUMN(voivodship,                      std::string,   "varchar(255)")  \
  COLUMN(email,                           std::string,   "varchar(255)")  \
  COLUMN(web_page,                        std::string,   "varchar(255)")  \
  COLUMN(fax,                             std::string,   "varchar(50)")   \
  COLUMN(coordinates_lat,                 double,        "double")        \
  COLUMN(coordinates_lng,                 double,        "double")        \
  COLUMN(coordinates_checked,             int,           "int")           \
  COLUMN(google_map_place,                std::string,   "varchar(300)")  \
  COLUMN(parent_company_id,               long long int, "int")           \
  COLUMN(correspondence_street_prefix_id, long long int, "int")           \
  COLUMN(correspondence_street,           std::string,   "varchar(255)")  \
  COLUMN(correspondence_house_number,     std::string,   "varchar(50)")   \
  COLUMN(correspondence_apartment_number, std::string,   "varchar(50)")   \
  COLUMN(correspondence_zip_code,         std::string,   "varchar(6)")    \
  COLUMN(correspondence_city,             std::string,   "varchar(255)")  \
  COLUMN(correspondence_borough,          std::string,   "varchar(255)")  \
  COLUMN(correspondence_county,           std::string,   "varchar(255)")  \
  COLUMN(correspondence_voivodship,       std::string,   "varchar(255)")  \
  COLUMN(active,                          int,           "int")           \
  COLUMN(created_at,                      std::tm,       "datetime")      \
  COLUMN(updated_at,                      std::tm,       "datetime")

class CrmCompany : public Model::Instance<CrmCompany> { 
  public:
    using Model::Instance<CrmCompany>::Instance;

    MODEL_ACCESSOR(id, long long int)
    #define COLUMN(a, t, _) MODEL_ACCESSOR(a, t)
      CRM_COMPANY_COLUMNS
    #undef COLUMN

    inline static const Model::Definition Definition {
      "id",
      "companies", 
      Model::ColumnTypes({
        {"id", COL_TYPE(long long int)},
        #define COLUMN(a, t, _) {#a, COL_TYPE(t)},
        CRM_COMPANY_COLUMNS
        #undef COLUMN
      }),
      Model::Validations( {
        /*
        // TODO
        'company_type_id' => 'exists:company_types,id',
         */
        Model::Validates::Matches("email", 
          prails::utilities::regex_from_string("/.+@.+/")),
        Model::Validates::NotNull("name"),
        Model::Validates::NotNull("common_name"),
        Model::Validates::MaxLength("common_name", 1000),
        Model::Validates::NotNull("company_type_id"),
        Model::Validates::NotNull("street_prefix_id"),
        Model::Validates::NotNull("city"),
        Model::Validates::NotNull("street"),
        Model::Validates::NotNull("email"),
        Model::Validates::IsBoolean("active"),
        Model::Validates::NotNull("created_at"),
        Model::Validates::NotNull("updated_at")
      })
    };

    std::string address();

    static void Migrate();

  private:
    static ModelRegister<CrmCompany> reg;
};
