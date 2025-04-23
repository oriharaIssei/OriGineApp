#include "CameraController.h"

/// external
#ifdef _DEBUG
#include "imgui/imgui.h"
#endif // _DEBUG

void CameraController::Initialize(GameEntity* /*_entity*/) {
}

bool CameraController::Edit() {
#ifdef _DEBUG
    bool isEdit = false;

    isEdit |= ImGui::DragFloat3("followOffset", followOffset_.v, 0.01f, -100.0f, 100.0f);

    ImGui::Spacing();

    isEdit |= ImGui::DragFloat("rotateSpeedPadStick", &rotateSpeedPadStick_, 0.01f, -100.0f, 100.0f);
    isEdit |= ImGui::DragFloat("rotateSpeedMouse", &rotateSpeedMouse_, 0.01f, -100.0f, 100.0f);

    isEdit |= ImGui::DragFloat("rotateSensitivity", &rotateSensitivity_, 0.01f, -1.f, 1.f);
    isEdit |= ImGui::DragFloat("interTargetInterpolation", &interTargetInterpolation_, 0.01f, -1.f, 1.0f);

    ImGui::Spacing();

    isEdit |= ImGui::DragFloat("maxRotateX", &maxRotateX_, 0.01f, -100.0f, 100.0f);
    isEdit |= ImGui::DragFloat("minRotateX", &minRotateX_, 0.01f, -100.0f, 100.0f);

    return isEdit;
#else
    return false;
#endif // _DEBUG
}

void CameraController::Save(BinaryWriter& _writer) {
    _writer.Write<3, float>("followOffset", followOffset_);
    _writer.Write<float>("rotateSpeedPadStick", rotateSpeedPadStick_);
    _writer.Write<float>("rotateSpeedMouse", rotateSpeedMouse_);
    _writer.Write<float>("rotateSensitivity", rotateSensitivity_);
    _writer.Write<float>("interTargetInterpolation", interTargetInterpolation_);
    _writer.Write<float>("maxRotateX", maxRotateX_);
    _writer.Write<float>("minRotateX", minRotateX_);
}

void CameraController::Load(BinaryReader& _reader) {
    _reader.Read<3, float>("followOffset", followOffset_);
    _reader.Read<float>("rotateSpeedPadStick", rotateSpeedPadStick_);
    _reader.Read<float>("rotateSpeedMouse", rotateSpeedMouse_);
    _reader.Read<float>("rotateSensitivity", rotateSensitivity_);
    _reader.Read<float>("interTargetInterpolation", interTargetInterpolation_);
    _reader.Read<float>("maxRotateX", maxRotateX_);
    _reader.Read<float>("minRotateX", minRotateX_);
}

void CameraController::Finalize() {
}
