#pragma once
#include "rest_controller.hpp"
#include "crm_person_comment_type.hpp"

class CrmPersonCommentTypesController : 
public Controller::RestInstance<CrmPersonCommentTypesController, CrmPersonCommentType> { 
  public:
    static constexpr std::string_view rest_prefix = { "/api/crm/person_comment_types" };
    using Controller::RestInstance<CrmPersonCommentTypesController, CrmPersonCommentType>::RestInstance;

  private:
    static ControllerRegister<CrmPersonCommentTypesController> reg;
};
