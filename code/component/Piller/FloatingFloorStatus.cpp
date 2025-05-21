#include "FloatingFloorStatus.h"
/// externals
#include "imgui/imgui.h"
#include "KetaShake.h"
#include "myGui/MyGui.h"

void FloatingFloorStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
    fallPosY_  = 0.0f;
    fallEaseT_ = 0.0f;
    fallspeed_ = 3.0f;
    currentHP_ = 3;
}

bool FloatingFloorStatus::Edit() {
    bool isChange = false;

#ifdef _DEBUG

    isChange = CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();

    isChange |= DragGuiCommand("fallspeed", fallspeed_, 0.01f);

#endif // _DEBUG

    return isChange;
}

void FloatingFloorStatus::Finalize() {}

void FloatingFloorStatus::SetColumAndRow(const int32_t& colum, const int32_t& row) {
    rowNum_   = row;
    columNum_ = colum;
}

void FloatingFloorStatus::SetColumDecrement() {
    columNum_--;
}

void FloatingFloorStatus::TakeDamage() {
    currentHP_--;
}

void FloatingFloorStatus::TakeBigDamage() {

    currentHP_ = 0;
}

void FloatingFloorStatus::ChangeDamageShake() {
    isDamageShake_ = true;
}

void FloatingFloorStatus::DamageShake(const float& shakeValue, const float& deltaTime) {
    damageShakePos_[X] = Shake<float>(shakeTime_, shakeValue);
    shakeTime_ -= deltaTime;

    if (shakeTime_ > 0.0f) {
        return;
    }
    //リセット
    shakeTime_      = 0.0f;
    damageShakePos_ = {0.0f, 0.0f, 0.0f};
    reactionStep_   = ReactionStep::CONSTANTSHAKE;
}

void FloatingFloorStatus::ConstantShake(const float& shakeValue) {
    damageShakePos_[X] = ShakeWave<float>(1.0f, shakeValue);
}

void FloatingFloorStatus::DamageShakeReset(FloatingFloorAnimationStatus* animestatus) {
    damageShakePos_ = {0.0f, 0.0f, 0.0f};
    shakeTime_      = animestatus->GetShakeMaxTime();
}

void FloatingFloorStatus::RevivalReset() {
    isDestroy_           = false;
    currentRevivalTimer_ = 0.0f;
    currentHP_           = HPMax_;
    isFall_              = false;
    isRevivaling_        = false;
    scoreUpRate_         = startScoreUpRate_;
    isDamageShake_       = false;
}

void to_json(nlohmann::json& _json, const FloatingFloorStatus& _block) {
    _json["isAlive"]   = _block.isAlive_;
    _json["fallspeed"] = _block.fallspeed_;
}

void from_json(const nlohmann::json& _json, FloatingFloorStatus& _block) {
    _json.at("isAlive").get_to(_block.isAlive_);
    _json.at("fallspeed").get_to(_block.fallspeed_);
}
