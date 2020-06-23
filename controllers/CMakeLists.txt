
add_library(pages_controller pages_controller.cpp)
add_library(auth_controller auth_controller.cpp)
add_library(tasks_controller tasks_controller.cpp)

target_link_libraries(pages_controller -lsoci_core -lsoci_sqlite3 -lsqlite3 
  -lsoci_mysql -lmysqlclient -lpistache -lprails)
target_link_libraries(auth_controller -lsoci_core -lsoci_sqlite3 -lsqlite3 
  -lsoci_mysql -lmysqlclient -lpistache -lprails)
target_link_libraries(tasks_controller -lsoci_core -lsoci_sqlite3 -lsqlite3 
  -lsoci_mysql -lmysqlclient -lpistache -lprails)

