#pragma once
#include "rest_controller.hpp"
#include "crm_sex.hpp"

class CrmSexesController : 
public Controller::RestInstance<CrmSexesController, CrmSex> { 
  public:
    static constexpr std::string_view rest_prefix = { "/api/crm/sexes" };
    static constexpr std::string_view rest_actions[] = { "index" }; 
    using Controller::RestInstance<CrmSexesController, CrmSex>::RestInstance;
  private:
    static ControllerRegister<CrmSexesController> reg;
};
