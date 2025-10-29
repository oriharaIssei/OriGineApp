#include "PlayerEffectControlParam.h"

#ifdef _DEBUG
#include <myGui/MyGui.h>
#endif // _DEBUG

PlayerEffectControlParam::PlayerEffectControlParam() {}
PlayerEffectControlParam::~PlayerEffectControlParam() {}

void PlayerEffectControlParam::Initialize(Entity* /*_entity*/) {
    if (trailColorByGearLevel_.empty()) {
        trailColorByGearLevel_ = std::vector<Vector4f>(kMaxPlayerGearLevel, Vector4f(1.f, 1.f, 1.f, 1.f));
    }
}
void PlayerEffectControlParam::Finalize() {}

float PlayerEffectControlParam::CalculateWheelSpinSpeedBySpeed(float _currentSpeed, float _maxSpeed) const {
    return std::lerp(0.0f, maxWheelSpinSpeed_, _currentSpeed / _maxSpeed);
}

float PlayerEffectControlParam::CalculateWheelTiltAngle(const Vec3f& _inputV, const Vec3f& _direction) const {
    // 進行方向のXZ平面への射影を正規化
    Vec3f forwardDir = _direction;
    forwardDir[Y]    = 0.0f;
    forwardDir       = Vector3f::Normalize(forwardDir);
    // 入力ベクトルを3Dベクトルに変換（Zが前、Xが右）
    Vec3f inputDir3D = {_inputV[X], 0.0f, _inputV[Z]};
    inputDir3D       = Vector3f::Normalize(inputDir3D);
    // 進行方向と入力方向の角度差を計算
    float dotProduct      = Vector3f::Dot(forwardDir, inputDir3D);
    dotProduct            = std::clamp(dotProduct, -1.0f, 1.0f); // 安全のためクランプ
    float angleDifference = std::acos(dotProduct); // ラジアン
    // 外積を使って回転方向を決定
    Vec3f crossProduct = forwardDir.cross(inputDir3D);
    if (crossProduct[Y] > 0) {
        angleDifference = -angleDifference; // 左回りの場合は負の角度にする
    }
    // ホイールの傾き角度を計算
    float result = angleDifference * wheelTiltAngleRate_;
    return result;
}

void PlayerEffectControlParam::Edit(Scene* /*_scene*/, Entity* /*_entity*/, [[maybe_unused]] const std::string& _parentLabel) {
#ifdef _DEBUG
    // ギアレベルに応じたトレイルカラー
    std::string label = "";
    if (ImGui::BeginTable("TrailColorByGearLevel", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("Gear Level");
        ImGui::TableSetupColumn("TrailColor");
        ImGui::TableHeadersRow();
        for (int level = 1; level <= kMaxPlayerGearLevel; ++level) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d", level);
            ImGui::TableSetColumnIndex(1);
            label = "TrailColor##" + std::to_string(level) + _parentLabel;
            ColorEditGuiCommand(label, trailColorByGearLevel_[level - 1]);
        }
        ImGui::EndTable();
    }

    // 壁走り時の回転オフセット
    DragGuiCommand("rotateOffsetOnWallRun##" + _parentLabel, rotateOffsetOnWallRun_, 0.01f);
    // 移動時の回転速度率
    DragGuiCommand("maxWheelSpinSpeed##" + _parentLabel, maxWheelSpinSpeed_, 0.001f);
    // 移動時のホイール傾き角度
    DragGuiCommand("wheelTiltAngleRate##" + _parentLabel, wheelTiltAngleRate_, 0.01f);
    // ホイール傾き角度の最大加速度
    DragGuiCommand("wheelTiltAngleMaxAccel##" + _parentLabel, wheelTiltAngleMaxAccel_, 0.01f);

#endif // _DEBUG
}

void to_json(nlohmann::json& j, const PlayerEffectControlParam& _p) {
    j = nlohmann::json{
        {"trailColorByGearLevel", _p.trailColorByGearLevel_},
        {"rotateOffsetOnWallRun", _p.rotateOffsetOnWallRun_},
        {"maxWheelSpinSpeed", _p.maxWheelSpinSpeed_},
        {"wheelTiltAngleRate", _p.wheelTiltAngleRate_},
        {"wheelTiltAngleMaxAccel", _p.wheelTiltAngleMaxAccel_}};
}

void from_json(const nlohmann::json& j, PlayerEffectControlParam& _p) {
    j.at("trailColorByGearLevel").get_to(_p.trailColorByGearLevel_);
    j.at("rotateOffsetOnWallRun").get_to(_p.rotateOffsetOnWallRun_);
    j.at("maxWheelSpinSpeed").get_to(_p.maxWheelSpinSpeed_);
    j.at("wheelTiltAngleRate").get_to(_p.wheelTiltAngleRate_);
    j.at("wheelTiltAngleMaxAccel").get_to(_p.wheelTiltAngleMaxAccel_);
}
