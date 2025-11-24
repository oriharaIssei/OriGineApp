#include "PlayerState.h"

#include "PlayerIdleState.h"

/// externals
#include "myGui/MyGui.h"

PlayerState::PlayerState()
    : IComponent() {}

PlayerState::~PlayerState() {}

void PlayerState::Initialize(Entity* /*_entity*/) {
    // moveStateEnum の 初期化
    // current,prev を IDLE に設定
    moveStateEnum_.Set(PlayerMoveState::IDLE);
    moveStateEnum_.Set(PlayerMoveState::IDLE);
    moveState_ = nullptr;

    // stateFlag の 初期化
    stateFlag_.Set(EnumBitmask<PlayerStateFlag>(0));
    stateFlag_.Set(EnumBitmask<PlayerStateFlag>(0));

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

    ImGui::Text("MoveState     : %s", moveStateName[moveStateEnum_.Current().ToEnum()]);
    ImGui::Text("Gear Level    : %d", gearLevel_);
    ImGui::Spacing();

    ImGui::Text("Is On Ground  : %s", stateFlag_.Current() | PlayerStateFlag::ON_GROUND ? "true" : "false");
    ImGui::Text("Is Goal       : %s", stateFlag_.Current() | PlayerStateFlag::IS_GOAL ? "true" : "false");

    ImGui::Spacing();

    ImGui::Text("Is Gear Up        : %s", stateFlag_.Current() | PlayerStateFlag::GEAR_UP ? "true" : "false");
    ImGui::Text("Gear Up Cool Time : %f", gearUpCoolTime_);

    ImGui::Spacing();
    ImGui::Text("Wall Collision    : %s", stateFlag_.Current() | PlayerStateFlag::ON_WALL ? "true" : "false");
}

void PlayerState::Finalize() {}

void PlayerState::OnCollisionWall(const Vec3f& _collisionNormal, int32_t _entityIndex) {
    stateFlag_.SetCurrent(stateFlag_.Current() | PlayerStateFlag::ON_WALL);
    wallCollisionNormal_ = _collisionNormal;
    wallEntityIndex_     = _entityIndex;
}

void PlayerState::OffCollisionWall() {
    stateFlag_.SetCurrent(stateFlag_.Current() | ~static_cast<int32_t>(PlayerStateFlag::ON_WALL));
    wallCollisionNormal_ = {0.f, 0.f, 0.f};
    // wallEntityIndex_     = -1; // 保持しておく
}

void PlayerState::OnCollisionGround(int32_t _entityIndex) {
    stateFlag_.SetCurrent(stateFlag_.Current() | PlayerStateFlag::ON_GROUND);
    lastFloorEntityIndex_ = _entityIndex;
}
void PlayerState::OffCollisionGround() {
    stateFlag_.SetCurrent(stateFlag_.Current() | ~static_cast<int32_t>(PlayerStateFlag::ON_GROUND));
}

void PlayerState::SetPlayerMoveState(std::shared_ptr<IPlayerMoveState> _playerMoveState) {
    moveState_ = _playerMoveState;
}

void to_json(nlohmann::json& /*j*/, const PlayerState& /*p*/) {
    // 保存するものはない
}
void from_json(const nlohmann::json& /*j*/, PlayerState& /*p*/) {
    // 読み込むものはない
}
