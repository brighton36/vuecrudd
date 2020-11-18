# Note: You may wish to manually specify your models here. Per :
# https://stackoverflow.com/questions/1027247/is-it-better-to-specify-source-files-with-glob-or-each-file-individually-in-cmak/1060061#1060061
# What follows, is a glob-based approach.

FILE(GLOB MODEL_SOURCES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/models/*.cpp)

foreach(MODEL_SRC IN LISTS MODEL_SOURCES)
  get_filename_component(MODEL ${MODEL_SRC} NAME_WE)
  message("-- Found Model - " ${MODEL})
  list(APPEND MODELS ${MODEL})
  add_library(${MODEL} STATIC ${MODEL_SRC})
  target_link_libraries(${MODEL} ${SHARED_OBJ_DEP})
endforeach(MODEL_SRC)

set(MODELS ${MODELS} PARENT_SCOPE) 
