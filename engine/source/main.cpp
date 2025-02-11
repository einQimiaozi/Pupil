#include <filesystem>

#include "editor/editor.h"
#include "engine.h"

// static const char *CONFIG_FILE_PATH = "../development/conf.ini";
static const char *CONFIG_FILE_PATH = "D:/Pupil/bin/development/conf.ini";       // debug test case

int main(int argc, char** argv) {
    // 引擎创建
    Pupil::PupilEngine* engine = new Pupil::PupilEngine();
    engine->initialize(CONFIG_FILE_PATH); // 通过config文件初始化引擎

    // 编辑器创建
    Pupil::PupilEditor* editor = new Pupil::PupilEditor();
    editor->initialize(engine);

    // 开始运行编辑器及其引擎
    editor->run();

    // 关机的收尾工作
    editor->shutdown();
    engine->shutdown();

    return 0;
}