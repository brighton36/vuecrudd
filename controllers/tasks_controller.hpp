#pragma once
#include "rest_controller.hpp"
#include "task.hpp"

class TasksController : public Controller::RestInstance<TasksController, Task> { 
  public:
    static const std::string route_prefix;

    using Controller::RestInstance<TasksController, Task>::RestInstance;

  private:
    Task modelDefault(std::tm tm_time);
    void modelUpdate(Task &, Controller::PostBody &, std::tm);
    static ServiceRegister<TasksController> reg;
};

