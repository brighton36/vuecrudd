#include "task.hpp"

ModelRegister<Task> Task::reg("task");

void Task::Migrate() {
  CreateTable({
    {"name", "varchar(100)"},
    {"active", "integer"},
    {"description", "varchar(300)"},
    {"created_at", "datetime"},
    {"updated_at", "datetime"},
  });
};
