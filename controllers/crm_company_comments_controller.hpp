#pragma once
#include "rest_controller.hpp"
#include "crm_company_comment.hpp"
#include "vuecrudd_utilities.hpp"

class CrmCompanyCommentsController : 
public Controller::RestInstance<CrmCompanyCommentsController, CrmCompanyComment> { 
  public:
    static constexpr std::string_view rest_prefix = { "/api/crm/company-comments" };
    static constexpr std::string_view rest_actions[]= { "index", "read" };
    using Controller::RestInstance<CrmCompanyCommentsController, CrmCompanyComment>::RestInstance;

    static void Routes(Pistache::Rest::Router&, std::shared_ptr<Controller::Instance>);
  private:
    void modelUpdate(CrmCompanyComment &model, Controller::PostBody &post, std::tm tm_time) {
      modelUpdateVuecrudd(model, post, tm_time);
      
      #define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
      CRM_COMPANY_COMMENT_COLUMNS
      #undef COLUMN
    }

    static ControllerRegister<CrmCompanyCommentsController> reg;
};
