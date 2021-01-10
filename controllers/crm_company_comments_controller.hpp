#pragma once
#include "vuecrud_controller.hpp"

#include "crm_company_comment.hpp"

class CrmCompanyCommentsController : 
public VuecrudController<CrmCompanyCommentsController, CrmCompanyComment> { 
  public:
    static constexpr std::string_view basename = { "company-comments" };
    using VuecrudController<CrmCompanyCommentsController, CrmCompanyComment>::VuecrudController;
  private:
    void modelUpdate(CrmCompanyComment &model, Controller::PostBody &post, std::tm tm_time) {
      modelUpdateVuecrud(model, post, tm_time);
      #define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
      CRM_COMPANY_COMMENT_COLUMNS
      #undef COLUMN
    }

    static ControllerRegister<CrmCompanyCommentsController> reg;
};
