#include "PlayerInput.h"

/// externals
#ifdef _DEBUG
#include <imgui/imgui.h>
#endif // _DEBUG

void PlayerInput::Initialize(GameEntity* /* _entity*/) {
}

bool PlayerInput::Edit() {
    return false;
}

void PlayerInput::Finalize() {
    // 何もしない
}

void to_json(nlohmann::json& , const PlayerInput& ) {
}

void from_json(const nlohmann::json& , PlayerInput& ) {
}
