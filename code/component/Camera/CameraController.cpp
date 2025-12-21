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
    currentOffset_       = firstOffset_;
    currentTargetOffset_ = firstTargetOffset_;
}

void CameraController::Edit(Scene* /*_scene*/, EntityHandle /*_OriGine::Entity*/, [[maybe_unused]] const std::string& _parentLabel) {
#ifdef _DEBUG

    if (DragGuiVectorCommand<3, float>("forward##" + _parentLabel, forward_, 0.01f, -1.f, 1.f, "%.3f", [this](Vector<3, float>* _v) {
            *_v = OriGine::Vec3f::Normalize(OriGine::Vec3f(_v->v[X], _v->v[Y], _v->v[Z]));
        })) {
        forward_ = OriGine::Vec3f::Normalize(forward_); // 正規化
    }
    DragGuiCommand("Fix For Forward Speed##" + _parentLabel, fixForForwardSpeed_, 0.01f, 0.0f);
    DragGuiCommand("angleLimitY##" + _parentLabel, angleLimitY_, 0.01f, 0.00001f, std::numbers::pi_v<float> * 2.f, "%.3f");

    std::string label = "Offset##" + _parentLabel;
    if (ImGui::TreeNode(label.c_str())) {
        DragGuiVectorCommand("FirstTargetOffset##" + _parentLabel, firstTargetOffset_, 0.01f);
        DragGuiVectorCommand("TargetOffsetOnDash##" + _parentLabel, targetOffsetOnDash_, 0.01f);
        DragGuiVectorCommand("TargetOffsetOnWallRun##" + _parentLabel, targetOffsetOnWallRun_, 0.01f);

        ImGui::Spacing();

        DragGuiVectorCommand("firstOffset##" + _parentLabel, firstOffset_, 0.01f);
        DragGuiVectorCommand("offsetOnDash##" + _parentLabel, offsetOnDash_, 0.01f);
        DragGuiVectorCommand("offsetOnWallRun##" + _parentLabel, offsetOnWallRun_, 0.01f);

        ImGui::TreePop();
    }

    ImGui::Spacing();

    DragGuiCommand("rotateSpeedPadStick##" + _parentLabel, rotateSpeedPadStick_, 0.01f);
    DragGuiCommand("rotateSpeedMouse##" + _parentLabel, rotateSpeedMouse_, 0.01f);

    DragGuiCommand("rotateSensitivity##" + _parentLabel, rotateSensitivity_, 0.01f);
    DragGuiCommand("interTargetInterpolation##" + _parentLabel, interTargetInterpolation_, 0.01f);

    ImGui::Spacing();

    DragGuiCommand("maxRotateX##" + _parentLabel, maxRotateX_, 0.01f);
    DragGuiCommand("minRotateX##" + _parentLabel, minRotateX_, 0.01f);

    ImGui::Spacing();

    label = "Fov##" + _parentLabel;
    if (ImGui::TreeNode(label.c_str())) {
        DragGuiCommand("fovYInterpolate##" + _parentLabel, fovYInterpolate_, 0.001f);
        DragGuiCommand("baseFovY##" + _parentLabel, baseFovY_, 0.01f);
        DragGuiCommand("fovYRateBase##" + _parentLabel, fovYRateBase_, 0.001f);
        DragGuiCommand("fovYRateCommonRate##" + _parentLabel, fovYRateCommonRate_, 0.001f);

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
        baseFovY_,
        ArithmeticSequence<float>(fovYRateBase_, fovYRateCommonRate_, _level - 1),
        _level);
}

void to_json(nlohmann::json& j, const CameraController& c) {
    j["forward"]                  = c.forward_;
    j["angleLimitY"]              = c.angleLimitY_;
    j["firstOffset"]              = c.firstOffset_;
    j["offsetOnDash"]             = c.offsetOnDash_;
    j["offsetOnWallRun"]          = c.offsetOnWallRun_;
    j["firstTargetOffset"]        = c.firstTargetOffset_;
    j["targetOffsetOnDash"]       = c.targetOffsetOnDash_;
    j["targetOffsetOnWallRun"]    = c.targetOffsetOnWallRun_;
    j["rotateSpeedPadStick"]      = c.rotateSpeedPadStick_;
    j["rotateSpeedMouse"]         = c.rotateSpeedMouse_;
    j["rotateSensitivity"]        = c.rotateSensitivity_;
    j["interTargetInterpolation"] = c.interTargetInterpolation_;
    j["maxRotateX"]               = c.maxRotateX_;
    j["minRotateX"]               = c.minRotateX_;
    j["baseFovY"]                 = c.baseFovY_;
    j["fovYRate"]                 = c.fovYRateBase_;
    j["fovYRateCommonRate"]       = c.fovYRateCommonRate_;
    j["fovYInterpolate"]          = c.fovYInterpolate_;
    j["fixForForwardSpeed"]       = c.fixForForwardSpeed_;
}

void from_json(const nlohmann::json& j, CameraController& c) {
    j.at("forward").get_to(c.forward_);
    j.at("angleLimitY").get_to(c.angleLimitY_);

    j.at("firstOffset").get_to(c.firstOffset_);
    j.at("offsetOnDash").get_to(c.offsetOnDash_);
    j.at("offsetOnWallRun").get_to(c.offsetOnWallRun_);

    j.at("firstTargetOffset").get_to(c.firstTargetOffset_);
    j.at("targetOffsetOnDash").get_to(c.targetOffsetOnDash_);
    j.at("targetOffsetOnWallRun").get_to(c.targetOffsetOnWallRun_);

    j.at("rotateSpeedPadStick").get_to(c.rotateSpeedPadStick_);
    j.at("rotateSpeedMouse").get_to(c.rotateSpeedMouse_);
    j.at("rotateSensitivity").get_to(c.rotateSensitivity_);
    j.at("interTargetInterpolation").get_to(c.interTargetInterpolation_);
    j.at("maxRotateX").get_to(c.maxRotateX_);
    j.at("minRotateX").get_to(c.minRotateX_);
    j.at("baseFovY").get_to(c.baseFovY_);
    j.at("fovYRate").get_to(c.fovYRateBase_);
    j.at("fovYRateCommonRate").get_to(c.fovYRateCommonRate_);
    j.at("fovYInterpolate").get_to(c.fovYInterpolate_);
    j.at("fixForForwardSpeed").get_to(c.fixForForwardSpeed_);
}
