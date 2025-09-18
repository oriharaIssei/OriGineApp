#include "PlayerState.h"

#include "PlayerIdleState.h"

/// externals
#include "myGui/MyGui.h"

PlayerState::PlayerState()
    : IComponent() {}

PlayerState::~PlayerState() {}

void PlayerState::Initialize(GameEntity* /*_entity*/) {
    moveStateEnum_    = PlayerMoveState::IDLE;
    preMoveStateEnum_ = PlayerMoveState::IDLE;
    moveState_        = nullptr;

    onGround_          = false;
    isGearUp_          = false;
    collisionWithWall_ = false;
    isGoal_            = false;

    wallCollisionNormal_ = {0.f, 0.f, 0.f};
    gearLevel_           = kDefaultPlayerGearLevel;
    gearUpCoolTime_      = 0.0f;
}

void PlayerState::Edit([[maybe_unused]] Scene* _scene, [[maybe_unused]] GameEntity* _entity, const std::string& _parentLabel) {
    static std::map<PlayerMoveState, const char*> moveStateName = {
        {PlayerMoveState::IDLE, "IDLE"},
        {PlayerMoveState::DASH, "DASH"},
        {PlayerMoveState::JUMP, "JUMP"},
        {PlayerMoveState::WALL_RUN, "WALL_RUN"},
        {PlayerMoveState::WALL_JUMP, "WALL_JUMP"}
        // {PlayerMoveState::SLIDE, "SLIDE"}
    };

    ImGui::Text("MoveState     : %s", moveStateName[moveStateEnum_.toEnum()]);
    ImGui::Text("Gear Level    : %d", gearLevel_);
    ImGui::Spacing();

    ImGui::Text("Is On Ground  : %s", onGround_ ? "true" : "false");
    ImGui::Text("Is Goal       : %s", isGoal_ ? "true" : "false");

    ImGui::Spacing();

    ImGui::Text("Is Gear Up        : %s", isGearUp_ ? "true" : "false");
    ImGui::Text("Gear Up Cool Time : %f", gearUpCoolTime_ );

    ImGui::Spacing();
    ImGui::Text("Wall Collision    : %s", collisionWithWall_ ? "true" : "false");
}

void PlayerState::Finalize() {}

void to_json(nlohmann::json& /*j*/, const PlayerState& /*p*/) {
    // 保存するものはない
}
void from_json(const nlohmann::json& /*j*/, PlayerState& /*p*/) {
    // 読み込むものはない
}
