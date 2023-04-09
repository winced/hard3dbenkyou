#include "camera.h"
#include "basic_utility.h"
#include <cmath>
#include <algorithm>

camera::camera(graphics& gfx, const DirectX::XMFLOAT3& pos)
{
    reset();
    camera_pos = pos;
    pos_cbuf.pos = pos;
    pixel_cbuf_p = std::make_unique<pixel_constant_buffer<camera_position_cbuf>>(pos_cbuf, 2u, gfx);
}

DirectX::XMMATRIX camera::get_matrix() const
{
    const DirectX::XMVECTOR forwardBaseVector = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    // apply the camera rotations to a base vector
    const auto lookVector = DirectX::XMVector3Transform(forwardBaseVector,
        DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f)
    );
    // generate camera transform (applied to all objects to arrange them relative
    // to camera position/orientation in world) from cam position and direction
    // camera "top" always faces towards +Y (cannot do a barrel roll)
    const auto camPosition = XMLoadFloat3(&camera_pos);
    const DirectX::XMVECTOR camTarget = DirectX::XMVectorAdd(camPosition, lookVector);
    return DirectX::XMMatrixLookAtLH(camPosition, camTarget, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
}

void camera::show_imgui_control_window()
{
    if (ImGui::Begin("camera control window"))
    {
        ImGui::SliderAngle("pitch", &pitch, 0.995f * -90.0f, 0.995f * 90.0f);
        ImGui::SliderAngle("yaw", &yaw, -180.0f, 180.0f);

        ImGui::SliderFloat("X", &camera_pos.x, -80.0f, 80.0f);
        ImGui::SliderFloat("Y", &camera_pos.y, -80.0f, 80.0f);
        ImGui::SliderFloat("Z", &camera_pos.z, -80.0f, 80.0f);
        if (ImGui::Button("reset"))
            reset();
    }
    ImGui::End();
}

void camera::reset()
{
    camera_pos = { 0.0f,7.5f,-18.0f };
    pos_cbuf.pos = camera_pos;
    pitch = 0.0f;
    yaw = 0.0f;
}

void camera::set(graphics& gfx)
{
    pixel_cbuf_p->update(pos_cbuf, gfx);
    pixel_cbuf_p->set(gfx);
}

void camera::rotate(float dx, float dy) noexcept
{
    yaw = wrap_angle(yaw + dx * rotation_speed);
    pitch = std::clamp(pitch + dy * rotation_speed, 0.995f * -PI / 2.0f, 0.995f * PI / 2.0f);
}

void camera::translate(DirectX::XMFLOAT3 translation) noexcept
{
    DirectX::XMStoreFloat3(&translation, DirectX::XMVector3Transform(
        DirectX::XMLoadFloat3(&translation),
        DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f) *
        DirectX::XMMatrixScaling(travel_speed, travel_speed, travel_speed)));
    camera_pos = { camera_pos.x + translation.x, camera_pos.y + translation.y, camera_pos.z + translation.z };
    pos_cbuf.pos = camera_pos;
}

DirectX::XMFLOAT3 camera::get_pos() const noexcept
{
    return camera_pos;
}
