// NOTE: This is kinda a cheat, but since I used the vue crud model for the rest tester
// in the libprails source, might as well reuse that here
#include "rest_controller_test.hpp"

const std::string TasksController::route_prefix = "/api/demo/tasks";

REGISTER_CONTROLLER(TasksController)
