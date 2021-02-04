#pragma once
#include "vuecrud_controller.hpp"

#include "crm_person_comment.hpp"

class CrmPersonCommentsController : 
public VuecrudController<CrmPersonCommentsController, CrmPersonComment> { 
  public:
    static constexpr std::string_view basename = { "person-comments" };
    static constexpr std::string_view rest_prefix = { "/api/crm" };

    using VuecrudController<CrmPersonCommentsController, CrmPersonComment>::VuecrudController;

    Controller::Response index(const Pistache::Rest::Request&);

    static std::optional<std::string> prefix(const std::string &action) {
      // This is just some odd exception to the routing patterns...
      if ((action == "read") || (action == "multiple_update"))
        return "/api/crud/crm/person-comments";
      return VuecrudController<CrmPersonCommentsController, CrmPersonComment>::prefix(action);
    }
  private:
    void model_update(CrmPersonComment &, Controller::PostBody &, std::tm);

    static ControllerRegister<CrmPersonCommentsController> reg;
};
