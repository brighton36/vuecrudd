#pragma once
#include "rest_controller.hpp"
#include "crm_company_comment.hpp"

class CrmCompanyCommentsController : 
public Controller::RestInstance<CrmCompanyCommentsController, CrmCompanyComment> { 
  public:
    static constexpr std::string_view rest_prefix = { "/api/crm/company-comments" };
    using Controller::RestInstance<CrmCompanyCommentsController, CrmCompanyComment>::RestInstance;

  private:
    static ControllerRegister<CrmCompanyCommentsController> reg;
};
