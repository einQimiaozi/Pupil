#pragma once

#include "function/ui/window_ui.h"

namespace Pupil {
    // �̳� WindowUI����ӿ���
    class EditorUI : public WindowUI {
    public:
        EditorUI();
        virtual void initialize(WindowUIInitInfo init_info) override final;
        // override: ����Ҫ��д���෽��ʱ��ʹ��override�ؼ��֣��������Խ���д�ļ�鹤��������������ȥ��
        // final: ��д��������Ϊֹ�����ܼ�������д
    };
}