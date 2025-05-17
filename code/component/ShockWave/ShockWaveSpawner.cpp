#include "ShockWaveSpawner.h"

/// externals
#include "imgui/imgui.h"
#include "myGui/MyGui.h"

void ShockWaveSpawner::Initialize([[maybe_unused]] GameEntity* _entity) {}

bool ShockWaveSpawner::Edit() {
    bool isChange = false;

#ifdef _DEBUG

    isChange = CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();

    isChange |= DragGuiCommand("MoveSpeed", moveSpeed_, 0.01f);

#endif // _DEBUG

    return isChange;
}

void ShockWaveSpawner::Finalize() {}

/// setter
void ShockWaveSpawner::SetDirection(const Direction& _direction) {

    switch (_direction) {
    case Direction::Right:

        direction_ = 1.0f;
        break;

    case Direction::Left:

        direction_ = -1.0f;
        break;

    default:
        direction_ = 0.0f;
        break;
    }
}

void to_json(nlohmann::json& _json, const ShockWaveSpawner& _s) {
    _json["isAlive"]   = _s.isAlive_;
    _json["moveSpeed"] = _s.moveSpeed_;
}

void from_json(const nlohmann::json& _json, ShockWaveSpawner& _s) {
    _json.at("isAlive").get_to(_s.isAlive_);
    _json.at("moveSpeed").get_to(_s.moveSpeed_);
}
