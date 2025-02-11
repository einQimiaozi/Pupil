#pragma once

#include <mutex>

#include "runtime/core/math/math_headers.h"

namespace Pupil {
    // 相机模式支持两类, 编辑时有一个相机, 预览时是另一个相机
    enum class RenderCameraType : int {
        Editor,
        Motor
    };

    class RenderCamera {
    public:
        RenderCameraType current_camera_type {RenderCameraType::Editor};
        
        // default camera setting
        static const Vector3 X, Y, Z;

        Vector3    position {0.0f, 0.0f, 0.0f};
        Quaternion rotation {Quaternion::IDENTITY};
        Quaternion invRotation {Quaternion::IDENTITY};
        float      znear {1000.0f};
        float      zfar {0.1f};
        float      aspect {0.f};
        float      fovx {Degree(89.f).valueDegrees()};
        float      fovy {0.f};
        Vector3    up_axis {Z};

        static constexpr float MIN_FOV {10.0f};
        static constexpr float MAX_FOV {89.0f};
        static constexpr int   MAIN_VIEW_MATRIX_INDEX {0};

        std::vector<Matrix4x4> view_matrices {Matrix4x4::IDENTITY};

        // camera controller
        void setCurrentCameraType(RenderCameraType type);
        void setMainViewMatrix(const Matrix4x4& view_matrix, RenderCameraType type = RenderCameraType::Editor);

        void move(Vector3 delta);
        void rotate(Vector2 delta);
        void zoom(float offset);
        void lookAt(const Vector3& position, const Vector3& target, const Vector3& up);

        Vector3   forward() const { return (invRotation * Y); }
        Vector3   up() const { return (invRotation * Z); }
        Vector3   right() const { return (invRotation * X); }
        Vector2   getFOV() const { return {fovx, fovy}; }
        Matrix4x4 getViewMatrix();
        Matrix4x4 getPersProjMatrix() const;
        Matrix4x4 getLookAtMatrix() const { return Math::makeLookAtMatrix(position, position + forward(), up()); }
        float     getFovYDeprecated() const { return fovy; }
        void      setAspect(float aspect);

    protected:
        std::mutex view_matrix_mutex;
    };

    inline const Vector3 RenderCamera::X = {1.0f, 0.0f, 0.0f};
    inline const Vector3 RenderCamera::Y = {0.0f, 1.0f, 0.0f};
    inline const Vector3 RenderCamera::Z = {0.0f, 0.0f, 1.0f};
}