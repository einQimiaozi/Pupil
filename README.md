# 编译

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

生成好的项目可执行文件路径

```
./Pupil/build/engine/Release/Pupil.exe
```


**下述编译方案为旧方案，已不再使用，但自行配置cmake_converter和visual studio后也可以使用**

# 编译

如果不修改代码的话

1. 在visual studio中重新生成解决方法

2. 生成makefile

```bash
mkdir build
cmake -S "" -B "build" -G "Visual Studio 17 2022"
```

如果你用的不是vs2022，替换为你的ide

3. 编译

```bash
cmake --build "build" --config Release
```

暂时不支持Debug版本，jolt对Debug版本支持很差

如果修改代码的话

1. 安装python，版本 >= 3.6

2. 在visual studio中重新生成解决方法

3. 重新生成CMakeLists.exe

```bash
cd cmake_converter
python main.py -s ${你的项目绝对地址}/Pupil.sln
```

4. 修改CMakeLists.txt

修改的部分1:

```cmake
...

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
source_group("HEADERS_GROUP" FILES ${HEADERS})      # 修改这里，把source_group改成这行代码

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
source_group("SOURCES_GROUP" FILES ${SOURCES})  # 修改这里，把source_group改成这行代码

# 修改这里，把set(ALL_FILES改成这行代码
set(ALL_FILES
    ${HEADERS}
    ${SOURCES}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} "${ALL_FILES}")      # 修改这里，把add_executable修改成这行代码

...
```

5. 生成makefile

```bash
cd ..
mkdir build
cmake -S . -B "build" -G "Visual Studio 17 2022"
```

如果你用的不是vs2022，替换为你的ide

6. 编译

```bash
cmake --build "build" --config Release
```