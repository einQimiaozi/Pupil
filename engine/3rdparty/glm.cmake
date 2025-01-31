file(GLOB glm_sources CONFIGURE_DEPENDS  "${CMAKE_CURRENT_SOURCE_DIR}/glm/*.h")
add_library(glm INTERFACE ${glm_sources})
target_include_directories(glm INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/glm)