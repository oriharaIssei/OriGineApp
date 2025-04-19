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

void PlayerInput::Save(BinaryWriter& /*_writer*/) {
}

void PlayerInput::Load(BinaryReader& /* _reader*/) {
}

void PlayerInput::Finalize() {
    // 何もしない
}
