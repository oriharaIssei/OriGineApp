#include "ShockWaveSpawner.h"

/// externals
#include "imgui/imgui.h"

void ShockWaveSpawner::Initialize([[maybe_unused]] GameEntity* _entity) {}

bool ShockWaveSpawner::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::DragFloat("MoveSpeed", &moveSpeed_, 0.01f);

    return isChange;
}

void ShockWaveSpawner::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    _writer.Write("moveSpeed", moveSpeed_);
}

void ShockWaveSpawner::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    _reader.Read("moveSpeed", moveSpeed_);
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
