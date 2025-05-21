#include "FloatingFloorStatus.h"
/// externals
#include "imgui/imgui.h"
#include "myGui/MyGui.h"
#include <string>
#include "KetaShake.h"

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

void FloatingFloorStatus::ChangeReactionMode() {
    switch (reactionStep_) {
    case ReactionStep::NONE:
        reactionStep_ = ReactionStep::DAMAGESHAKE;
        break;
    case ReactionStep::CONSTANTSHAKE:
        reactionStep_ = ReactionStep::CHANGEFALLSTATE;
        break;
    case ReactionStep::END:
        reactionStep_ = ReactionStep::DAMAGESHAKE;
        break;
    default:
        break;
    }
}

void FloatingFloorStatus::DamageShake(FloatingFloorAnimationStatus* animestatus, const float& deltaTime) {
    damageShakePos_ = ShakeWave<Vec3f>(shakeTime_, animestatus->GetShakeValue());
    shakeTime_ -= deltaTime;

    if (shakeTime_ > 0.0f) {
        return;
    }

    shakeTime_ = 0.0f;
    damageShakePos_ = {0.0f, 0.0f, 0.0f};
    reactionStep_   = ReactionStep::CONSTANTSHAKE;
}

void FloatingFloorStatus::DamageShakeReset(FloatingFloorAnimationStatus* animestatus) {
    damageShakePos_ = {0.0f, 0.0f, 0.0f};
    shakeTime_ = animestatus->GetShakeMaxTime();
 }

void FloatingFloorStatus::RevivalReset() {
    isDestroy_           = false;
    currentRevivalTimer_ = 0.0f;
    currentHP_           = HPMax_;
    isFall_              = false;
    isRevivaling_        = false;
    scoreUpRate_         = startScoreUpRate_;
}

void to_json(nlohmann::json& _json, const FloatingFloorStatus& _block) {
    _json["isAlive"]   = _block.isAlive_;
    _json["fallspeed"] = _block.fallspeed_;
}

void from_json(const nlohmann::json& _json, FloatingFloorStatus& _block) {
    _json.at("isAlive").get_to(_block.isAlive_);
    _json.at("fallspeed").get_to(_block.fallspeed_);
}
