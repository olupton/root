############################################################################
# CMakeLists.txt file for building ROOT core/foundation package
############################################################################

ROOT_GLOB_SOURCES(sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx)

ROOT_GLOB_HEADERS(headers ${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h)
list(REMOVE_ITEM headers ${CMAKE_CURRENT_SOURCE_DIR}/inc/libcpp_string_view.h)
list(REMOVE_ITEM headers ${CMAKE_CURRENT_SOURCE_DIR}/inc/RWrap_libcpp_string_view.h)

set(Foundation_dict_headers ${headers} PARENT_SCOPE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/res)

ROOT_OBJECT_LIBRARY(Foundation ${sources})

ROOT_OBJECT_LIBRARY(Foundation_Stage1 ${sources}) # used by rootcling_stage1
set_target_properties(Foundation_Stage1 PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS} ${CLING_CXXFLAGS}")

ROOT_INSTALL_HEADERS()
