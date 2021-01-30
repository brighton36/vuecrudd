#pragma once
#include "vuecrud_controller.hpp"

#include "crm_company_comment_type.hpp"

class CrmCompanyCommentTypesController : 
public VuecrudController<CrmCompanyCommentTypesController, CrmCompanyCommentType> { 
  public:
    static constexpr std::string_view read_prefix = { "/api/crud/crm" };
    static constexpr std::string_view basename = { "company-comment-types" };
    using VuecrudController<CrmCompanyCommentTypesController, CrmCompanyCommentType>::VuecrudController;
  private:
    void modelUpdate(CrmCompanyCommentType &, Controller::PostBody &, std::tm);

    static ControllerRegister<CrmCompanyCommentTypesController> reg;
};
