cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

project(Pupil CXX)

################################################################################
# Set target arch type if empty. Visual studio solution generator provides it.
################################################################################
if(NOT CMAKE_VS_PLATFORM_NAME)
    set(CMAKE_VS_PLATFORM_NAME "x64")
endif()
message("${CMAKE_VS_PLATFORM_NAME} architecture in use")

if(NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64"
     OR "${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86"))
    message(FATAL_ERROR "${CMAKE_VS_PLATFORM_NAME} arch is not supported!")
endif()

################################################################################
# Global configuration types
################################################################################
set(CMAKE_CONFIGURATION_TYPES
    "Debug"
    "Release"
    CACHE STRING "" FORCE
)

################################################################################
# Global compiler options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_CXX_FLAGS "")
    set(CMAKE_CXX_FLAGS_DEBUG "")
    set(CMAKE_CXX_FLAGS_RELEASE "")
endif()

################################################################################
# Global linker options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_EXE_LINKER_FLAGS "")
    set(CMAKE_MODULE_LINKER_FLAGS "")
    set(CMAKE_SHARED_LINKER_FLAGS "")
    set(CMAKE_STATIC_LINKER_FLAGS "")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS}")
endif()

################################################################################
# Nuget packages function stub.
################################################################################
function(use_package TARGET PACKAGE VERSION)
    message(WARNING "No implementation of use_package. Create yours. "
                    "Package \"${PACKAGE}\" with version \"${VERSION}\" "
                    "for target \"${TARGET}\" is ignored!")
endfunction()

################################################################################
# Common utils
################################################################################
include(CMake/Utils.cmake)

################################################################################
# Additional Global Settings(add specific info there)
################################################################################
include(CMake/GlobalSettingsInclude.cmake OPTIONAL)

################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
# Sub-projects
################################################################################



























set(PROJECT_NAME Pupil)

################################################################################
# Source groups
################################################################################
set(
    HEADERS
    "engine/source/editor/editor.h"
    "engine/source/editor/editor_ui.h"
    "engine/source/runtime/core/log/log.h"
    "engine/source/runtime/core/macro.h"
    "engine/source/runtime/core/primitive/primitive.h"
    "engine/source/runtime/engine.h"
    "engine/source/runtime/function/global/global_context.h"
    "engine/source/runtime/function/render/render_system.h"
    "engine/source/runtime/function/render/window_system.h"
    "engine/source/runtime/function/ui/window_ui.h"
    "engine/source/runtime/platform/rhi/rhi.h"
    "engine/source/runtime/platform/rhi/rhi_struct.h"
    "engine/source/runtime/platform/rhi/vulkan/vulkan_rhi.h"
    "engine/source/runtime/platform/rhi/vulkan/vulkan_struct.h"
    ...
)
source_group("HEADERS_GROUP" FILES ${HEADERS})

set(
    SOURCES
    "engine/source/editor/editor.cpp"
    "engine/source/editor/editor_ui.cpp"
    "engine/source/main.cpp"
    "engine/source/runtime/core/log/log.cpp"
    "engine/source/runtime/engine.cpp"
    "engine/source/runtime/function/global/global_context.cpp"
    "engine/source/runtime/function/render/render_system.cpp"
    "engine/source/runtime/function/render/window_system.cpp"
    "engine/source/runtime/function/ui/window_ui.cpp"
    "engine/source/runtime/platform/rhi/vulkan/vulkan_rhi.cpp"
    ...
)
source_group("SOURCES_GROUP" FILES ${SOURCES})

set(ALL_FILES
    ${HEADERS}
    ${SOURCES}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} "${ALL_FILES}")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Pupil)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()
################################################################################
# Include directories
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/engine/source/runtime;"
        "${CMAKE_CURRENT_SOURCE_DIR}/engine/source/editor;"
        "${CMAKE_CURRENT_SOURCE_DIR}/engine/3rdparty/tinyobjloader;"
        "${CMAKE_CURRENT_SOURCE_DIR}/engine/3rdparty/stb;"
        "${CMAKE_CURRENT_SOURCE_DIR}/engine/3rdparty/JoltPhysics/Jolt;"
        "${CMAKE_CURRENT_SOURCE_DIR}/engine/3rdparty/glm;"
        "${CMAKE_CURRENT_SOURCE_DIR}/engine/3rdparty/VulkanSDK/Include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/engine/3rdparty/spdlog/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/engine/3rdparty/glfw/include"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/engine/source/runtime;"
        "${CMAKE_CURRENT_SOURCE_DIR}/engine/source/editor;"
        "${CMAKE_CURRENT_SOURCE_DIR}/engine/3rdparty/tinyobjloader;"
        "${CMAKE_CURRENT_SOURCE_DIR}/engine/3rdparty/stb;"
        "${CMAKE_CURRENT_SOURCE_DIR}/engine/3rdparty/JoltPhysics/Jolt;"
        "${CMAKE_CURRENT_SOURCE_DIR}/engine/3rdparty/glm;"
        "${CMAKE_CURRENT_SOURCE_DIR}/engine/3rdparty/VulkanSDK/Include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/engine/3rdparty/spdlog/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/engine/3rdparty/glfw/include"
    )
endif()

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /std:c++17;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            /utf-8;
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /std:c++17;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            /utf-8;
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    endif()
endif()

################################################################################
# Dependencies
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "glfw3;"
        "vulkan-1;"
        "Jolt;"
        "spdlog"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "glfw3;"
        "vulkan-1;"
        "Jolt;"
        "spdlog"
    )
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_link_directories(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "D:/Pupil/engine/3rdparty/JoltPhysics/Build/VS2022_CL/Debug"
        ">"
        "$<$<CONFIG:Release>:"
            "D:/Pupil/engine/3rdparty/JoltPhysics/Build/VS2022_CL/Release"
        ">"
        "D:/Pupil/engine/3rdparty/glfw/lib-vc2022;"
        "D:/Pupil/engine/3rdparty/VulkanSDK/lib/Win32;"
        "D:/Pupil/engine/3rdparty/spdlog/build/Release"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_link_directories(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "D:/Pupil/engine/3rdparty/JoltPhysics/Build/VS2022_CL/Debug"
        ">"
        "$<$<CONFIG:Release>:"
            "D:/Pupil/engine/3rdparty/JoltPhysics/Build/VS2022_CL/Release"
        ">"
        "D:/Pupil/engine/3rdparty/glfw/lib-vc2022;"
        "D:/Pupil/engine/3rdparty/VulkanSDK/lib/Win32;"
        "D:/Pupil/engine/3rdparty/spdlog/build/Release"
    )
endif()

