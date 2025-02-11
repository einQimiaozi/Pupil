#include "runtime/function/render/render_pass_base.h"

#include "runtime/core/macro.h"

namespace Pupil {
    void RenderPassBase::postInitialize() {}
    void RenderPassBase::setCommonInfo(RenderPassCommonInfo common_info) {
        rhi             = common_info.rhi;
        render_resource = common_info.render_resource;
    }
    void RenderPassBase::preparePassData(std::shared_ptr<RenderResourceBase> render_resource) {}
    void RenderPassBase::initializeUIRenderBackend(WindowUI* window_ui) {}
}