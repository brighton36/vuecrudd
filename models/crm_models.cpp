#include <variant>
#include <nlohmann/json.hpp>

#include "vuecrud_utilities.hpp"

#include "crm_company.hpp"
#include "crm_company_comment.hpp"
#include "crm_company_comment_type.hpp"
#include "crm_company_file.hpp"
#include "crm_company_type.hpp"
#include "crm_language.hpp"
#include "crm_person.hpp"
#include "utilities.hpp"
#include "crm_person_comment.hpp"
#include "crm_person_comment_type.hpp"
#include "crm_position.hpp"
#include "crm_position_task.hpp"
#include "crm_sex.hpp"
#include "crm_street_prefix.hpp"
#include "permission.hpp"

using namespace std;

PSYM_MODEL(CrmCompany);
PSYM_MODEL(CrmCompanyComment);
PSYM_MODEL(CrmCompanyCommentType);
PSYM_MODEL(CrmCompanyFile);
PSYM_MODEL(CrmCompanyType);
PSYM_MODEL(CrmLanguage);
PSYM_MODEL(CrmPerson);
PSYM_MODEL(CrmPersonComment);
PSYM_MODEL(CrmPersonCommentType);
PSYM_MODEL(CrmPosition);
PSYM_MODEL(CrmPositionTask);
PSYM_MODEL(CrmSex);
PSYM_MODEL(CrmStreetPrefix);

// Migrate()
#define COLUMN(a, _, t) {#a, t}, 
#define MODEL_MIGRATE(m, columns) void m::Migrate() {CreateTable({ columns });}
MODEL_MIGRATE(CrmCompany, CRM_COMPANY_COLUMNS);
MODEL_MIGRATE(CrmCompanyComment, CRM_COMPANY_COMMENT_COLUMNS);
MODEL_MIGRATE(CrmCompanyCommentType, CRM_COMPANY_COMMENT_TYPE_COLUMNS);
MODEL_MIGRATE(CrmCompanyFile, CRM_COMPANY_FILE_COLUMNS);
MODEL_MIGRATE(CrmCompanyType, CRM_COMPANY_TYPE_COLUMNS);
MODEL_MIGRATE(CrmLanguage, CRM_LANGUAGE_COLUMNS);
MODEL_MIGRATE(CrmPerson, CRM_PERSON_COLUMNS);
MODEL_MIGRATE(CrmPersonComment, CRM_PERSON_COMMENT_COLUMNS);
MODEL_MIGRATE(CrmSex, CRM_SEX_COLUMNS);
MODEL_MIGRATE(CrmStreetPrefix, CRM_STREET_PREFIX_COLUMNS);
MODEL_MIGRATE(CrmPersonCommentType, CRM_PERSON_COMMENT_TYPE_COLUMNS);
MODEL_MIGRATE(CrmPosition, CRM_POSITION_COLUMNS);
MODEL_MIGRATE(CrmPositionTask, CRM_POSITION_TASK_COLUMNS);
#undef COLUMN 

// to_json()
#define COLUMN(a, _1, _2) ret[#a] = ColumnValueToJson(a());
#define MODEL_TO_JSON(m, columns) nlohmann::json m::to_json() { \
    nlohmann::json ret({{"id", ColumnValueToJson(id())}});      \
    columns; return ret; }
MODEL_TO_JSON(CrmCompanyComment, CRM_COMPANY_COMMENT_COLUMNS);
MODEL_TO_JSON(CrmCompanyCommentType, CRM_COMPANY_COMMENT_TYPE_COLUMNS);
MODEL_TO_JSON(CrmCompanyFile, CRM_COMPANY_FILE_COLUMNS);
MODEL_TO_JSON(CrmCompanyType, CRM_COMPANY_TYPE_COLUMNS);
MODEL_TO_JSON(CrmLanguage, CRM_LANGUAGE_COLUMNS);
MODEL_TO_JSON(CrmPersonComment, CRM_PERSON_COMMENT_COLUMNS);
MODEL_TO_JSON(CrmSex, CRM_SEX_COLUMNS);
MODEL_TO_JSON(CrmStreetPrefix, CRM_STREET_PREFIX_COLUMNS);
MODEL_TO_JSON(CrmPersonCommentType, CRM_PERSON_COMMENT_TYPE_COLUMNS);
MODEL_TO_JSON(CrmPositionTask, CRM_POSITION_TASK_COLUMNS);

nlohmann::json CrmCompany::to_json() { 
  nlohmann::json ret({{"id", ColumnValueToJson(id())}});
  CRM_COMPANY_COLUMNS; 
  ret["address"] = address();
  return ret; 
}

nlohmann::json CrmPerson::to_json() { 
  nlohmann::json ret({{"id", ColumnValueToJson(id())}});
  CRM_PERSON_COLUMNS; 
  ret["fullname"] = fullname();
  return ret; 
}

nlohmann::json CrmPosition::to_json() { 
  nlohmann::json ret({{"id", ColumnValueToJson(id())}});
  CRM_POSITION_COLUMNS; 
  // The modelSelect() joins these columns for us.  
  if (auto company_id = recordGet("company_id"); company_id.has_value()) {
    ret["company"] = nlohmann::json::object({{"common_name", "todo"}});
    ret["company"]["id"] = get<long long int>(*company_id);

    if (auto company_name = recordGet("company_name"); company_name.has_value())
      ret["company"]["common_name"] = get<string>(*company_name);
  }
  if (auto person_id = recordGet("person_id"); person_id.has_value()) {
    ret["person"] = nlohmann::json::object({{"fullname", "todo"}});
    ret["person"]["id"] = get<long long int>(*person_id);
    if (auto person_fullname = recordGet("person_fullname"); person_fullname.has_value())
      ret["person"]["fullname"] = get<string>(*person_fullname);
  }
  return ret; 
}
#undef COLUMN 


string CrmPerson::fullname() {
  vector<string> fullname;
  if (firstname().has_value()) fullname.push_back(*firstname());
  if (lastname().has_value()) fullname.push_back(*lastname());
  return prails::utilities::join(fullname, " ");
}

string CrmCompany::address() {
  string addr;

  if (street().has_value())
    addr.append(street().value());

  // This is how the reference worked... (shrug)
  addr.append(" ");

  if (house_number().has_value())
    addr.append(house_number().value());

  if (apartment_number().has_value() && (!(*apartment_number()).empty()))
    addr.append(" lok. "+*apartment_number());

  // Remove excess whitespace:
  string ret;
  regex_replace(back_inserter(ret), addr.begin(), addr.end(), regex("\\s+"), " ");

  return ret;
}
