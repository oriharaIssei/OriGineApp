#include "FloorAndPillerSpawner.h"
/// externals
#include "imgui/imgui.h"
#include<string>

void FloorAndPillerSpawner::Initialize([[maybe_unused]] GameEntity* _entity) {
    columNum_ = 10;
}

bool FloorAndPillerSpawner::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::DragFloat("FieldRadius", &fallSpeed_, 0.01f);
    isChange |= ImGui::DragFloat("floorSpace", &floorSpace_, 0.01f);
    isChange |= ImGui::DragFloat("pillerSpace", &pillerSpace_, 0.01f);
    for (int i = 0; i < 10; ++i) {
        isChange |= ImGui::Checkbox(("IsAppearSafeZone" + std::to_string(i)).c_str(), &isAppearSafeZone_[i]);
    }
    return isChange;
}

void FloorAndPillerSpawner::Save(BinaryWriter& _writer) {
    _writer.Write(fallSpeed_);
    _writer.Write(pillerSpace_);
    _writer.Write(floorSpace_);
    for (int i = 0; i < 10; ++i) {
        _writer.Write(isAppearSafeZone_[i]);
    }
}

void FloorAndPillerSpawner::Load(BinaryReader& _reader) {
    _reader.Read(fallSpeed_);
    _reader.Read(pillerSpace_);
    _reader.Read(floorSpace_);
    for (int i = 0; i < 10; ++i) {
        _reader.Read(isAppearSafeZone_[i]);
    }
}

void FloorAndPillerSpawner::Finalize() {}

