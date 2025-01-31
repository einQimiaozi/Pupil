set(PRECOMPILE_TOOLS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/precompile/bin")
set(PRECOMPILE_IN_PATH "${CMAKE_CURRENT_SOURCE_DIR}/precompile/precompile.json.in")
set(PRECOMPILE_FILES "${PRECOMPILE_TOOLS_PATH}/precompile.json")
configure_file(${PRECOMPILE_IN_PATH} ${PRECOMPILE_FILES})

# use wine for linux
if (CMAKE_HOST_WIN32)
    set(PRECOMPILE_PRE_EXE)
	  set(PRECOMPILE_GEN ${PUPIL_ROOT_DIR}/build/engine/precompile/relfectionGen/Release/PupilRelfectionGen.exe)
    set(sys_include "*") 
elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux" )
    set(PRECOMPILE_PRE_EXE)
	  set(PRECOMPILE_GEN ${PRECOMPILE_TOOLS_PATH}/PupilRelfectionGen)
    set(sys_include "/usr/include/c++/9/") 
elseif(CMAKE_HOST_APPLE)
    find_program(XCRUN_EXECUTABLE xcrun)
    if(NOT XCRUN_EXECUTABLE)
      message(FATAL_ERROR "xcrun not found!!!")
    endif()

    execute_process(
      COMMAND ${XCRUN_EXECUTABLE} --sdk macosx --show-sdk-platform-path
      OUTPUT_VARIABLE osx_sdk_platform_path_test
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    set(PRECOMPILE_PRE_EXE)
	  set(PRECOMPILE_GEN ${PRECOMPILE_TOOLS_PATH}/PupilRelfectionGen)
    set(sys_include "${osx_sdk_platform_path_test}/../../Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1") 
endif()

set (GEN_INPUT ${CMAKE_BINARY_DIR}/gen_header.h)

### BUILDING ====================================================================================
set(PRECOMPILE_TARGET "PupilPreCompile")
add_custom_target(${PRECOMPILE_TARGET} ALL

COMMAND
  ${CMAKE_COMMAND} -E echo "************************************************************* "
COMMAND
  ${CMAKE_COMMAND} -E echo "**** [Precompile] BEGIN "
COMMAND
  ${CMAKE_COMMAND} -E echo "************************************************************* "

COMMAND
    ${PRECOMPILE_GEN} "${PRECOMPILE_FILES}"  "${GEN_INPUT}"  "${ENGINE_ROOT_DIR}/source" "${sys_include}" "Pupil" 0
### BUILDING ====================================================================================
COMMAND
    ${CMAKE_COMMAND} -E echo "+++ Precompile finished +++"
)