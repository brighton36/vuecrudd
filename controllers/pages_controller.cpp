#include "pages_controller.hpp"

using namespace std;
using namespace Pistache;

ControllerRegister<PagesController> PagesController::reg("pages");

void PagesController::Routes(Rest::Router& r, shared_ptr<Controller::Instance> controller) {
  using namespace Rest;

  Routes::Get(r, "/",      bind("index", &PagesController::index, controller));
  Routes::Get(r, "/sweet", bind("sweet", &PagesController::sweet, controller));
  Routes::Get(r, "/login", bind("login", &PagesController::login, controller));
}

Controller::Response PagesController::index(const Rest::Request&) {
  return render_html("default", "index");
}

Controller::Response PagesController::login(const Rest::Request&) {
  return render_html("default", "login");
}

Controller::Response PagesController::sweet(const Rest::Request&) {
  return Controller::Response(200, "text/html", "Sweet: " + controller_name);
}

