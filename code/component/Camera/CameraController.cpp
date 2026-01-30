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

void to_json(nlohmann::json& j, const CameraController& c) {
    j["forward"]                  = c.forward;
    j["angleLimitY"]              = c.angleLimitY;
    j["firstOffset"]              = c.firstOffset;
    j["offsetOnDash"]             = c.offsetOnDash;
    j["offsetOnWallRun"]          = c.offsetOnWallRun;
    j["firstTargetOffset"]        = c.firstTargetOffset;
    j["targetOffsetOnDash"]       = c.targetOffsetOnDash;
    j["targetOffsetOnWallRun"]    = c.targetOffsetOnWallRun;
    j["rotateZOnWallRun"]         = c.rotateZOnWallRun;
    j["rotateSpeedPadStick"]      = c.rotateSpeedPadStick;
    j["rotateSpeedMouse"]         = c.rotateSpeedMouse;
    j["rotateSensitivity"]        = c.rotateSensitivity;
    j["interTargetInterpolation"] = c.interTargetInterpolation;
    j["maxRotateX"]               = c.maxRotateX;
    j["minRotateX"]               = c.minRotateX;
    j["baseFovY"]                 = c.baseFovY;
    j["fovYRate"]                 = c.fovYRateBase;
    j["fovYRateCommonRate"]       = c.fovYRateCommonRate;
    j["fovYInterpolate"]          = c.fovYInterpolate;
    j["fixForForwardSpeed"]       = c.fixForForwardSpeed;
}

void from_json(const nlohmann::json& j, CameraController& c) {
    j.at("forward").get_to(c.forward);
    j.at("angleLimitY").get_to(c.angleLimitY);

    j.at("firstOffset").get_to(c.firstOffset);
    j.at("offsetOnDash").get_to(c.offsetOnDash);
    j.at("offsetOnWallRun").get_to(c.offsetOnWallRun);

    j.at("firstTargetOffset").get_to(c.firstTargetOffset);
    j.at("targetOffsetOnDash").get_to(c.targetOffsetOnDash);
    j.at("targetOffsetOnWallRun").get_to(c.targetOffsetOnWallRun);
    j.at("rotateZOnWallRun").get_to(c.rotateZOnWallRun);

    j.at("rotateSpeedPadStick").get_to(c.rotateSpeedPadStick);
    j.at("rotateSpeedMouse").get_to(c.rotateSpeedMouse);
    j.at("rotateSensitivity").get_to(c.rotateSensitivity);
    j.at("interTargetInterpolation").get_to(c.interTargetInterpolation);
    j.at("maxRotateX").get_to(c.maxRotateX);
    j.at("minRotateX").get_to(c.minRotateX);
    j.at("baseFovY").get_to(c.baseFovY);
    j.at("fovYRate").get_to(c.fovYRateBase);
    j.at("fovYRateCommonRate").get_to(c.fovYRateCommonRate);
    j.at("fovYInterpolate").get_to(c.fovYInterpolate);
    j.at("fixForForwardSpeed").get_to(c.fixForForwardSpeed);
}
