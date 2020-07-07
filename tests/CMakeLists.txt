function(declare_test test_name)
  set(TEST_EXECUTABLE run_${test_name})
  set(TEST_SOURCE ${test_name}.cpp)

  add_executable(${TEST_EXECUTABLE} ${TEST_SOURCE})
  target_link_libraries(${TEST_EXECUTABLE} ${SHARED_OBJ_DEP})
  target_link_libraries(${TEST_EXECUTABLE} gtest gtest_main)
  target_link_libraries(${TEST_EXECUTABLE} 
    "-Wl,--whole-archive" ${CONTROLLERS} ${MODELS} "-Wl,--no-whole-archive")
  add_test(${test_name} ${TEST_EXECUTABLE})
endfunction()

include_directories(${CMAKE_BINARY_DIR}/_deps/pistache-src/tests)
include_directories(${CMAKE_BINARY_DIR}/_deps/pistache-src/third-party/rapidjson/include)

add_definitions(-DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")

declare_test(tasks_controller)

# TODO: we should glob this and set it throughout the project
target_precompile_headers(run_tasks_controller PRIVATE 
  ${CMAKE_BINARY_DIR}/_deps/prails-src/include/model.hpp)
target_precompile_headers(run_tasks_controller PRIVATE 
  ${CMAKE_BINARY_DIR}/_deps/pistache-src/tests/httplib.h)
target_precompile_headers(run_tasks_controller PRIVATE 
  ${PROJECT_SOURCE_DIR}/models/task.hpp)
target_precompile_headers(run_tasks_controller PRIVATE 
  ${CMAKE_BINARY_DIR}/_deps/prails-src/include/server.hpp)
target_precompile_headers(run_tasks_controller PRIVATE 
  ${CMAKE_BINARY_DIR}/_deps/googletest-src/googletest/include/gtest/gtest.h)

