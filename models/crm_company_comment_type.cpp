#include "crm_company_comment_type.hpp"

using namespace std;

REGISTER_MODEL(CrmCompanyCommentType);

void CrmCompanyCommentType::Migrate() {
  CreateTable({
    #define COLUMN(a, _, t) {#a, t},
    CRM_COMPANY_COMMENT_TYPE_COLUMNS
    #undef COLUMN
  });
};
