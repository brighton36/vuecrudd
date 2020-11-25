#include "crm_person_comment.hpp"

using namespace std;

REGISTER_MODEL(CrmPersonComment);

void CrmPersonComment::Migrate() {
  CreateTable({
    #define COLUMN(a, _, t) {#a, t},
    CRM_PERSON_COMMENT_COLUMNS
    #undef COLUMN
  });
};
