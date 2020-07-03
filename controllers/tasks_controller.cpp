#include "tasks_controller.hpp"

const std::string TasksController::route_prefix = "/api/demo/tasks";

ControllerRegister<TasksController> TasksController::reg("tasks");

Task TasksController::modelDefault(std::tm tm_time) {
  return Task({ {"created_at", tm_time}, {"active", (int) 1} });
}

void TasksController::modelUpdate(Task &task, Controller::PostBody &post, std::tm tm_time) {
  task.updated_at(tm_time);

  if (post["name"]) task.name(*post["name"]);
  if (post["description"]) task.description(*post["description"]);
  if (post["active"]) task.active(stoi(*post["active"]));
}
