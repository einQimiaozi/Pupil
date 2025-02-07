# 开发日志

### 2025.02.07

1. feat: 去掉GLM, 手动重构线性代数库。(reason: glm无法支持编译期反射)

2. feat: 支持编译期反射，编译期反射工具reflectionGen独立在runtime之外，由预编译生成，不需要手动使用，已自动接入runtime

3. feat: 使用vulkan作为通用graphics接口，不做单独rhi层，rhi跨平台能力由vulkan支持

4. 日志系统基于spdlog，通过宏定义调用，详见runtime/core/macro.h

5. feat: 渲染系统global资源初始化开发完毕

6. todo: 渲染系统mainLoop

7. todo: 支持unix跨平台编译

8. todo: 睡不着的时候画好架构图

9. todo: 睡不着的时候接入jolt物理系统初始化

10. todo: 渲染管线中的image imageView sampler没有回收，加入回收能力