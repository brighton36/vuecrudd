#include "crm_street_prefixes_controller.hpp"

PSYM_CONTROLLER(CrmStreetPrefixesController)

void CrmStreetPrefixesController::Routes(
Pistache::Rest::Router& r, std::shared_ptr<Instance> controller) {
  using namespace Pistache::Rest::Routes;

  RestInstance<CrmStreetPrefixesController, CrmStreetPrefix>::Routes(r,controller);
  // RoutesVuecrudd<CrmStreetPrefixesController>("/api/crud/crm/street-prefixes",r,controller);
}
