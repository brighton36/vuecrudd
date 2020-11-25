#include "crm_person_comment_type.hpp"

using namespace std;

REGISTER_MODEL(CrmPersonCommentType);

void CrmPersonCommentType::Migrate() {
  CreateTable({
    #define COLUMN(a, _, t) {#a, t},
    CRM_PERSON_COMMENT_TYPE_COLUMNS
    #undef COLUMN
  });
};
