#include "runtime/function/render/render_scene.h"
#include "runtime/function/render/render_resource.h"
#include "runtime/function/render/render_pass.h"

namespace Pupil {
    void RenderScene::clear() {}

    void RenderScene::setVisibleNodesReference() {
        RenderPass::visiable_nodes.p_directional_light_visible_mesh_nodes = &directional_light_visible_mesh_nodes;
        RenderPass::visiable_nodes.p_point_lights_visible_mesh_nodes      = &point_lights_visible_mesh_nodes;
        RenderPass::visiable_nodes.p_main_camera_visible_mesh_nodes       = &main_camera_visible_mesh_nodes;
        RenderPass::visiable_nodes.p_axis_node                            = &axis_node;
    }
}