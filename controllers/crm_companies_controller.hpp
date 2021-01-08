#pragma once
#include "rest_controller.hpp"
#include "crm_company.hpp"

class CrmCompanyController : 
public Controller::RestInstance<CrmCompanyController, CrmCompany> { 
  public:
    using Controller::RestInstance<CrmCompanyController, CrmCompany>::RestInstance;

    // TODO: Maybe we should register the index separately, rather than every crud/* action...
    static constexpr std::string_view rest_prefix = { "/api/crm/companies" };
    static constexpr std::string_view rest_actions[]= { "index", "read", 
      "delete", "multiple_delete" };

    Controller::Response index(const Pistache::Rest::Request&);
    static void Routes(Pistache::Rest::Router&, std::shared_ptr<Controller::Instance>);

  private:
    // This is a kind of utility function for vuecrudd. It keeys time concerns DRY
    // across the models
    template <typename T>
    void modelUpdateVuecrudd(T &model, Controller::PostBody &post, std::tm &tm_time) {
      if (post.has_scalar("created_at"))
        throw std::invalid_argument(
          "request attempted to update \"created_at\", which is forbidden");

      if (post.has_scalar("updated_at"))
        throw std::invalid_argument(
          "request attempted to update \"updated_at\", which is forbidden");

      model.updated_at(tm_time);

      // Seems like we're a new record...
      if (!model.id().has_value()) {
        model.created_at(tm_time);
        model.active(true);
      }
    }

    void modelUpdate(CrmCompany &model, Controller::PostBody &post, std::tm tm_time) {
      modelUpdateVuecrudd(model, post, tm_time);
      
      #define COLUMN(a, b, _) REST_COLUMN_UPDATE(a, b)
      CRM_COMPANY_COLUMNS
      #undef COLUMN
    }

    static ControllerRegister<CrmCompanyController> reg;
};
