#pragma once
#include "rest_controller.hpp"
#include "vuecrud_utilities.hpp"


template <class U, class T>
class VuecrudController : public Controller::RestInstance<U,T> {
  public:
    static constexpr std::string_view read_prefix = { "/api/crm" };
    static constexpr std::string_view rest_prefix = { "/api/crud/crm" };
    static constexpr std::string_view rest_actions[]= { "index", "read", 
      "create", "update", "multiple_update" };
    static constexpr std::string_view read_actions[]= {  };

    using Controller::RestInstance<U, T>::RestInstance;

    static std::string basename() { 
      return {U::basename.data(), U::basename.size()};
    }

    static std::optional<std::string> prefix(const std::string &action) {
      return ( ((action == "read") || (action == "index")) ? 
        std::string({U::read_prefix.data(), U::read_prefix.size()}) :
        std::string({U::rest_prefix.data(), U::rest_prefix.size()})
        )+"/"+basename();
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
