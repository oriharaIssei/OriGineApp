#include "FloatingFloorStatus.h"
/// externals
#include "imgui/imgui.h"
#include <string>

void FloatingFloorStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
    fallPosY_  = 0.0f;
    fallEaseT_ = 0.0f;
    fallspeed_ = 3.0f;
    currentHP_ = 3;
    /* collisionSize_ = 5.0f;*/
}

bool FloatingFloorStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::DragFloat("fallspeed", &fallspeed_, 0.01f);

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

void FloatingFloorStatus::RevivalReset() {
    isDestroy_           = false;
    currentRevivalTimer_ = 0.0f;
    currentHP_           = HPMax_;
    isFall_              = false;
    isRevivaling_        = false;
    scoreUpRate_        = startScoreUpRate_;
  
}

void to_json(nlohmann::json& _json, const FloatingFloorStatus& _block) {
    _json["isAlive"]   = _block.isAlive_;
    _json["fallspeed"] = _block.fallspeed_;
}

void from_json(const nlohmann::json& _json, FloatingFloorStatus& _block) {
    _json.at("isAlive").get_to(_block.isAlive_);
    _json.at("fallspeed").get_to(_block.fallspeed_);
}
