#pragma once
#include "rest_controller.hpp"
#include "crm_street_prefix.hpp"
#include "vuecrudd_utilities.hpp"

class CrmStreetPrefixesController : 
public Controller::RestInstance<CrmStreetPrefixesController, CrmStreetPrefix> { 
  public:
    static constexpr std::string_view rest_prefix = { "/api/crm/street-prefixes" };
    static constexpr std::string_view rest_actions[] = { "index" }; 
    using Controller::RestInstance<CrmStreetPrefixesController, CrmStreetPrefix>::RestInstance;
    static void Routes(Pistache::Rest::Router&, std::shared_ptr<Controller::Instance>);
  private:
    void modelUpdate(CrmStreetPrefix &model, Controller::PostBody &post, std::tm tm_time) {
      modelUpdateVuecrudd(model, post, tm_time);
 
      #define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
      CRM_STREET_PREFIX_COLUMNS
      #undef COLUMN
    }
    static ControllerRegister<CrmStreetPrefixesController> reg;
};

