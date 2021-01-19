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
    void modelUpdate(CrmCompanyCommentType &model, Controller::PostBody &post, std::tm tm_time) {
      modelUpdateVuecrud(model, post, tm_time);
      #define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
      CRM_COMPANY_COMMENT_TYPE_COLUMNS
      #undef COLUMN
    }

    static ControllerRegister<CrmCompanyCommentTypesController> reg;
};
