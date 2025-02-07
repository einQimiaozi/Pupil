set(TARGET_NAME PupilEditor) # 增加构建目标

file(GLOB EDITOR_HEADERS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB EDITOR_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(${TARGET_NAME} STATIC ${EDITOR_HEADERS} ${EDITOR_SOURCES}) # 给建构目标添加文件

target_compile_options(${TARGET_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/WX->")

target_link_libraries(${TARGET_NAME} PupilRuntime) # Runtime已经作为静态库被构建了，所以要链接过来

set(COMPILE_MODE deployment)
if("$<CONFIG:Debug>")
    set(COMPILE_MODE development)
endif()

set(POST_BUILD_COMMANDS
  COMMAND ${CMAKE_COMMAND} -E copy "${RUNTIME_ROOT_DIR}/resource/config_manager/${COMPILE_MODE}/conf.ini" "${OUTPUT_DIR}/${COMPILE_MODE}/conf.ini"
)

add_custom_command(TARGET ${TARGET_NAME} ${POST_BUILD_COMMANDS})

set(PUPIL_EDITOR_HEADS "${EDITOR_HEADERS}" CACHE STRING "" FORCE)