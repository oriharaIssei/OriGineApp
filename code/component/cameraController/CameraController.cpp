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

void CameraController::Finalize() {
}

void to_json(nlohmann::json& j, const CameraController& c) {
    j["followOffset"]             = c.followOffset_;
    j["rotateSpeedPadStick"]      = c.rotateSpeedPadStick_;
    j["rotateSpeedMouse"]         = c.rotateSpeedMouse_;
    j["rotateSensitivity"]        = c.rotateSensitivity_;
    j["interTargetInterpolation"] = c.interTargetInterpolation_;
    j["maxRotateX"]               = c.maxRotateX_;
    j["minRotateX"]               = c.minRotateX_;
}

void from_json(const nlohmann::json& j, CameraController& c) {
    j.at("followOffset").get_to(c.followOffset_);
    j.at("rotateSpeedPadStick").get_to(c.rotateSpeedPadStick_);
    j.at("rotateSpeedMouse").get_to(c.rotateSpeedMouse_);
    j.at("rotateSensitivity").get_to(c.rotateSensitivity_);
    j.at("interTargetInterpolation").get_to(c.interTargetInterpolation_);
    j.at("maxRotateX").get_to(c.maxRotateX_);
    j.at("minRotateX").get_to(c.minRotateX_);
}
