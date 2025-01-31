cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

project(PupilEngine CXX)

set(TARGET_NAME Pupil)
set(ENGINE_ROOT_DIR "${PUPIL_ROOT_DIR}/engine")
set(EDITOR_ROOT_DIR "${PUPIL_ROOT_DIR}/editor")
set(THIRD_PARTY_DIR "${ENGINE_ROOT_DIR}/3rdparty")
set(ENGINE_ASSET_DIR "/asset")

add_definitions(-DPUPIL_ENGINE_ROOT_DIR="${ENGINE_ROOT_DIR}")

set(vulkan_include ${THIRD_PARTY_DIR}/VulkanSDK/include)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options("/MT")
endif()

if(WIN32)
    set(vulkan_lib ${THIRD_PARTY_DIR}/VulkanSDK/lib/Win32/vulkan-1.lib)
    set(glslangValidator_executable ${THIRD_PARTY_DIR}/VulkanSDK/bin/Win32/glslangValidator.exe)
    add_compile_definitions("PUPIL_VK_LAYER_PATH=${THIRD_PARTY_DIR}/VulkanSDK/bin/Win32")
    add_definitions(-DWin32)
    message("vulkan platform: Win32")
elseif(UNIX)
    if(APPLE)
      set(vulkan_lib ${THIRD_PARTY_DIR}/VulkanSDK/lib/MacOS/libvulkan.1.dylib)
      set(glslangValidator_executable ${THIRD_PARTY_DIR}/VulkanSDK/bin/MacOS/glslangValidator)
      add_compile_definitions("PUPIL_VK_LAYER_PATH=${THIRD_PARTY_DIR}/VulkanSDK/bin/MacOS")
      add_compile_definitions("PUPIL_VK_ICD_FILENAMES=${THIRD_PARTY_DIR}/VulkanSDK/bin/MacOS/MoltenVK_icd.json")
      add_definitions(-DAPPLE)
      message("vulkan platform: MacOS")
    else()
      set(vulkan_lib ${THIRD_PARTY_DIR}/VulkanSDK/lib/Linux/libvulkan.so.1)
      set(glslangValidator_executable ${THIRD_PARTY_DIR}/VulkanSDK/bin/Linux/glslangValidator)
      add_compile_definitions("PUPIL_VK_LAYER_PATH=${THIRD_PARTY_DIR}/VulkanSDK/bin/Linux")
      add_definitions(-DUNIX)
      message("vulkan platform: Unix/Linux --Unknown")
    endif()
else()
  message(FATAL_ERROR "Unknown Platform")
endif()

file(GLOB EDITOR_HEADERS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
file(GLOB EDITOR_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

add_subdirectory(3rdparty)
add_subdirectory(source/runtime)
add_subdirectory(source/editor)
add_subdirectory(precompile/relfectionGen)

set(CODEGEN_TARGET "PupilPreCompile")
include(precompile/precompile.cmake)
set_target_properties("${CODEGEN_TARGET}" PROPERTIES FOLDER "Engine")

add_dependencies(PupilRuntime "${CODEGEN_TARGET}")
add_dependencies("${CODEGEN_TARGET}" "PupilRelfectionGen")

add_executable(${TARGET_NAME} ${EDITOR_HEADERS} ${EDITOR_SOURCES})

target_link_libraries(${TARGET_NAME} PupilEditor)