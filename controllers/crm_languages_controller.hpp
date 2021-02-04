#pragma once
#include "vuecrud_controller.hpp"

#include "crm_language.hpp"

class CrmLanguagesController : 
public VuecrudController<CrmLanguagesController, CrmLanguage> { 
  public:
    static constexpr std::string_view basename = { "languages" };
    static constexpr std::string_view read_prefix = { "/api/crud/crm" };

    using VuecrudController<CrmLanguagesController, CrmLanguage>::VuecrudController;
  private:
    void model_update(CrmLanguage &, Controller::PostBody &, std::tm);

    static ControllerRegister<CrmLanguagesController> reg;
};
