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
#include <math/mathEnv.h>
#include <math/Quaternion.h>
#include <math/Sequence.h>

PlayerStatus::PlayerStatus() {}
PlayerStatus::~PlayerStatus() {}

void PlayerStatus::Initialize(Entity* /*_entity*/) {
    gearUpCoolTime_  = baseGearupCoolTime_;
    currentMaxSpeed_ = baseSpeed_;
}

void PlayerStatus::Edit([[maybe_unused]] Scene* _scene, [[maybe_unused]] Entity* _entity, [[maybe_unused]] const std::string& _parentLabel) {
#ifdef _DEBUG

    if (ImGui::TreeNode("Speed")) {
        DragGuiCommand("baseSpeed", baseSpeed_);
        DragGuiCommand("speedUpRateBase", speedUpRateBase_);
        DragGuiCommand("speedUpRateCommonRate", speedUpRateCommonRate_);
        if (ImGui::BeginTable("SpeedByGearLevel", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
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

    if (ImGui::TreeNode("CoolTime")) {
        DragGuiCommand("gearUpCoolTime", baseGearupCoolTime_);
        DragGuiCommand("coolTimeAddRateBase", coolTimeAddRateBase_);
        DragGuiCommand("coolTimeAddRateCommonRate", coolTimeAddRateCommonRate_);
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

    DragGuiCommand("directionInterpolateRate", directionInterpolateRate_);
    DragGuiCommand("jumpPower", jumpPower_);
    DragGuiCommand("fallPower", fallPower_);
    DragGuiCommand("wallRunRate", wallRunRate_);
    DragGuiVectorCommand<3, float>("WallJumpOffset", wallJumpOffset_, 0.01f);

#endif // _DEBUG
}

void PlayerStatus::Debug(Scene* /*_scene*/, Entity* /*_entity*/, const std::string& /*_parentLabel*/) {
#ifdef _DEBUG

    ImGui::Text("Base Gear Up Cool Time : %.2f", baseGearupCoolTime_);
    ImGui::Text("Gear Up Cool Time      : %.2f", gearUpCoolTime_);
    ImGui::Spacing();
    ImGui::Text("Base Speed          : %.2f", baseSpeed_);
    ImGui::Text("CurrentMax Speed       : %.2f", currentMaxSpeed_);
    ImGui::Text("Jump Power          : %.2f", jumpPower_);
    ImGui::Text("Fall Power          : %.2f", fallPower_);

    ImGui::Spacing();
    ImGui::Text("Speed Up Rate Base        : %.2f", speedUpRateBase_);
    ImGui::Text("Speed Up Rate Common Rate : %.2f", speedUpRateCommonRate_);
    ImGui::Spacing();
    ImGui::Text("Cool Time Up Rate Base        : %.2f", coolTimeAddRateBase_);
    ImGui::Text("Cool Time Up Rate Common Rate : %.2f", coolTimeAddRateCommonRate_);
    ImGui::Spacing();
    ImGui::Text("Wall Run Rate             : %.2f", wallRunRate_);
    ImGui::Text("Wall Jump Offset       : (%.2f, %.2f, %.2f)", wallJumpOffset_[X], wallJumpOffset_[Y], wallJumpOffset_[Z]);
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
    Vec2f inputDirection = _input->getInputDirection();

    if (inputDirection.lengthSq() <= 0.0f) {
        _input->setWorldInputDirection(Vec3f());
        return;
    }

    // カメラのヨー（Y軸回転角）を取得
    // --- 安定したカメラYaw抽出 ---
    float cameraYaw = _cameraRotation.ToYaw();

    // 入力方向を3Dベクトルに変換（Zが前、Xが右）
    Vec3f inputDir3D = {inputDirection[X], 0.0f, inputDirection[Y]};
    inputDir3D.normalize();

    // カメラの向きに合わせて入力方向を回転（ローカル→ワールド変換）
    Vec3f moveDirWorld = inputDir3D * MakeMatrix::RotateY(cameraYaw);
    moveDirWorld       = moveDirWorld.normalize();
    // ワールド方向に変換した入力方向を保存
    _input->setWorldInputDirection(moveDirWorld);

    // 現在の移動方向と補間
    Vec3f currentDir = _rigidbody->getVelocity();
    if (currentDir.lengthSq() <= kEpsilon) {
        currentDir = axisZ * MakeMatrix::RotateQuaternion(_transform->rotate);
    }
    currentDir[Y] = 0.0f;
    currentDir    = currentDir.normalize();

    float alpha  = 1.0f - std::exp(-directionInterpolateRate_ * _deltaTime);
    moveDirWorld = Lerp<3, float>(currentDir, moveDirWorld, alpha);
    moveDirWorld = moveDirWorld.normalize();

    // プレイヤーの回転をカメラ方向に合わせる(更新分だけ回転)
    _transform->rotate = Quaternion::LookAt(moveDirWorld, axisY);

    // 移動加速度を設定
    Vec3f accel = moveDirWorld * (currentMaxSpeed_ * kPlayerAccelRate);
    _rigidbody->setAcceleration(X, accel[X]);
    _rigidbody->setAcceleration(Z, accel[Z]);
}

void to_json(nlohmann::json& j, const PlayerStatus& _playerStatus) {
    j["baseSpeed"]                = _playerStatus.baseSpeed_;
    j["wallRunRate"]              = _playerStatus.wallRunRate_;
    j["wallJumpOffset"]           = _playerStatus.wallJumpOffset_;
    j["jumpPower"]                = _playerStatus.jumpPower_;
    j["fallPower"]                = _playerStatus.fallPower_;
    j["gearUpCoolTime"]           = _playerStatus.baseGearupCoolTime_;
    j["directionInterpolateRate"] = _playerStatus.directionInterpolateRate_;

    j["speedUpRateBase"]           = _playerStatus.speedUpRateBase_;
    j["speedUpRateCommonRate"]     = _playerStatus.speedUpRateCommonRate_;
    j["coolTimeAddRateBase"]       = _playerStatus.coolTimeAddRateBase_;
    j["coolTimeAddRateCommonRate"] = _playerStatus.coolTimeAddRateCommonRate_;
}
void from_json(const nlohmann::json& j, PlayerStatus& _playerStatus) {
    j.at("baseSpeed").get_to(_playerStatus.baseSpeed_);
    j.at("jumpPower").get_to(_playerStatus.jumpPower_);
    j.at("fallPower").get_to(_playerStatus.fallPower_);
    j.at("wallRunRate").get_to(_playerStatus.wallRunRate_);
    j.at("wallJumpOffset").get_to(_playerStatus.wallJumpOffset_);
    j.at("gearUpCoolTime").get_to(_playerStatus.baseGearupCoolTime_);
    j.at("directionInterpolateRate").get_to(_playerStatus.directionInterpolateRate_);

    j.at("speedUpRateBase").get_to(_playerStatus.speedUpRateBase_);
    j.at("speedUpRateCommonRate").get_to(_playerStatus.speedUpRateCommonRate_);
    j.at("coolTimeAddRateBase").get_to(_playerStatus.coolTimeAddRateBase_);
    j.at("coolTimeAddRateCommonRate").get_to(_playerStatus.coolTimeAddRateCommonRate_);
}
