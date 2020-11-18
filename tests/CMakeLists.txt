function(declare_test test_name)
  set(TEST_EXECUTABLE run_${test_name})
  set(TEST_SOURCE ${test_name}.cpp)

  add_executable(${TEST_EXECUTABLE} ${TEST_SOURCE})
  target_link_libraries(${TEST_EXECUTABLE} gtest gtest_main)
  target_link_libraries(${TEST_EXECUTABLE} ${SHARED_OBJ_DEP})
  target_link_libraries(${TEST_EXECUTABLE} 
    "-Wl,--whole-archive" ${CONTROLLERS} ${MODELS} "-Wl,--no-whole-archive")
  add_test(${test_name} ${TEST_EXECUTABLE})
endfunction()

# Do we need these...
include_directories(${CMAKE_BINARY_DIR}/_deps/pistache-src/tests)

add_definitions(-DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")
add_definitions(-DTESTS_CONFIG_FILE="${PROJECT_SOURCE_DIR}/config/test-server.yml")

declare_test(auth_controller_test)
