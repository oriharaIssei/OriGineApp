#include "CameraController.h"

/// external
#ifdef _DEBUG
#include "myGui/MyGui.h"
#endif // _DEBUG

/// math
#include "math/Sequence.h"
#include <numbers>

void CameraController::Initialize(GameEntity* /*_entity*/) {
}

void CameraController::Edit(Scene* /*_scene*/, GameEntity* /*_entity*/,[[maybe_unused]] const std::string& _parentLabel) {
#ifdef _DEBUG

    if (DragGuiVectorCommand<3, float>("forward" + _parentLabel, forward_, 0.01f, -1.f, 1.f, "%.3f", [this](Vector<3, float>* _v) {
            *_v = Vec3f::Normalize(Vec3f(_v->v[X], _v->v[Y], _v->v[Z]));
        })) {
        forward_ = Vec3f::Normalize(forward_); // 正規化
    }
    DragGuiCommand("angleLimitY" + _parentLabel, angleLimitY_, 0.01f, 0.00001f, std::numbers::pi_v<float> * 2.f, "%.3f");

    DragGuiVectorCommand("followTargetOffset" + _parentLabel, followTargetOffset_, 0.01f, -100.0f, 100.0f);
    DragGuiVectorCommand("followOffset" + _parentLabel, followOffset_, 0.01f, -100.0f, 100.0f);

    ImGui::Spacing();

    DragGuiCommand("rotateSpeedPadStick" + _parentLabel, rotateSpeedPadStick_, 0.01f, -100.0f, 100.0f);
    DragGuiCommand("rotateSpeedMouse" + _parentLabel, rotateSpeedMouse_, 0.01f, -100.0f, 100.0f);

    DragGuiCommand("rotateSensitivity" + _parentLabel, rotateSensitivity_, 0.01f, -1.f, 1.f);
    DragGuiCommand("interTargetInterpolation" + _parentLabel, interTargetInterpolation_, 0.01f, -1.f, 1.0f);

    ImGui::Spacing();

    DragGuiCommand("maxRotateX" + _parentLabel, maxRotateX_, 0.01f, -100.0f, 100.0f);
    DragGuiCommand("minRotateX" + _parentLabel, minRotateX_, 0.01f, -100.0f, 100.0f);

    ImGui::Spacing();

    std::string label = "Fov##"  + _parentLabel;
    if (ImGui::TreeNode(label.c_str())) {
        DragGuiCommand("fovYInterpolate" + _parentLabel, fovYInterpolate_, 0.001f, 0.0f, 1.0f);
        DragGuiCommand("baseFovY" + _parentLabel, baseFovY_, 0.01f);
        DragGuiCommand("fovYRateBase" + _parentLabel, fovYRateBase_, 0.001f);
        DragGuiCommand("fovYRateCommonRate" + _parentLabel, fovYRateCommonRate_, 0.001f);

        label = "FovY By Gear Level" + _parentLabel;
        if (ImGui::BeginTable(label.c_str(), 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
            ImGui::TableSetupColumn("Gear Level");
            ImGui::TableSetupColumn("FovY");
            ImGui::TableHeadersRow();
            for (int level = 1; level <= 6; ++level) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%d", level);
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%.2f", CalculateFovY(level));
            }
            ImGui::EndTable();
        }
        ImGui::TreePop();
    }

#endif // _DEBUG
}

void CameraController::Finalize() {
}

float CameraController::CalculateFovY(int32_t _level) const {
    return ArithmeticSequence<float>(
        baseFovY_,
        ArithmeticSequence<float>(fovYRateBase_, fovYRateCommonRate_, _level - 1),
        _level);
}

void to_json(nlohmann::json& j, const CameraController& c) {
    j["forward"]                  = c.forward_;
    j["angleLimitY"]              = c.angleLimitY_;
    j["followOffset"]             = c.followOffset_;
    j["followTargetOffset"]       = c.followTargetOffset_;
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
}

void from_json(const nlohmann::json& j, CameraController& c) {
    j.at("forward").get_to(c.forward_);
    j.at("angleLimitY").get_to(c.angleLimitY_);
    j.at("followOffset").get_to(c.followOffset_);
    j.at("followTargetOffset").get_to(c.followTargetOffset_);
    j.at("rotateSpeedPadStick").get_to(c.rotateSpeedPadStick_);
    j.at("rotateSpeedMouse").get_to(c.rotateSpeedMouse_);
    j.at("rotateSensitivity").get_to(c.rotateSensitivity_);
    j.at("interTargetInterpolation").get_to(c.interTargetInterpolation_);
    j.at("maxRotateX").get_to(c.maxRotateX_);
    j.at("minRotateX").get_to(c.minRotateX_);
    j.at("baseFovY").get_to(c.baseFovY_);
    j.at("fovYRate").get_to(c.fovYRateBase_);
    j.at("fovYRateCommonRate").get_to(c.fovYRateCommonRate_);
}
