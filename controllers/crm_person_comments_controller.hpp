#pragma once
#include "rest_controller.hpp"
#include "crm_person_comment.hpp"

class CrmPersonCommentsController : 
public Controller::RestInstance<CrmPersonCommentsController, CrmPersonComment> { 
  public:
    static constexpr std::string_view rest_prefix = { "/api/crm/person_comments" };
    using Controller::RestInstance<CrmPersonCommentsController, CrmPersonComment>::RestInstance;

  private:
    static ControllerRegister<CrmPersonCommentsController> reg;
};
