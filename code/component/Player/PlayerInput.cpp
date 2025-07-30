#include "PlayerInput.h"

/// externals
#ifdef _DEBUG
#include <myGui/MyGui.h>
#endif // _DEBUG

void PlayerInput::Initialize(GameEntity* /* _entity*/) {
}

void PlayerInput::Edit(Scene* /*_scene*/, GameEntity* /*_entity*/, [[maybe_unused]] const std::string& _parentLabel) {

#ifdef _DEBUG
    DragGuiCommand("MaxJumpInputTime##" + _parentLabel, maxJumpTime_, 0.01f, 0.01f);
#endif // _DEBUG
}

void PlayerInput::Finalize() {
    // 何もしない
}

void to_json(nlohmann::json& j, const PlayerInput& _input) {
    j["MaxJumpInputTime"] = _input.maxJumpTime_;
}

void from_json(const nlohmann::json& j, PlayerInput& _input) {
    _input.maxJumpTime_ = j["MaxJumpInputTime"].get<float>();
}
