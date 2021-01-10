#pragma once
#include "vuecrud_controller.hpp"

#include "crm_street_prefix.hpp"

class CrmStreetPrefixesController : 
public VuecrudController<CrmStreetPrefixesController, CrmStreetPrefix> { 
  public:
    static constexpr std::string_view basename = { "street-prefixes" };
    using VuecrudController<CrmStreetPrefixesController, CrmStreetPrefix>::VuecrudController;
  private:
    void modelUpdate(CrmStreetPrefix &model, Controller::PostBody &post, std::tm tm_time) {
      modelUpdateVuecrud(model, post, tm_time);
      #define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
      CRM_STREET_PREFIX_COLUMNS
      #undef COLUMN
    }
    static ControllerRegister<CrmStreetPrefixesController> reg;
};

