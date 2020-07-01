# Note: You may wish to manually specify your controllers here. Per :
# https://stackoverflow.com/questions/1027247/is-it-better-to-specify-source-files-with-glob-or-each-file-individually-in-cmak/1060061#1060061
# What follows, is a glob-based approach.

# TODO: Test: file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.cpp")
FILE(GLOB CONTROLLER_SOURCES CONFIGURE_DEPENDS 
	${PROJECT_SOURCE_DIR}/controllers/*.cpp)

foreach(CONTROLLER_SRC IN LISTS CONTROLLER_SOURCES)
	get_filename_component(CONTROLLER ${CONTROLLER_SRC} NAME_WE)
	message("-- Found Controller:" ${CONTROLLER})
	add_library(${CONTROLLER} ${CONTROLLER_SRC})
	# TODO: I think the so's should be in a SHARED_OBJ_DEP variable...
	target_link_libraries(${CONTROLLER} -lsoci_core -lsoci_sqlite3 -lsqlite3 
		-lsoci_mysql -lmysqlclient -lpistache -lprails)
endforeach(CONTROLLER_SRC)

# TODO: put this in the loop above
set(CONTROLLERS pages_controller auth_controller PARENT_SCOPE) 

