cmake_minimum_required (VERSION 3.1.3)
project (vuecrudd VERSION 0.1)

include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

##############################################################################
# Dependencies:
configure_file(CMakeLists.txt.in external/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/external )
if(result)
  message(FATAL_ERROR "CMake step for external dependency failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/external )
if(result)
  message(FATAL_ERROR "Build step for external dependency failed: ${result}")
endif()

# TODO : Can we add_subdirectory... instead of these include/links
set(PRAILS_SRC ${CMAKE_BINARY_DIR}/external/prails/src)
include_directories(
  ${PRAILS_SRC}/prails/include
  ${PRAILS_SRC}/prails/thirdparty/include
  ${PRAILS_SRC}/prails-build/external/pistache-prefix/src/pistache/include
  ${PRAILS_SRC}/prails-build/external/spdlog-prefix/src/spdlog/include)

link_directories(
  ${PRAILS_SRC}/prails-build
  ${PRAILS_SRC}/prails-build/external/pistache-prefix/src/pistache-build/src)

include_directories(/usr/include/soci /usr/include/mysql)

##############################################################################
# Compiler Options
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

##############################################################################
# Project Components
include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/models)
add_subdirectory(${PROJECT_SOURCE_DIR}/controllers)

# TODO: Move this into a function in the project?
set(CONTROLLERS pages_controller auth_controller )

##############################################################################
# The web server executable:
add_executable(${CMAKE_PROJECT_NAME} "${CMAKE_PROJECT_NAME}.cpp")
#add_dependencies(${CMAKE_PROJECT_NAME} prails_external-build)
# TODO: Maybe we should make this more automatic... 
target_link_libraries(${CMAKE_PROJECT_NAME} -lprails -lpistache -lpthread 
  -lsoci_core -lsoci_sqlite3 -lsqlite3 -lsoci_mysql -lmysqlclient)
target_link_libraries(${CMAKE_PROJECT_NAME} 
  "-Wl,--whole-archive" ${CONTROLLERS} "-Wl,--no-whole-archive")
