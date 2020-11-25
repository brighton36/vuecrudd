#include "crm_company_comment.hpp"

using namespace std;

REGISTER_MODEL(CrmCompanyComment);

void CrmCompanyComment::Migrate() {
  CreateTable({
    #define COLUMN(a, _, t) {#a, t},
    CRM_COMPANY_COMMENT_COLUMNS
    #undef COLUMN
  });
};
