#include "PlayerState.h"

#include "PlayerIdleState.h"

/// externals
#include "myGui/MyGui.h"

PlayerState::PlayerState()
    : IComponent() {}

PlayerState::~PlayerState() {}

void PlayerState::Initialize(Entity* /*_entity*/) {
    moveStateEnum_    = PlayerMoveState::IDLE;
    preMoveStateEnum_ = PlayerMoveState::IDLE;
    moveState_        = nullptr;

    onGround_          = false;
    IsGearUp_          = false;
    collisionWithWall_ = false;
    isGoal_            = false;

    wallCollisionNormal_ = {0.f, 0.f, 0.f};
    gearLevel_           = kDefaultPlayerGearLevel;
    gearUpCoolTime_      = 0.0f;
}

void PlayerState::Edit([[maybe_unused]] Scene* _scene, [[maybe_unused]] Entity* _entity, [[maybe_unused]] const std::string& _parentLabel) {
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

    ImGui::Text("Is Gear Up        : %s", IsGearUp_ ? "true" : "false");
    ImGui::Text("Gear Up Cool Time : %f", gearUpCoolTime_);

    ImGui::Spacing();
    ImGui::Text("Wall Collision    : %s", collisionWithWall_ ? "true" : "false");
}

void PlayerState::Finalize() {}

void PlayerState::OnCollisionWall(const Vec3f& _collisionNormal, int32_t _entityIndex) {
    collisionWithWall_   = true;
    wallCollisionNormal_ = _collisionNormal;
    wallEntityIndex_     = _entityIndex;
}

void PlayerState::OffCollisionWall() {
    collisionWithWall_   = false;
    wallCollisionNormal_ = {0.f, 0.f, 0.f};
    // wallEntityIndex_     = -1; // 保持しておく
}

void PlayerState::OnCollisionGround(int32_t _entityIndex) {
    onGround_             = true;
    lastFloorEntityIndex_ = _entityIndex;
}
void PlayerState::OffCollisionGround() {
    onGround_ = false;
}

void PlayerState::SetPlayerMoveState(std::shared_ptr<IPlayerMoveState> _playerMoveState) {
    moveState_     = _playerMoveState;
    moveStateEnum_ = moveState_->GetState();
}

void to_json(nlohmann::json& /*j*/, const PlayerState& /*p*/) {
    // 保存するものはない
}
void from_json(const nlohmann::json& /*j*/, PlayerState& /*p*/) {
    // 読み込むものはない
}
