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
    void modelUpdate(CrmLanguage &model, Controller::PostBody &post, std::tm tm_time) {
      modelUpdateVuecrud(model, post, tm_time);
      #define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
      CRM_LANGUAGE_COLUMNS
      #undef COLUMN
    }

    static ControllerRegister<CrmLanguagesController> reg;
};
