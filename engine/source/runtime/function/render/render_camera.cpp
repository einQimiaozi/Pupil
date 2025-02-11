#include "runtime/function/render/render_camera.h"

namespace Pupil {
    void RenderCamera::setCurrentCameraType(RenderCameraType type) {
        std::lock_guard<std::mutex> lock_guard(view_matrix_mutex);
        current_camera_type = type;
    }

    void RenderCamera::setMainViewMatrix(const Matrix4x4& view_matrix, RenderCameraType type) {
        std::lock_guard<std::mutex> lock_guard(view_matrix_mutex);
        current_camera_type = type;
        view_matrices[MAIN_VIEW_MATRIX_INDEX] = view_matrix;

        Vector3 s  = Vector3(view_matrix[0][0], view_matrix[0][1], view_matrix[0][2]);
        Vector3 u  = Vector3(view_matrix[1][0], view_matrix[1][1], view_matrix[1][2]);
        Vector3 f  = Vector3(-view_matrix[2][0], -view_matrix[2][1], -view_matrix[2][2]);
        position = s * (-view_matrix[0][3]) + u * (-view_matrix[1][3]) + f * view_matrix[2][3];
    }

    void RenderCamera::move(Vector3 delta) { position += delta; }

    void RenderCamera::rotate(Vector2 delta) {
        // rotation around x, y axis
        delta = Vector2(Radian(Degree(delta.x)).valueRadians(), Radian(Degree(delta.y)).valueRadians());

        // limit pitch
        float dot = up_axis.dotProduct(forward());
        if ((dot < -0.99f && delta.x > 0.0f) || // angle nearing 180 degrees
            (dot > 0.99f && delta.x < 0.0f))    // angle nearing 0 degrees
            delta.x = 0.0f;

        // pitch is relative to current sideways rotation
        // yaw happens independently
        // this prevents roll
        Quaternion pitch, yaw;
        pitch.fromAngleAxis(Radian(delta.x), X);
        yaw.fromAngleAxis(Radian(delta.y), Z);

        rotation = pitch * rotation * yaw;

        invRotation = rotation.conjugate();
    }

    void RenderCamera::zoom(float offset) {
        // > 0 = zoom in (decrease FOV by <offset> angles)
        fovx = Math::clamp(fovx - offset, MIN_FOV, MAX_FOV);
    }

    void RenderCamera::lookAt(const Vector3& position, const Vector3& target, const Vector3& up) {
        this->position = position;

        // model rotation
        // maps vectors to camera space (x, y, z)
        Vector3 forward = (target - position).normalisedCopy();
        rotation = forward.getRotationTo(Y);

        // correct the up vector
        // the cross product of non-orthogonal vectors is not normalized
        Vector3 right = forward.crossProduct(up.normalisedCopy()).normalisedCopy();
        Vector3 orthUp = right.crossProduct(forward);

        Quaternion upRotation = (rotation * orthUp).getRotationTo(Z);

        rotation = Quaternion(upRotation) * rotation;

        // inverse of the model rotation
        // maps camera space vectors to model vectors
        invRotation = rotation.conjugate();
    }

    Matrix4x4 RenderCamera::getViewMatrix() {
        std::lock_guard<std::mutex> lock_guard(view_matrix_mutex);
        auto view_matrix = Matrix4x4::IDENTITY;
        switch (current_camera_type) {
            case RenderCameraType::Editor:
                view_matrix = Math::makeLookAtMatrix(position, position + forward(), up());
                break;
            case RenderCameraType::Motor:
                view_matrix =view_matrices[MAIN_VIEW_MATRIX_INDEX];
                break;
            default:
                break;
        }
        return view_matrix;
    }

    Matrix4x4 RenderCamera::getPersProjMatrix() const {
        Matrix4x4 fix_mat(1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
        Matrix4x4 proj_mat = fix_mat * Math::makePerspectiveMatrix(Radian(Degree(fovy)), aspect, znear, zfar);

        return proj_mat;
    }

    void RenderCamera::setAspect(float aspect) {
        this->aspect = aspect;
        fovy = Radian(Math::atan(Math::tan(Radian(Degree(fovx) * 0.5f)) / aspect) * 2.0f).valueDegrees();
    }
}