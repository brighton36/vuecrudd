#pragma once
#include "rest_controller.hpp"
#include "crm_person_comment.hpp"
#include "vuecrudd_utilities.hpp"

class CrmPersonCommentsController : 
public Controller::RestInstance<CrmPersonCommentsController, CrmPersonComment> { 
  public:
    static constexpr std::string_view rest_prefix = { "/api/crm/person-comments" };
    static constexpr std::string_view rest_actions[]= { "index", "read" };
    using Controller::RestInstance<CrmPersonCommentsController, CrmPersonComment>::RestInstance;

    static void Routes(Pistache::Rest::Router&, std::shared_ptr<Controller::Instance>);
  private:
    void modelUpdate(CrmPersonComment &model, Controller::PostBody &post, std::tm tm_time) {
      modelUpdateVuecrudd(model, post, tm_time);
 
      #define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
      CRM_PERSON_COMMENT_COLUMNS
      #undef COLUMN
    }

    static ControllerRegister<CrmPersonCommentsController> reg;
};
