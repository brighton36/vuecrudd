// NOTE: This is kinda a cheat, but since I used the vue crud model for the rest 
// tester in the libprails source, might as well reuse that here
#include "user.hpp"

#define TASKS_REST_PREFIX "/api/crud/crm/tasks"
#define TASKS_CLASS_NAME TasksController
#define AUTHORIZER_CLASS_NAME User

#include "rest_controller_test.hpp"
