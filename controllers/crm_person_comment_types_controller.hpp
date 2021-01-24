#pragma once
#include "vuecrud_controller.hpp"

#include "crm_person_comment_type.hpp"

class CrmPersonCommentTypesController : 
public VuecrudController<CrmPersonCommentTypesController, CrmPersonCommentType> { 
  public:
    static constexpr std::string_view basename = { "person-comment-types" };
    static constexpr std::string_view rest_prefix = { "/api/crm" };
    using VuecrudController<CrmPersonCommentTypesController, CrmPersonCommentType>::VuecrudController;
  private:
    void modelUpdate(CrmPersonCommentType &model, Controller::PostBody &post, std::tm tm_time) {
      modelUpdateVuecrud(model, post, tm_time);
      #define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
      CRM_PERSON_COMMENT_TYPE_COLUMNS
      #undef COLUMN
    }

    static ControllerRegister<CrmPersonCommentTypesController> reg;
};
