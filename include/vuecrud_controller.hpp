#pragma once
#include "rest_controller.hpp"
#include "vuecrud_utilities.hpp"

template <class U, class T>
class VuecrudController : public Controller::RestInstance<U,T> {
  public:
    static constexpr std::string_view rest_actions[]= { "index", "read" };
    static constexpr std::string_view vuecrud_basename = { "" };

    using Controller::RestInstance<U, T>::RestInstance;

    static void Routes(Pistache::Rest::Router &r, std::shared_ptr<Controller::Instance> controller) {
      using namespace Pistache::Rest::Routes;
      using namespace Controller;

      RestInstance<U,T>::Routes(r,controller);
      std::string prefix = "/api/crud/crm/"+basename();

      Put(r, prefix+"/:id", Instance::bind("update", &U::create_or_update, controller));
      Post(r, prefix, Instance::bind("create", &U::create_or_update, controller));
      Post(r, prefix+"/multiple-update", 
        Instance::bind("multiple_update", &U::multiple_update, controller));

      if ( !GetConfig().cors_allow().empty() ) {
        Options(r, prefix+"/*", 
          Instance::bind("api_options_actions", &U::options, controller));
        Options(r, prefix, 
          Instance::bind("api_options_index", &U::options, controller));
      }
    }

    static std::string basename() { 
      return {U::basename.data(), U::basename.size()};
    }

    static std::string prefix() { 
      return "/api/crm/"+basename();
    }
  protected:
    // This is a kind of utility function for vuecrud. It keeys time concerns DRY
    // across the models
    void modelUpdateVuecrud(T &model, Controller::PostBody &post, std::tm &tm_time) {
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
};
