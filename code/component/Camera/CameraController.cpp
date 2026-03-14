#include "CameraController.h"

#ifdef _DEBUG
/// gui
#include "myGui/MyGui.h"
#endif // _DEBUG

/// math
#include <numbers>

using namespace OriGine;

void CameraController::Initialize(Scene* /*_scene*/, EntityHandle /*_owner*/) {
    currentOffset       = firstOffset;
    currentTargetOffset = firstTargetOffset;
}

void CameraController::Edit(Scene* /*_scene*/, EntityHandle /*_OriGine::Entity*/, [[maybe_unused]] const std::string& _parentLabel) {
#ifdef _DEBUG

    std::string label = "Offset##" + _parentLabel;
    if (ImGui::TreeNode(label.c_str())) {
        DragGuiVectorCommand("FirstTargetOffset##" + _parentLabel, firstTargetOffset, 0.01f);
        DragGuiVectorCommand("TargetOffsetOnDash##" + _parentLabel, targetOffsetOnDash, 0.01f);
        DragGuiCommand("MinTargetOffsetXOnWallRun##" + _parentLabel, minTargetOffsetXOnWallRun, 0.01f, 0.f, targetOffsetOnWallRun[X]);
        DragGuiVectorCommand("TargetOffsetOnWallRun##" + _parentLabel, targetOffsetOnWallRun, 0.01f);

        ImGui::Spacing();

        DragGuiVectorCommand("firstOffset##" + _parentLabel, firstOffset, 0.01f);
        DragGuiVectorCommand("offsetOnDash##" + _parentLabel, offsetOnDash, 0.01f);
        DragGuiVectorCommand("offsetOnWallRun##" + _parentLabel, offsetOnWallRun, 0.01f);
    }

    ImGui::Spacing();

    DragGuiCommand("rotateZ OnWallRun##" + _parentLabel, rotateZOnWallRun, 0.01f);

    ImGui::Spacing();

    DragGuiCommand("rotateSpeedPadStick##" + _parentLabel, rotateSpeedPadStick, 0.01f);
    DragGuiCommand("rotateSpeedMouse##" + _parentLabel, rotateSpeedMouse, 0.01f);

    DragGuiCommand("rotateSensitivity##" + _parentLabel, rotateSensitivity, 0.01f);

    ImGui::Spacing();

    DragGuiVectorCommand("interTargetInterpolation##" + _parentLabel, interTargetInterpolation, 0.01f);
    DragGuiVectorCommand("interLookAtTargetInterpolation##" + _parentLabel, interLookAtTargetInterpolation, 0.01f);

    ImGui::Spacing();

    DragGuiCommand("maxRotateX##" + _parentLabel, maxRotateX, 0.01f);
    DragGuiCommand("minRotateX##" + _parentLabel, minRotateX, 0.01f);

    ImGui::Spacing();

    label = "Fov##" + _parentLabel;
    if (ImGui::TreeNode(label.c_str())) {
        DragGuiCommand("fovYInterpolate##" + _parentLabel, fovYInterpolate, 0.001f);
        DragGuiCommand("fovMin##" + _parentLabel, fovMin, 0.001f);
        DragGuiCommand("fovMax##" + _parentLabel, fovMax, 0.001f);
        DragGuiCommand("fovMinSpeed##" + _parentLabel, fovMinSpeed, 0.1f);
        DragGuiCommand("fovMaxSpeed##" + _parentLabel, fovMaxSpeed, 0.1f);
        EasingComboGui("fovEaseType##" + _parentLabel, fovEaseType);
        ImGui::TreePop();
    }

#endif // _DEBUG
}

void CameraController::Finalize() {
}

float CameraController::CalculateFovYBySpeed(float _xzSpeed) const {
    if (fovMaxSpeed <= fovMinSpeed) {
        return fovMin;
    }
    float t      = std::clamp((_xzSpeed - fovMinSpeed) / (fovMaxSpeed - fovMinSpeed), 0.f, 1.f);
    float easedT = EasingFunctions[static_cast<int>(fovEaseType)](t);
    return std::lerp(fovMin, fovMax, easedT);
}

void to_json(nlohmann::json& _j, const CameraController& _c) {
    _j["firstOffset"]                    = _c.firstOffset;
    _j["offsetOnDash"]                   = _c.offsetOnDash;
    _j["offsetOnWallRun"]                = _c.offsetOnWallRun;
    _j["firstTargetOffset"]              = _c.firstTargetOffset;
    _j["targetOffsetOnDash"]             = _c.targetOffsetOnDash;
    _j["minTargetOffsetXOnWallRun"]      = _c.minTargetOffsetXOnWallRun;
    _j["targetOffsetOnWallRun"]          = _c.targetOffsetOnWallRun;
    _j["rotateZOnWallRun"]               = _c.rotateZOnWallRun;
    _j["rotateSpeedPadStick"]            = _c.rotateSpeedPadStick;
    _j["rotateSpeedMouse"]               = _c.rotateSpeedMouse;
    _j["rotateSensitivity"]              = _c.rotateSensitivity;
    _j["interTargetInterpolation"]       = _c.interTargetInterpolation;
    _j["interLookAtTargetInterpolation"] = _c.interLookAtTargetInterpolation;
    _j["maxRotateX"]                     = _c.maxRotateX;
    _j["minRotateX"]                     = _c.minRotateX;
    _j["fovYInterpolate"]                = _c.fovYInterpolate;
    _j["fovMin"]                         = _c.fovMin;
    _j["fovMax"]                         = _c.fovMax;
    _j["fovMinSpeed"]                    = _c.fovMinSpeed;
    _j["fovMaxSpeed"]                    = _c.fovMaxSpeed;
    _j["fovEaseType"]                    = static_cast<int>(_c.fovEaseType);
    _j["fixForForwardSpeed"]             = _c.fixForForwardSpeed;
}

void from_json(const nlohmann::json& _j, CameraController& _c) {
    _j.at("firstOffset").get_to(_c.firstOffset);
    _j.at("offsetOnDash").get_to(_c.offsetOnDash);
    _j.at("offsetOnWallRun").get_to(_c.offsetOnWallRun);

    _j.at("firstTargetOffset").get_to(_c.firstTargetOffset);
    _j.at("minTargetOffsetXOnWallRun").get_to(_c.minTargetOffsetXOnWallRun);
    _j.at("targetOffsetOnDash").get_to(_c.targetOffsetOnDash);
    _j.at("targetOffsetOnWallRun").get_to(_c.targetOffsetOnWallRun);
    _j.at("rotateZOnWallRun").get_to(_c.rotateZOnWallRun);

    _j.at("rotateSpeedPadStick").get_to(_c.rotateSpeedPadStick);
    _j.at("rotateSpeedMouse").get_to(_c.rotateSpeedMouse);
    _j.at("rotateSensitivity").get_to(_c.rotateSensitivity);

    _j.at("interTargetInterpolation").get_to(_c.interTargetInterpolation);
    _j.at("interLookAtTargetInterpolation").get_to(_c.interLookAtTargetInterpolation);

    _j.at("maxRotateX").get_to(_c.maxRotateX);
    _j.at("minRotateX").get_to(_c.minRotateX);
    _j.at("fovYInterpolate").get_to(_c.fovYInterpolate);
    _c.fovMin      = _j.value("fovMin", AppConfig::Camera::kDefaultFovMin);
    _c.fovMax      = _j.value("fovMax", AppConfig::Camera::kDefaultFovMax);
    _c.fovMinSpeed = _j.value("fovMinSpeed", AppConfig::Camera::kDefaultFovMinSpeed);
    _c.fovMaxSpeed = _j.value("fovMaxSpeed", AppConfig::Camera::kDefaultFovMaxSpeed);
    _c.fovEaseType = static_cast<OriGine::EaseType>(_j.value("fovEaseType", 0));
    _j.at("fixForForwardSpeed").get_to(_c.fixForForwardSpeed);
}
