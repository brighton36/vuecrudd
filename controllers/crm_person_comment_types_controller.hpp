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
    void model_update(CrmPersonCommentType &, Controller::PostBody &, std::tm);

    static ControllerRegister<CrmPersonCommentTypesController> reg;
};
