#pragma once
#include "rest_controller.hpp"
#include "crm_street_prefix.hpp"

class CrmStreetPrefixesController : 
public Controller::RestInstance<CrmStreetPrefixesController, CrmStreetPrefix> { 
  public:
    static constexpr std::string_view rest_prefix = { "/api/crm/street_prefixes" };
    static constexpr std::string_view rest_actions[] = { "index" }; 
    using Controller::RestInstance<CrmStreetPrefixesController, CrmStreetPrefix>::RestInstance;
  private:
    static ControllerRegister<CrmStreetPrefixesController> reg;
};

