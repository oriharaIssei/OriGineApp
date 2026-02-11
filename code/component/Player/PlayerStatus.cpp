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

    currentWallRunInterval_ = 0.f;
    currentWheelieInterval_ = 0.f;
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

    DragGuiCommand("Invincibility Time##" + _parentLabel, invincibilityTime_, 0.01f);
    DragGuiCommand("Wall Run Interval##" + _parentLabel, wallRunInterval_, 0.01f);

    ImGui::Spacing();

    DragGuiCommand("Direction Interpolate Rate##" + _parentLabel, directionInterpolateRate_, 0.01f);
    DragGuiCommand("Speed Restore Lerp Rate##" + _parentLabel, speedRestoreLerpRate_, 0.01f);

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
    wallJumpOffset_ = wallJumpOffset_.normalize();

    DragGuiVectorCommand<3, float>("WheelieJumpOffset##" + _parentLabel, wheelieJumpOffset_, 0.01f);
    wheelieJumpOffset_ = wheelieJumpOffset_.normalize();
    DragGuiCommand("GravityApplyDelay On WallRun##" + _parentLabel, gravityApplyDelayOnWallRun_, 0.01f);
    DragGuiCommand("WallRunDetachSpeed##" + _parentLabel, wallRunDetachSpeed_, 0.01f);

    ImGui::Spacing();

    DragGuiCommand("RailSpeedRate##" + _parentLabel, railSpeedRate_, 0.01f);
    DragGuiCommand("RailRampUpTime##" + _parentLabel, railRampUpTime_, 0.01f);
    DragGuiVectorCommand<3, float>("RailJumpOffset##" + _parentLabel, railJumpOffset_, 0.01f);
    railJumpOffset_ = railJumpOffset_.normalize();
    DragGuiCommand("RailInterval##" + _parentLabel, railInterval_, 0.01f);

    ImGui::Spacing();
    DragGuiCommand("Default Mass##" + _parentLabel, defaultMass_, 0.01f);
    DragGuiCommand("Mass On Wall Run##" + _parentLabel, massOnWallRun_, 0.01f);

    ImGui::Spacing();

    DragGuiCommand("Ground Check Threshold##" + _parentLabel, groundCheckThreshold_, 0.01f);
    DragGuiCommand("Wall Check Threshold##" + _parentLabel, wallCheckThreshold_, 0.01f);
    DragGuiCommand("Max Wheelie Fall Speed##" + _parentLabel, maxWheelieFallSpeed_, 0.01f);

#endif // _DEBUG
}

void PlayerStatus::Debug(Scene* /*_scene*/, EntityHandle /*_handle*/, const std::string& /*_parentLabel*/) {
#ifdef _DEBUG

    ImGui::Text("Base Gear Up Cool Time : %.2f", baseGearupCoolTime_);
    ImGui::Text("Gear Up Cool Time      : %.2f", gearUpCoolTime_);
    ImGui::Spacing();
    ImGui::Text("Base Speed             : %.2f", baseSpeed_);
    ImGui::Text("CurrentMax Speed       : %.2f", currentMaxSpeed_);
    ImGui::Text("Speed Restore Lerp Rate: %.2f", speedRestoreLerpRate_);

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

void PlayerStatus::UpdateAccel(float _deltaTime, const Vec3f& _newDirection, Rigidbody* _rigidbody) {
    constexpr float kPlayerAccelRate = 8.0f;

    Vec2f xzVelo       = Vec2f(_rigidbody->GetVelocity()[X], _rigidbody->GetVelocity()[Z]);
    float currentSpeed = xzVelo.length();
    if (currentSpeed >= currentMaxSpeed_) {
        // 速度が最大速度以上の場合、加速度を減少させる
        currentSpeed = LerpByDeltaTime(currentSpeed, currentMaxSpeed_, _deltaTime, speedRestoreLerpRate_);
        xzVelo       = xzVelo.normalize() * currentSpeed;

        _rigidbody->SetVelocity(X, xzVelo[X]);
        _rigidbody->SetVelocity(Z, xzVelo[Y]);
    } else if (currentSpeed <= 0.f) {
        currentSpeed = currentMaxSpeed_;
    }

    // 移動加速度を設定
    Vec3f accel = _newDirection * (currentSpeed * kPlayerAccelRate);
    _rigidbody->SetAcceleration(X, accel[X]);
    _rigidbody->SetAcceleration(Z, accel[Z]);
}

Vec3f PlayerStatus::ComputeSmoothedDirection(const Vec3f& _targetDir, const Rigidbody* _rigidbody, const Transform* _transform, float _deltaTime) const {
    // 現在のXZ平面の速度を取得
    Vec3f currentDir = _rigidbody->GetVelocity();
    currentDir[Y]    = 0.0f;

    // 停止している場合は、現在の体の向きを基準にする
    if (currentDir.lengthSq() <= kEpsilon) {
        currentDir = axisZ * MakeMatrix4x4::RotateQuaternion(_transform->rotate);
    }
    currentDir = currentDir.normalize();

    // 補間計算
    Vec3f resultDir = LerpByDeltaTime(currentDir, _targetDir, _deltaTime, directionInterpolateRate_);
    return resultDir.normalize();
}

void to_json(nlohmann::json& _j, const PlayerStatus& _playerStatus) {
    _j["baseSpeed"] = _playerStatus.baseSpeed_;

    _j["wallRunRate"]       = _playerStatus.wallRunRate_;
    _j["wallRunRampUpTime"] = _playerStatus.wallRunRampUpTime_;
    _j["wallJumpOffset"]    = _playerStatus.wallJumpOffset_;

    _j["jumpHoldVelocityEaseType"] = static_cast<int>(_playerStatus.jumpHoldVelocityEaseType_);
    _j["minJumpHoldVelocity"]      = _playerStatus.minJumpHoldVelocity_;
    _j["maxJumpHoldVelocity"]      = _playerStatus.maxJumpHoldVelocity_;
    _j["jumpChargeRateEaseType"]   = static_cast<int>(_playerStatus.jumpChargeRateEaseType_);
    _j["minJumpChargeRate"]        = _playerStatus.minJumpChargeRate_;
    _j["maxJumpChargeRate"]        = _playerStatus.maxJumpChargeRate_;

    _j["risingGravityRate"]  = _playerStatus.risingGravityRate_;
    _j["fallingGravityRate"] = _playerStatus.fallingGravityRate_;

    _j["gearUpCoolTime"]           = _playerStatus.baseGearupCoolTime_;
    _j["directionInterpolateRate"] = _playerStatus.directionInterpolateRate_;
    _j["speedRestoreLerpRate"]     = _playerStatus.speedRestoreLerpRate_;

    _j["speedUpRateBase"]           = _playerStatus.speedUpRateBase_;
    _j["speedUpRateCommonRate"]     = _playerStatus.speedUpRateCommonRate_;
    _j["coolTimeAddRateBase"]       = _playerStatus.coolTimeAddRateBase_;
    _j["coolTimeAddRateCommonRate"] = _playerStatus.coolTimeAddRateCommonRate_;

    _j["wheelieJumpOffset"] = _playerStatus.wheelieJumpOffset_;

    _j["gravityApplyDelayOnWallRun"] = _playerStatus.gravityApplyDelayOnWallRun_;
    _j["wallRunDetachSpeed"]         = _playerStatus.wallRunDetachSpeed_;

    _j["defaultMass"]   = _playerStatus.defaultMass_;
    _j["massOnWallRun"] = _playerStatus.massOnWallRun_;

    _j["wallRunInterval"] = _playerStatus.wallRunInterval_;

    _j["railSpeedRate"]  = _playerStatus.railSpeedRate_;
    _j["railRampUpTime"] = _playerStatus.railRampUpTime_;
    _j["railJumpOffset"] = _playerStatus.railJumpOffset_;
    _j["railInterval"]   = _playerStatus.railInterval_;

    _j["groundCheckThreshold"] = _playerStatus.groundCheckThreshold_;
    _j["wallCheckThreshold"]   = _playerStatus.wallCheckThreshold_;
    _j["maxWheelieFallSpeed"]  = _playerStatus.maxWheelieFallSpeed_;
}
void from_json(const nlohmann::json& _j, PlayerStatus& _playerStatus) {
    _j.at("baseSpeed").get_to(_playerStatus.baseSpeed_);

    int easetype = 0;
    _j.at("jumpHoldVelocityEaseType").get_to(easetype);
    _playerStatus.jumpHoldVelocityEaseType_ = static_cast<EaseType>(easetype);
    _j.at("minJumpHoldVelocity").get_to(_playerStatus.minJumpHoldVelocity_);
    _j.at("maxJumpHoldVelocity").get_to(_playerStatus.maxJumpHoldVelocity_);

    _j.at("jumpChargeRateEaseType").get_to(easetype);
    _playerStatus.jumpChargeRateEaseType_ = static_cast<EaseType>(easetype);
    _j.at("minJumpChargeRate").get_to(_playerStatus.minJumpChargeRate_);
    _j.at("maxJumpChargeRate").get_to(_playerStatus.maxJumpChargeRate_);

    _j.at("risingGravityRate").get_to(_playerStatus.risingGravityRate_);
    _j.at("fallingGravityRate").get_to(_playerStatus.fallingGravityRate_);

    _j.at("wallRunRate").get_to(_playerStatus.wallRunRate_);
    _j.at("wallRunRampUpTime").get_to(_playerStatus.wallRunRampUpTime_);
    _j.at("wallJumpOffset").get_to(_playerStatus.wallJumpOffset_);
    _j.at("gearUpCoolTime").get_to(_playerStatus.baseGearupCoolTime_);

    _j.at("directionInterpolateRate").get_to(_playerStatus.directionInterpolateRate_);
    if (_j.contains("speedRestoreLerpRate")) {
        _j.at("speedRestoreLerpRate").get_to(_playerStatus.speedRestoreLerpRate_);
    }

    _j.at("speedUpRateBase").get_to(_playerStatus.speedUpRateBase_);
    _j.at("speedUpRateCommonRate").get_to(_playerStatus.speedUpRateCommonRate_);
    _j.at("coolTimeAddRateBase").get_to(_playerStatus.coolTimeAddRateBase_);
    _j.at("coolTimeAddRateCommonRate").get_to(_playerStatus.coolTimeAddRateCommonRate_);

    if (_j.contains("railRampUpTime")) {
        _j.at("railRampUpTime").get_to(_playerStatus.railRampUpTime_);
    }
    if (_j.contains("railSpeedRate")) {
        _j.at("railSpeedRate").get_to(_playerStatus.railSpeedRate_);
    }
    if (_j.contains("railJumpOffset")) {
        _j.at("railJumpOffset").get_to(_playerStatus.railJumpOffset_);
    }

    if (_j.contains("wheelieJumpOffset")) {
        _j.at("wheelieJumpOffset").get_to(_playerStatus.wheelieJumpOffset_);
    }

    if (_j.contains("gravityApplyDelayOnWallRun")) {
        _j.at("gravityApplyDelayOnWallRun").get_to(_playerStatus.gravityApplyDelayOnWallRun_);
    }
    if (_j.contains("wallRunDetachSpeed")) {
        _j.at("wallRunDetachSpeed").get_to(_playerStatus.wallRunDetachSpeed_);
    }

    if (_j.contains("defaultMass")) {
        _j.at("defaultMass").get_to(_playerStatus.defaultMass_);
    }
    if (_j.contains("massOnWallRun")) {
        _j.at("massOnWallRun").get_to(_playerStatus.massOnWallRun_);
    }

    if (_j.contains("wallRunInterval")) {
        _j.at("wallRunInterval").get_to(_playerStatus.wallRunInterval_);
    }
    if (_j.contains("railInterval")) {
        _j.at("railInterval").get_to(_playerStatus.railInterval_);
    }

    if (_j.contains("groundCheckThreshold")) {
        _j.at("groundCheckThreshold").get_to(_playerStatus.groundCheckThreshold_);
    }
    if (_j.contains("wallCheckThreshold")) {
        _j.at("wallCheckThreshold").get_to(_playerStatus.wallCheckThreshold_);
    }
    if (_j.contains("maxWheelieFallSpeed")) {
        _j.at("maxWheelieFallSpeed").get_to(_playerStatus.maxWheelieFallSpeed_);
    }
}
