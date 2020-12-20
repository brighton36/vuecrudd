#pragma once
#include "rest_controller.hpp"
#include "crm_company_comment_type.hpp"

class CrmCompanyCommentTypesController : 
public Controller::RestInstance<CrmCompanyCommentTypesController, CrmCompanyCommentType> { 
  public:
    static constexpr std::string_view rest_prefix = { "/api/crud/crm/company-comment-types" };
    using Controller::RestInstance<CrmCompanyCommentTypesController, CrmCompanyCommentType>::RestInstance;

  private:
    static ControllerRegister<CrmCompanyCommentTypesController> reg;
};
