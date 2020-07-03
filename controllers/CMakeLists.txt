# Note: You may wish to manually specify your controllers here. Per :
# https://stackoverflow.com/questions/1027247/is-it-better-to-specify-source-files-with-glob-or-each-file-individually-in-cmak/1060061#1060061
# What follows, is a glob-based approach.

FILE(GLOB CONTROLLER_SOURCES CONFIGURE_DEPENDS 
	${PROJECT_SOURCE_DIR}/controllers/*.cpp)

foreach(CONTROLLER_SRC IN LISTS CONTROLLER_SOURCES)
	get_filename_component(CONTROLLER ${CONTROLLER_SRC} NAME_WE)
	message("-- Found Controller - " ${CONTROLLER})
  list(APPEND CONTROLLERS ${CONTROLLER})
	add_library(${CONTROLLER} ${CONTROLLER_SRC})
	target_link_libraries(${CONTROLLER} ${SHARED_OBJ_DEP})
endforeach(CONTROLLER_SRC)

set(CONTROLLERS ${CONTROLLERS} PARENT_SCOPE) 
