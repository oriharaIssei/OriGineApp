#include "PlayerStatus.h"

/// component
#include "component/physics/Rigidbody.h"
#include "component/transform/Transform.h"
#include "PlayerInput.h"
#include "State/PlayerState.h"

/// externals
#ifdef _DEBUG
#include "myGui/MyGui.h"
#include <imgui/imgui.h>
#endif // _DEBUG

/// math
#include <math/Interpolation.h>
#include <math/mathEnv.h>
#include <math/Quaternion.h>
#include <math/Sequence.h>

using namespace OriGine;

PlayerStatus::PlayerStatus() {}
PlayerStatus::~PlayerStatus() {}

void PlayerStatus::Initialize(Scene* /*_scene*/, EntityHandle /*_owner*/) {
    gearUpCoolTime_  = baseGearupCoolTime_;
    currentMaxSpeed_ = baseSpeed_;
}

void PlayerStatus::Edit([[maybe_unused]] Scene* _scene, [[maybe_unused]] EntityHandle _owner, [[maybe_unused]] const std::string& _parentLabel) {
#ifdef _DEBUG

    std::string label = "Speed##" + _parentLabel;
    if (ImGui::TreeNode(label.c_str())) {
        DragGuiCommand("baseSpeed##" + _parentLabel, baseSpeed_, 0.01f);
        DragGuiCommand("speedUpRateBase##" + _parentLabel, speedUpRateBase_, 0.01f);
        DragGuiCommand("speedUpRateCommonRate##" + _parentLabel, speedUpRateCommonRate_, 0.01f);

        label = "SpeedByGearLevel##" + _parentLabel;
        if (ImGui::BeginTable(label.c_str(), 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
            ImGui::TableSetupColumn("Gear Level");
            ImGui::TableSetupColumn("Speed");
            ImGui::TableHeadersRow();
            for (int level = 1; level <= kMaxPlayerGearLevel; ++level) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%d", level);
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%.2f", CalculateSpeedByGearLevel(level));
            }
            ImGui::EndTable();
        }
        ImGui::TreePop();
    }

    ImGui::Spacing();

    label = "CoolTime##" + _parentLabel;
    if (ImGui::TreeNode(label.c_str())) {
        DragGuiCommand("gearUpCoolTime##" + _parentLabel, baseGearupCoolTime_, 0.01f);
        DragGuiCommand("coolTimeAddRateBase##" + _parentLabel, coolTimeAddRateBase_, 0.01f);
        DragGuiCommand("coolTimeAddRateCommonRate##" + _parentLabel, coolTimeAddRateCommonRate_, 0.01f);

        label = "CoolTimeByGearLevel##" + _parentLabel;
        if (ImGui::BeginTable("CoolTimeByGearLevel", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
            ImGui::TableSetupColumn("Gear Level");
            ImGui::TableSetupColumn("CoolTime");
            ImGui::TableHeadersRow();
            for (int level = 1; level <= kMaxPlayerGearLevel; ++level) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%d", level);
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%.2f", CalculateCoolTimeByGearLevel(level));
            }
            ImGui::EndTable();
        }
        ImGui::TreePop();
    }

    ImGui::Spacing();

    DragGuiCommand("invincibilityTime##" + _parentLabel, invincibilityTime_, 0.01f);

    ImGui::Spacing();

    DragGuiCommand("directionInterpolateRate##" + _parentLabel, directionInterpolateRate_, 0.01f);

    ImGui::Spacing();

    if (ImGui::TreeNode("JumpHoldVelocity")) {
        EasingComboGui("EaseType##JumpHoldVelocity" + _parentLabel, jumpHoldVelocityEaseType_);

        DragGuiCommand("Min##JumpHoldVelocity##" + _parentLabel, minJumpHoldVelocity_, 0.01f);
        DragGuiCommand("Max##JumpHoldVelocity##" + _parentLabel, maxJumpHoldVelocity_, 0.01f);

        ImGui::TreePop();
    }

    ImGui::Spacing();

    if (ImGui::TreeNode("JumpChargeRate")) {
        EasingComboGui("EaseType##JumpChargeRate##" + _parentLabel, jumpChargeRateEaseType_);

        DragGuiCommand("Min##JumpChargeRate##" + _parentLabel, minJumpChargeRate_, 0.01f);
        DragGuiCommand("Max##JumpChargeRate##" + _parentLabel, maxJumpChargeRate_, 0.01f);

        ImGui::TreePop();
    }

    ImGui::Spacing();

    DragGuiCommand("RisingGravityRate##" + _parentLabel, risingGravityRate_, 0.01f);
    DragGuiCommand("FallingGravityRate##" + _parentLabel, fallingGravityRate_, 0.01f);

    ImGui::Spacing();

    DragGuiCommand("WallRunRate##" + _parentLabel, wallRunRate_, 0.01f);
    DragGuiCommand("WallRunRampUpTime##" + _parentLabel, wallRunRampUpTime_, 0.01f);
    DragGuiVectorCommand<3, float>("WallJumpOffset##" + _parentLabel, wallJumpOffset_, 0.01f);

    DragGuiVectorCommand<3, float>("WheelieJumpOffset##" + _parentLabel, wheelieJumpOffset_, 0.01f);

#endif // _DEBUG
}

void PlayerStatus::Debug(Scene* /*_scene*/, EntityHandle /*_handle*/, const std::string& /*_parentLabel*/) {
#ifdef _DEBUG

    ImGui::Text("Base Gear Up Cool Time : %.2f", baseGearupCoolTime_);
    ImGui::Text("Gear Up Cool Time      : %.2f", gearUpCoolTime_);
    ImGui::Spacing();
    ImGui::Text("Base Speed          : %.2f", baseSpeed_);
    ImGui::Text("CurrentMax Speed       : %.2f", currentMaxSpeed_);

    if (ImGui::TreeNode("JumpHoldVelocity")) {
        ImGui::Text("EaseType : %s", EasingNames[jumpHoldVelocityEaseType_].c_str());

        ImGui::Text("Min : %.3", minJumpHoldVelocity_);
        ImGui::Text("Max : %.3", maxJumpHoldVelocity_);

        ImGui::TreePop();
    }
    if (ImGui::TreeNode("JumpChargeRate")) {
        ImGui::Text("EaseType : %s", EasingNames[jumpHoldVelocityEaseType_].c_str());

        ImGui::Text("Min : %.3", minJumpChargeRate_);
        ImGui::Text("Max : %.3", maxJumpChargeRate_);

        ImGui::TreePop();
    }

    ImGui::Text("Rising Gravity Rate  : %.2f", risingGravityRate_);
    ImGui::Text("Falling Gravity Rate : %.2f", fallingGravityRate_);

    ImGui::Spacing();
    ImGui::Text("Speed Up Rate Base        : %.2f", speedUpRateBase_);
    ImGui::Text("Speed Up Rate Common Rate : %.2f", speedUpRateCommonRate_);
    ImGui::Spacing();
    ImGui::Text("Cool Time Up Rate Base        : %.2f", coolTimeAddRateBase_);
    ImGui::Text("Cool Time Up Rate Common Rate : %.2f", coolTimeAddRateCommonRate_);
    ImGui::Spacing();
    ImGui::Text("Wall Run Rate             : %.2f", wallRunRate_);
    ImGui::Text("Wall Run Ramp Up Time     : %.2f", wallRunRampUpTime_);
    ImGui::Text("Wall Jump Offset          : (%.2f, %.2f, %.2f)", wallJumpOffset_[X], wallJumpOffset_[Y], wallJumpOffset_[Z]);
    ImGui::Text("Direction Interpolate Rate: %.2f", directionInterpolateRate_);
#endif
}

void PlayerStatus::Finalize() {}

float PlayerStatus::CalculateSpeedByGearLevel(int32_t _gearLevel) const {
    return ArithmeticSequence<float>(
        baseSpeed_,
        ArithmeticSequence<float>(speedUpRateBase_, speedUpRateCommonRate_, _gearLevel - 1),
        _gearLevel);
}

float PlayerStatus::CalculateCoolTimeByGearLevel(int32_t _gearLevel) const {
    return GeometricSequence<float>(
        baseGearupCoolTime_,
        ArithmeticSequence<float>(coolTimeAddRateBase_, coolTimeAddRateCommonRate_, _gearLevel - 1),
        _gearLevel);
}

void PlayerStatus::UpdateAccel(float _deltaTime, PlayerInput* _input, Transform* _transform, Rigidbody* _rigidbody, const Quaternion& _cameraRotation) {
    constexpr float kPlayerAccelRate = 8.0f;

    // 入力方向を取得
    Vec2f inputDirection = _input->GetInputDirection();

    if (inputDirection.lengthSq() <= 0.0f) {
        _input->SetWorldInputDirection(Vec3f());
        return;
    }

    // カメラのヨー（Y軸回転角）を取得
    // --- 安定したカメラYaw抽出 ---
    float cameraYaw = _cameraRotation.ToYaw();

    // 入力方向を3Dベクトルに変換（Zが前、Xが右）
    Vec3f inputDir3D = {inputDirection[X], 0.0f, inputDirection[Y]};
    inputDir3D       = inputDir3D.normalize();

    // カメラの向きに合わせて入力方向を回転（ローカル→ワールド変換）
    Vec3f moveDirWorld = inputDir3D * MakeMatrix4x4::RotateY(cameraYaw);
    moveDirWorld       = moveDirWorld.normalize();
    // ワールド方向に変換した入力方向を保存
    _input->SetWorldInputDirection(moveDirWorld);

    // 現在の移動方向と補間
    Vec3f currentXZDir = _rigidbody->GetVelocity();
    currentXZDir[Y]    = 0.0f;
    if (currentXZDir.lengthSq() <= kEpsilon) {
        currentXZDir = axisZ * MakeMatrix4x4::RotateQuaternion(_transform->rotate);
    }

    currentXZDir = currentXZDir.normalize();

    moveDirWorld = LerpByDeltaTime(currentXZDir, moveDirWorld, _deltaTime, directionInterpolateRate_);
    moveDirWorld = moveDirWorld.normalize();

    // プレイヤーの回転をカメラ方向に合わせる(更新分だけ回転)
    _transform->rotate = Quaternion::LookAt(moveDirWorld, axisY);

    // 移動加速度を設定
    Vec3f accel = moveDirWorld * (currentMaxSpeed_ * kPlayerAccelRate);
    _rigidbody->SetAcceleration(X, accel[X]);
    _rigidbody->SetAcceleration(Z, accel[Z]);
}

void to_json(nlohmann::json& j, const PlayerStatus& _playerStatus) {
    j["baseSpeed"]         = _playerStatus.baseSpeed_;
    j["wallRunRate"]       = _playerStatus.wallRunRate_;
    j["wallRunRumpUpTime"] = _playerStatus.wallRunRampUpTime_;
    j["wallJumpOffset"]    = _playerStatus.wallJumpOffset_;

    j["jumpHoldVelocityEaseType"] = static_cast<int>(_playerStatus.jumpHoldVelocityEaseType_);
    j["minJumpHoldVelocity"]      = _playerStatus.minJumpHoldVelocity_;
    j["maxJumpHoldVelocity"]      = _playerStatus.maxJumpHoldVelocity_;
    j["jumpChargeRateEaseType"]   = static_cast<int>(_playerStatus.jumpChargeRateEaseType_);
    j["minJumpChargeRate"]        = _playerStatus.minJumpChargeRate_;
    j["maxJumpChargeRate"]        = _playerStatus.maxJumpChargeRate_;

    j["risingGravityRate"]  = _playerStatus.risingGravityRate_;
    j["fallingGravityRate"] = _playerStatus.fallingGravityRate_;

    j["gearUpCoolTime"]           = _playerStatus.baseGearupCoolTime_;
    j["directionInterpolateRate"] = _playerStatus.directionInterpolateRate_;

    j["speedUpRateBase"]           = _playerStatus.speedUpRateBase_;
    j["speedUpRateCommonRate"]     = _playerStatus.speedUpRateCommonRate_;
    j["coolTimeAddRateBase"]       = _playerStatus.coolTimeAddRateBase_;
    j["coolTimeAddRateCommonRate"] = _playerStatus.coolTimeAddRateCommonRate_;

    j["wheelieJumpOffset"] = _playerStatus.wheelieJumpOffset_;
}
void from_json(const nlohmann::json& j, PlayerStatus& _playerStatus) {
    j.at("baseSpeed").get_to(_playerStatus.baseSpeed_);

    int easetype = 0;
    j.at("jumpHoldVelocityEaseType").get_to(easetype);
    _playerStatus.jumpHoldVelocityEaseType_ = static_cast<EaseType>(easetype);
    j.at("minJumpHoldVelocity").get_to(_playerStatus.minJumpHoldVelocity_);
    j.at("maxJumpHoldVelocity").get_to(_playerStatus.maxJumpHoldVelocity_);

    j.at("jumpChargeRateEaseType").get_to(easetype);
    _playerStatus.jumpChargeRateEaseType_ = static_cast<EaseType>(easetype);
    j.at("minJumpChargeRate").get_to(_playerStatus.minJumpChargeRate_);
    j.at("maxJumpChargeRate").get_to(_playerStatus.maxJumpChargeRate_);

    j.at("risingGravityRate").get_to(_playerStatus.risingGravityRate_);
    j.at("fallingGravityRate").get_to(_playerStatus.fallingGravityRate_);

    j.at("wallRunRate").get_to(_playerStatus.wallRunRate_);
    j.at("wallRunRumpUpTime").get_to(_playerStatus.wallRunRampUpTime_);
    j.at("wallJumpOffset").get_to(_playerStatus.wallJumpOffset_);
    j.at("gearUpCoolTime").get_to(_playerStatus.baseGearupCoolTime_);
    j.at("directionInterpolateRate").get_to(_playerStatus.directionInterpolateRate_);

    j.at("speedUpRateBase").get_to(_playerStatus.speedUpRateBase_);
    j.at("speedUpRateCommonRate").get_to(_playerStatus.speedUpRateCommonRate_);
    j.at("coolTimeAddRateBase").get_to(_playerStatus.coolTimeAddRateBase_);
    j.at("coolTimeAddRateCommonRate").get_to(_playerStatus.coolTimeAddRateCommonRate_);

    if (j.contains("wheelieJumpOffset")) {
        j.at("wheelieJumpOffset").get_to(_playerStatus.wheelieJumpOffset_);
    }
}
