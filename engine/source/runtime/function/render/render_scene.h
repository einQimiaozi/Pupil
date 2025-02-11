#pragma once

#include <optional>
#include <vector>

#include "runtime/function/render/light.h"
#include "runtime/function/render/render_common.h"
#include "runtime/function/render/render_entity.h"

namespace Pupil {
    class RenderResource;
    class RenderCamera;

    class RenderScene {
    public:
        // light
        AmbientLight       ambient_light;
        PDirectionalLight  directional_light;
        PointLightList     point_light_list;

        // render entities
        std::vector<RenderEntity> render_entities;

        // axis, for editor
        std::optional<RenderEntity> render_axis;

        // visible objects (updated per frame)
        std::vector<RenderMeshNode> directional_light_visible_mesh_nodes;
        std::vector<RenderMeshNode> point_lights_visible_mesh_nodes;
        std::vector<RenderMeshNode> main_camera_visible_mesh_nodes;
        RenderAxisNode              axis_node;

        void clear();

        // set visible nodes ptr in render pass
        void setVisibleNodesReference();

        // update visible objects in each frame
        // todo: updateVisibleObjects
    };
}