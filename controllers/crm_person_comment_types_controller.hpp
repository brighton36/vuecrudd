#pragma once
#include "rest_controller.hpp"
#include "crm_person_comment_type.hpp"
#include "vuecrudd_utilities.hpp"

class CrmPersonCommentTypesController : 
public Controller::RestInstance<CrmPersonCommentTypesController, CrmPersonCommentType> { 
  public:
    static constexpr std::string_view rest_prefix = { "/api/crm/person-comment-types" };
    static constexpr std::string_view rest_actions[]= { "index", "read" };
    using Controller::RestInstance<CrmPersonCommentTypesController, CrmPersonCommentType>::RestInstance;

    static void Routes(Pistache::Rest::Router&, std::shared_ptr<Controller::Instance>);
  private:
    void modelUpdate(CrmPersonCommentType &model, Controller::PostBody &post, std::tm tm_time) {
      modelUpdateVuecrudd(model, post, tm_time);
 
      #define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
      CRM_PERSON_COMMENT_TYPE_COLUMNS
      #undef COLUMN
    }

    static ControllerRegister<CrmPersonCommentTypesController> reg;
};
