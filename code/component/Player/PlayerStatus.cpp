#include "PlayerStatus.h"

/// externals
#ifdef _DEBUG
#include <imgui/imgui.h>
#endif // _DEBUG

void PlayerStatus::Initialize(GameEntity* /*_entity*/) {
}

bool PlayerStatus::Edit() {
#ifdef _DEBUG
    static std::map<MoveState, const char*> moveStateName = {
        {MoveState::IDLE, "IDLE"},
        {MoveState::MOVE, "MOVE"},
        {MoveState::JUMP, "JUMP"}};

    bool isChange = false;

    ImGui::Text("MoveState : %s", moveStateName[moveState_]);
    ImGui::Text("isJump    : %d", isJump_);
    ImGui::Text("canJump   : %d", canJump_);
    isChange |= ImGui::InputFloat("baseSpeed", &dashState_.baseSpeed_);
    isChange |= ImGui::InputFloat("gearUpCoolTime", &dashState_.gearUpCoolTime_);
    isChange |= ImGui::InputFloat("directionInterpolateRate", &directionInterpolateRate_);

    return isChange;
#else
    return false;
#endif // _DEBUG
}

void PlayerStatus::Finalize() {
}

void to_json(nlohmann::json& j, const PlayerStatus& _playerStatus) {
    j["baseSpeed"]                = _playerStatus.dashState_.baseSpeed_;
    j["gearUpCoolTime"]           = _playerStatus.dashState_.gearUpCoolTime_;
    j["directionInterpolateRate"] = _playerStatus.directionInterpolateRate_;
}
void from_json(const nlohmann::json& j, PlayerStatus& _playerStatus) {
    j.at("baseSpeed").get_to(_playerStatus.dashState_.baseSpeed_);
    j.at("gearUpCoolTime").get_to(_playerStatus.dashState_.gearUpCoolTime_);
    j.at("directionInterpolateRate").get_to(_playerStatus.directionInterpolateRate_);
}
