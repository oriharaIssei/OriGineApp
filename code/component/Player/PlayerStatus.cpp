#include "PlayerStatus.h"

/// externals
#ifdef _DEBUG
#include "myGui/MyGui.h"
#include <imgui/imgui.h>
#endif // _DEBUG

void PlayerStatus::Initialize(GameEntity* /*_entity*/) {
}

bool PlayerStatus::Edit() {
#ifdef _DEBUG

    bool isChange = false;

    isChange |= DragGuiCommand("baseSpeed", baseSpeed_);
    isChange |= DragGuiCommand("gearUpCoolTime", baseGearupCoolTime_);
    isChange |= DragGuiCommand("directionInterpolateRate", directionInterpolateRate_);

    return isChange;
#else
    return false;
#endif // _DEBUG
}

void PlayerStatus::Debug() {
#ifdef _DEBUG

    static std::map<MoveState, const char*> moveStateName = {
        {MoveState::IDLE, "IDLE"},
        {MoveState::DASH, "DASH"},
        {MoveState::JUMP, "JUMP"},
        {MoveState::SLIDE, "SLIDE"}};

    ImGui::Text("MoveState: %s", moveStateName[moveState_]);
    ImGui::Text("Gear Level: %d", gearLevel_);

    ImGui::Text("Base Gear Up Cool Time: %.2f", baseGearupCoolTime_);
    ImGui::Text("Gear Up Cool Time: %.2f", gearUpCoolTime_);

    ImGui::Text("Base Speed: %.2f", baseSpeed_);
    ImGui::Text("Direction Interpolate Rate: %.2f", directionInterpolateRate_);

#endif
}

void PlayerStatus::Finalize() {}

void to_json(nlohmann::json& j, const PlayerStatus& _playerStatus) {
    j["baseSpeed"]                = _playerStatus.baseSpeed_;
    j["gearUpCoolTime"]           = _playerStatus.baseGearupCoolTime_;
    j["directionInterpolateRate"] = _playerStatus.directionInterpolateRate_;
}
void from_json(const nlohmann::json& j, PlayerStatus& _playerStatus) {
    j.at("baseSpeed").get_to(_playerStatus.baseSpeed_);
    j.at("gearUpCoolTime").get_to(_playerStatus.baseGearupCoolTime_);
    j.at("directionInterpolateRate").get_to(_playerStatus.directionInterpolateRate_);
}
