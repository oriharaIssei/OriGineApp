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
#include <math/Sequence.h>

PlayerStatus::PlayerStatus() {}
PlayerStatus::~PlayerStatus() {}

void PlayerStatus::Initialize(GameEntity* /*_entity*/) {}

void PlayerStatus::Edit([[maybe_unused]] Scene* _scene, [[maybe_unused]] GameEntity* _entity, [[maybe_unused]] const std::string& _parentLabel) {
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

void PlayerStatus::Debug(Scene* /*_scene*/, GameEntity* /*_entity*/, const std::string& /*_parentLabel*/) {
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

void PlayerStatus::UpdateAccel(PlayerInput* _input, Transform* _transform, Rigidbody* _rigidbody, const Quaternion& _cameraRotation) {
    // 入力方向を取得
    Vec2f inputDirection = _input->getInputDirection();

    // カメラの回転からヨー（y軸回転）だけを抽出
    float cameraYaw              = _cameraRotation.ToEulerAngles()[Y]; // y成分のみ
    Quaternion cameraYawRotation = Quaternion::RotateAxisAngle(Vec3f(0.f, 1.f, 0.f), cameraYaw);

    // 入力方向の回転
    if (inputDirection.lengthSq() <= 0.0f) {
        return;
    }

    float inputAngle         = std::atan2(inputDirection[X], inputDirection[Y]);
    Quaternion inputRotation = Quaternion::RotateAxisAngle(Vec3f(0.f, 1.f, 0.f), inputAngle);

    // y軸のみの回転合成
    Quaternion targetRotation = Quaternion::Normalize(cameraYawRotation * inputRotation);

    // プレイヤーの回転を補間して設定
    _transform->rotate = Slerp(_transform->rotate, targetRotation, directionInterpolateRate_);

    // 移動速度の計算
    Vec3f movementDirection = Vec3f(0.f, 0.f, 1.f) * MakeMatrix::RotateQuaternion(_transform->rotate);
    Vec3f accel             = movementDirection * (currentMaxSpeed_ * 4.f);

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
