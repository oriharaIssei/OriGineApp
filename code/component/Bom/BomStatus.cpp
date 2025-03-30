#include "BomStatus.h"

/// externals
#include "imgui/imgui.h"

void BomStatus::Initialize([[maybe_unused]] GameEntity* _entity) {}

bool BomStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

      ImGui::Text("Offset");
    isChange |= ImGui::InputFloat3("##BomOffset", positionOffset_.v);

      ImGui::Text("etc");
    isChange |= ImGui::DragFloat("explotionTime", &explotionTime_);
    isChange |= ImGui::DragFloat("CollisionRadius", &collisionRadius_);

    return isChange;
}

void BomStatus::Save(BinaryWriter& _writer) {
    _writer.Write(isAlive_);
    _writer.Write(explotionTime_);
    _writer.Write<3, float>(positionOffset_);
}

void BomStatus::Load(BinaryReader& _reader) {
    _reader.Read(isAlive_);
    _reader.Read(explotionTime_);
    _reader.Read<3, float>(positionOffset_);
}

void BomStatus::Finalize() {}
