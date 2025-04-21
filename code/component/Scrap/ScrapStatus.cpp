#include "ScrapStatus.h"

/// externals
#include "imgui/imgui.h"

void ScrapStatus::Initialize([[maybe_unused]] GameEntity* _entity) {}

bool ScrapStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    return isChange;
}

void ScrapStatus::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
}

void ScrapStatus::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
}

void ScrapStatus::Finalize() {}

void ScrapStatus::LifeTimeDecrement(const float& decrementValue) {
    lifeTime_ -= decrementValue;
}
