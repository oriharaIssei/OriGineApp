#include "CameraController.h"

#ifdef _DEBUG
/// copmonent
#include "component/player/state/PlayerState.h"
/// gui
#include "myGui/MyGui.h"
#endif // _DEBUG

/// math
#include "math/Sequence.h"
#include <numbers>

using namespace OriGine;

void CameraController::Initialize(Scene* /*_scene*/, EntityHandle /*_owner*/) {
    currentOffset       = firstOffset;
    currentTargetOffset = firstTargetOffset;
}

void CameraController::Edit(Scene* /*_scene*/, EntityHandle /*_OriGine::Entity*/, [[maybe_unused]] const std::string& _parentLabel) {
#ifdef _DEBUG

    if (DragGuiVectorCommand<3, float>("forward##" + _parentLabel, forward, 0.01f, -1.f, 1.f, "%.3f", [this](Vector<3, float>* _v) {
            *_v = OriGine::Vec3f::Normalize(OriGine::Vec3f(_v->v[X], _v->v[Y], _v->v[Z]));
        })) {
        forward = OriGine::Vec3f::Normalize(forward); // 正規化
    }
    DragGuiCommand("Fix For Forward Speed##" + _parentLabel, fixForForwardSpeed, 0.01f, 0.0f);
    DragGuiCommand("angleLimitY##" + _parentLabel, angleLimitY, 0.01f, 0.00001f, std::numbers::pi_v<float> * 2.f, "%.3f");

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

        ImGui::Spacing();

        DragGuiCommand("rotateZ OnWallRun##" + _parentLabel, rotateZOnWallRun, 0.01f);

        ImGui::TreePop();
    }

    ImGui::Spacing();

    DragGuiCommand("rotateSpeedPadStick##" + _parentLabel, rotateSpeedPadStick, 0.01f);
    DragGuiCommand("rotateSpeedMouse##" + _parentLabel, rotateSpeedMouse, 0.01f);

    DragGuiCommand("rotateSensitivity##" + _parentLabel, rotateSensitivity, 0.01f);
    DragGuiCommand("interTargetInterpolation##" + _parentLabel, interTargetInterpolation, 0.01f);

    ImGui::Spacing();

    DragGuiCommand("maxRotateX##" + _parentLabel, maxRotateX, 0.01f);
    DragGuiCommand("minRotateX##" + _parentLabel, minRotateX, 0.01f);

    ImGui::Spacing();

    label = "Fov##" + _parentLabel;
    if (ImGui::TreeNode(label.c_str())) {
        DragGuiCommand("fovYInterpolate##" + _parentLabel, fovYInterpolate, 0.001f);
        DragGuiCommand("baseFovY##" + _parentLabel, baseFovY, 0.01f);
        DragGuiCommand("fovYRateBase##" + _parentLabel, fovYRateBase, 0.001f);
        DragGuiCommand("fovYRateCommonRate##" + _parentLabel, fovYRateCommonRate, 0.001f);

        label = "FovY By Gear Level##" + _parentLabel;
        if (ImGui::BeginTable(label.c_str(), 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
            ImGui::TableSetupColumn("Gear Level");
            ImGui::TableSetupColumn("FovY");
            ImGui::TableHeadersRow();
            for (int level = 1; level <= kMaxPlayerGearLevel; ++level) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%d", level);
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%.2f", CalculateFovYByPlayerGearLevel(level));
            }
            ImGui::EndTable();
        }
        ImGui::TreePop();
    }

#endif // _DEBUG
}

void CameraController::Finalize() {
}

float CameraController::CalculateFovYByPlayerGearLevel(int32_t _level) const {
    return ArithmeticSequence<float>(
        baseFovY,
        ArithmeticSequence<float>(fovYRateBase, fovYRateCommonRate, _level - 1),
        _level);
}

void to_json(nlohmann::json& _j, const CameraController& _c) {
    _j["forward"]                   = _c.forward;
    _j["angleLimitY"]               = _c.angleLimitY;
    _j["firstOffset"]               = _c.firstOffset;
    _j["offsetOnDash"]              = _c.offsetOnDash;
    _j["offsetOnWallRun"]           = _c.offsetOnWallRun;
    _j["firstTargetOffset"]         = _c.firstTargetOffset;
    _j["targetOffsetOnDash"]        = _c.targetOffsetOnDash;
    _j["minTargetOffsetXOnWallRun"] = _c.minTargetOffsetXOnWallRun;
    _j["targetOffsetOnWallRun"]     = _c.targetOffsetOnWallRun;
    _j["rotateZOnWallRun"]          = _c.rotateZOnWallRun;
    _j["rotateSpeedPadStick"]       = _c.rotateSpeedPadStick;
    _j["rotateSpeedMouse"]          = _c.rotateSpeedMouse;
    _j["rotateSensitivity"]         = _c.rotateSensitivity;
    _j["interTargetInterpolation"]  = _c.interTargetInterpolation;
    _j["maxRotateX"]                = _c.maxRotateX;
    _j["minRotateX"]                = _c.minRotateX;
    _j["baseFovY"]                  = _c.baseFovY;
    _j["fovYRate"]                  = _c.fovYRateBase;
    _j["fovYRateCommonRate"]        = _c.fovYRateCommonRate;
    _j["fovYInterpolate"]           = _c.fovYInterpolate;
    _j["fixForForwardSpeed"]        = _c.fixForForwardSpeed;
}

void from_json(const nlohmann::json& _j, CameraController& _c) {
    _j.at("forward").get_to(_c.forward);
    _j.at("angleLimitY").get_to(_c.angleLimitY);

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
    _j.at("maxRotateX").get_to(_c.maxRotateX);
    _j.at("minRotateX").get_to(_c.minRotateX);
    _j.at("baseFovY").get_to(_c.baseFovY);
    _j.at("fovYRate").get_to(_c.fovYRateBase);
    _j.at("fovYRateCommonRate").get_to(_c.fovYRateCommonRate);
    _j.at("fovYInterpolate").get_to(_c.fovYInterpolate);
    _j.at("fixForForwardSpeed").get_to(_c.fixForForwardSpeed);
}
