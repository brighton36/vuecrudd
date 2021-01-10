#pragma once
#include "vuecrud_controller.hpp"

#include "crm_person_comment.hpp"

class CrmPersonCommentsController : 
public VuecrudController<CrmPersonCommentsController, CrmPersonComment> { 
  public:
    static constexpr std::string_view basename = { "person-comments" };
    using VuecrudController<CrmPersonCommentsController, CrmPersonComment>::VuecrudController;
  private:
    void modelUpdate(CrmPersonComment &model, Controller::PostBody &post, std::tm tm_time) {
      modelUpdateVuecrud(model, post, tm_time);
      #define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
      CRM_PERSON_COMMENT_COLUMNS
      #undef COLUMN
    }

    static ControllerRegister<CrmPersonCommentsController> reg;
};
