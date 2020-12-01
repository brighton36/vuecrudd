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
    CrmStreetPrefix modelDefault(std::tm tm_time) {
      // TODO: Can we nix?
      return CrmStreetPrefix({ {"created_at", tm_time}, {"active", (int) 1} });
    }

    void modelUpdate(CrmStreetPrefix &task, Controller::PostBody &post, std::tm tm_time) {
      // TODO: Can we nix?
    }

    static ControllerRegister<CrmStreetPrefixesController> reg;
};

