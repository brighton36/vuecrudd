#pragma once
#include "vuecrud_controller.hpp"

#include "crm_street_prefix.hpp"

class CrmStreetPrefixesController : 
public VuecrudController<CrmStreetPrefixesController, CrmStreetPrefix> { 
  public:
    static constexpr std::string_view basename = { "street-prefixes" };
    using VuecrudController<CrmStreetPrefixesController, CrmStreetPrefix>::VuecrudController;
  private:
    void modelUpdate(CrmStreetPrefix &, Controller::PostBody &, std::tm);
    static ControllerRegister<CrmStreetPrefixesController> reg;
};

