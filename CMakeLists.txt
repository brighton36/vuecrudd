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
	${prails_SOURCE_DIR}/include
	${prails_SOURCE_DIR}/thirdparty/include
	${prails_BINARY_DIR}/external/pistache-prefix/src/pistache/include
	${prails_BINARY_DIR}/external/spdlog-prefix/src/spdlog/include)

link_directories(
	${prails_BINARY_DIR}
	${prails_BINARY_DIR}/external/pistache-prefix/src/pistache-build/src)

##############################################################################
if (CMAKE_CXX_CPPCHECK)
	# TODO: can we put a / after the first colon here...
	# TODO: Do we need the unmatche suppression? probably in the prails build. 
	# TODO: Maybe we can remove the deps and external here, once it's in the prails
  list(
    APPEND CMAKE_CXX_CPPCHECK
    "--enable=warning,style,performance,portability,unusedFunction"
    "-DCPPCHECK"
    "-I${PROJECT_SOURCE_DIR}/include"
    "-I${PROJECT_SOURCE_DIR}/controllers"
    "-I${PROJECT_SOURCE_DIR}/models"
		"--suppress=unmatchedSuppression"
    "--suppress=*:*build/*"
    "--suppress=*:*_deps/*"
    "--suppress=*:external/*"
  )
else()
  message("-- Cppcheck not found")
  set(CMAKE_CXX_CPPCHECK "")
endif()


##############################################################################

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
