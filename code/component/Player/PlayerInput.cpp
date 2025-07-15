#include "PlayerInput.h"

/// externals
#ifdef _DEBUG
#include <myGui/MyGui.h>
#endif // _DEBUG

void PlayerInput::Initialize(GameEntity* /* _entity*/) {
}

bool PlayerInput::Edit() {
    bool isChanged = false;

#ifdef _DEBUG
    isChanged |= DragGuiCommand("MaxJumpInputTime", maxJumpTime_, 0.01f, 0.01f);
#endif // _DEBUG

    return isChanged;
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
