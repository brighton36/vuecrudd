cmake_minimum_required (VERSION 3.1.3)
project (vuecrudd VERSION 0.1)

##############################################################################
# Dependencies:
include(${CMAKE_ROOT}/Modules/FetchContent.cmake)

FetchContent_Declare(
  prails
	GIT_REPOSITORY "https://github.com/brighton36/prails.git"
)
FetchContent_MakeAvailable(prails)
FetchContent_GetProperties(prails)
if(NOT prails_POPULATED)
  FetchContent_Populate(prails)
  add_subdirectory(${prails_SOURCE_DIR} ${prails_BINARY_DIR})
endif()

include_directories(
	${CMAKE_BINARY_DIR}/_deps/prails-src/include
	${CMAKE_BINARY_DIR}/_deps/prails-src/thirdparty/include
	${CMAKE_BINARY_DIR}/_deps/pistache-src/include
	${CMAKE_BINARY_DIR}/_deps/spdlog-src/include)

link_directories(
	${CMAKE_BINARY_DIR}/_deps/prails-build
	${CMAKE_BINARY_DIR}/_deps/pistache-build/src)

##############################################################################
# Lint detection:
if (CMAKE_CXX_CPPCHECK)
  list(
    APPEND CMAKE_CXX_CPPCHECK
    "--enable=warning,style,performance,portability,unusedFunction"
    "-DCPPCHECK"
    "-I${PROJECT_SOURCE_DIR}/include"
    "-I${PROJECT_SOURCE_DIR}/controllers"
    "-I${PROJECT_SOURCE_DIR}/models"
    "--suppress=*:_deps/*"
  )
else()
  message("-- Cppcheck not found")
  set(CMAKE_CXX_CPPCHECK "")
endif()

##############################################################################
# Compiler Options
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

##############################################################################
# Project Components
include_directories(/usr/include/soci /usr/include/mysql)
include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/models)
add_subdirectory(${PROJECT_SOURCE_DIR}/controllers)

# TODO: Move this into a function in the project?
set(CONTROLLERS pages_controller auth_controller )

##############################################################################
# The web server executable:
add_executable(${CMAKE_PROJECT_NAME} "${CMAKE_PROJECT_NAME}.cpp")
target_link_libraries(${CMAKE_PROJECT_NAME} -lprails -lpistache -lpthread 
  -lsoci_core -lsoci_sqlite3 -lsqlite3 -lsoci_mysql -lmysqlclient)
target_link_libraries(${CMAKE_PROJECT_NAME} 
  "-Wl,--whole-archive" ${CONTROLLERS} "-Wl,--no-whole-archive")
