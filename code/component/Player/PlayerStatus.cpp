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

void PlayerStatus::Save(BinaryWriter& _writer) {
    _writer.Write("baseSpeed", dashState_.baseSpeed_);
    _writer.Write("gearUpCoolTime", dashState_.gearUpCoolTime_);
    _writer.Write("directionInterpolateRate", directionInterpolateRate_);
}

void PlayerStatus::Load(BinaryReader& _reader) {
    _reader.Read("baseSpeed", dashState_.baseSpeed_);
    _reader.Read("gearUpCoolTime", dashState_.gearUpCoolTime_);
    _reader.Read("directionInterpolateRate", directionInterpolateRate_);
}

void PlayerStatus::Finalize() {
}
