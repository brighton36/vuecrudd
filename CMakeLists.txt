cmake_minimum_required (VERSION 3.1.3)
project (vuecrudd VERSION 0.1)

##############################################################################
# Dependencies:
include(${CMAKE_ROOT}/Modules/FetchContent.cmake)

# These ensure that our make install doesnt contain any entries from these 
# CMakefiles
SET(PISTACHE_INSTALL OFF CACHE BOOL "Install Pistache")
SET(INSTALL_GTEST OFF CACHE BOOL "Install Gtest")
SET(SPDLOG_INSTALL OFF CACHE BOOL "Install Spdlog")

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
    "--inline-suppr"
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

if(CMAKE_BUILD_TYPE MATCHES "Release")
  # These optimizations cause Address Boundary segfaults in soci::to_base (during
  # Count queries?) on gcc-8. Newer gcc's are not affected. In any case, we disable 
  # them, since Release is built -O3: 
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-tree-forwprop -fno-tree-phiprop \
    -fno-tree-fre -fPIC")
endif()

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs \
    -ftest-coverage -fPIC")
endif()

##############################################################################
# Project Components
include_directories(/usr/include/soci /usr/include/mysql)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty/include)
include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/models)

set(SHARED_OBJ_DEP -lsoci_core -lsoci_sqlite3 -lsqlite3 -lsoci_mysql 
  -lmysqlclient prails pistache_static faker)

# This is kind of a kludge to keep things DRY. We're using the task model and
# controller from the libprails test folder, here in our task module. This allows
# us to include that definition from the libprails test directory:
include_directories(${CMAKE_BINARY_DIR}/_deps/prails-src/tests)

add_definitions(-DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")

add_subdirectory(${PROJECT_SOURCE_DIR}/models)
add_subdirectory(${PROJECT_SOURCE_DIR}/controllers)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib)

#######################################################################
# Test targets
option(BUILD_TESTS "build tests alongside the project" ON)

if (BUILD_TESTS)
  enable_testing()
  add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
endif()

##############################################################################
# The web server executable:
add_executable(${CMAKE_PROJECT_NAME} "${CMAKE_PROJECT_NAME}.cpp")
target_link_libraries(${CMAKE_PROJECT_NAME} ${SHARED_OBJ_DEP})
target_link_libraries(${CMAKE_PROJECT_NAME} 
  "-Wl,--whole-archive" ${CONTROLLERS} ${MODELS} "-Wl,--no-whole-archive")

##############################################################################
# Install and Packaging:
install(TARGETS ${CMAKE_PROJECT_NAME} COMPONENT runtime RUNTIME DESTINATION bin)
install(DIRECTORY DESTINATION /var/vuecrudd)
install(DIRECTORY DESTINATION /var/vuecrudd/api)
install(DIRECTORY DESTINATION /var/vuecrudd/www)

# Spdlog shared lib:
# I guess newer cmake just supports this: install(TARGETS spdlog COMPONENT runtime)
# But we need to target LTS cmake, so, we add these manually
INSTALL(FILES ${PROJECT_BINARY_DIR}/_deps/spdlog-build/libspdlog.so DESTINATION lib)
INSTALL(FILES ${PROJECT_BINARY_DIR}/_deps/spdlog-build/libspdlog.so.1 DESTINATION lib)
INSTALL(FILES ${PROJECT_BINARY_DIR}/_deps/spdlog-build/libspdlog.so.1.6.1 DESTINATION lib)
# TODO: I think we need to dynamically figure out what the pistache lib name is. Or specify it to the pistache build...
#INSTALL(FILES ${PROJECT_BINARY_DIR}/_deps/pistache-build/libpistache-0.0.002-git20200802.so DESTINATION lib)
#INSTALL(FILES ${PROJECT_BINARY_DIR}/_deps/pistache-build/libpistache-0.0.002-git20200802.so.0.0 DESTINATION lib)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "cderose")
INCLUDE(CPack)
