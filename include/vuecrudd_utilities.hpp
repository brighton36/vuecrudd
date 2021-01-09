#pragma once
#include "rest_controller.hpp"

// TODO: Do I need each of these...
#include <pistache/router.h>
#include <controller.hpp>

using namespace prails::utilities;
using namespace Controller;

// This is a kind of utility function for vuecrudd. It keeys time concerns DRY
// across the models
template <typename T>
void modelUpdateVuecrudd(T &model, PostBody &post, std::tm &tm_time) {
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

// For some reason, these update actions dont use the same prefix...
template<typename T>
void RoutesVuecrudd(std::string rest_prefix, 
Pistache::Rest::Router& r, std::shared_ptr<Instance> controller) {
  using namespace Pistache::Rest::Routes;

  Put(r, rest_prefix+"/:id", 
    Controller::Instance::bind("update", &T::create_or_update, controller));
  Post(r, rest_prefix, 
    Controller::Instance::bind("create", &T::create_or_update, controller));
  Post(r, rest_prefix+"/multiple-update", 
    Controller::Instance::bind("multiple_update", &T::multiple_update, controller));

  if ( !GetConfig().cors_allow().empty() ) {
    Options(r, rest_prefix+"/*", 
      Controller::Instance::bind("vuecrud_options_actions", &T::options, controller));
    Options(r, rest_prefix, 
      Controller::Instance::bind("vuecrud_options_index", &T::options, controller));
  }
}
