#pragma once
#include "vuecrud_controller.hpp"

#include "crm_company_comment.hpp"

class CrmCompanyCommentsController : 
public VuecrudController<CrmCompanyCommentsController, CrmCompanyComment> { 
  public:
    static constexpr std::string_view read_prefix = { "/api/crm" };
    static constexpr std::string_view rest_prefix = { "/api/crm" };
    static constexpr std::string_view basename = { "company-comments" };
    using VuecrudController<CrmCompanyCommentsController, CrmCompanyComment>::VuecrudController;

    Controller::Response index(const Pistache::Rest::Request&);
    static std::optional<std::string> prefix(const std::string &action) {
      // This is just some odd exception to the routing patterns...
      if ((action == "read") || (action == "multiple_update"))
        return "/api/crud/crm/company-comments";
      return VuecrudController<CrmCompanyCommentsController, CrmCompanyComment>::prefix(action);
    }
  private:
    void modelUpdate(CrmCompanyComment &, Controller::PostBody &, std::tm);

    static ControllerRegister<CrmCompanyCommentsController> reg;
};
