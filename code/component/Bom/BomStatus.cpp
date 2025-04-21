#include "BomStatus.h"

/// externals
#include "imgui/imgui.h"

void BomStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
    currentTime_ = 0.0f;
    explotionTime_   = 2.0f;
    collisionRadius_ = 2.0f;
    positionOffset_  = Vec3f(0.0f, 0.0f, 0.0f);
    launthSpeed_     = 14.0f;
}

bool BomStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

      ImGui::Text("Offset");
    isChange |= ImGui::InputFloat3("##BomOffset", positionOffset_.v);

      ImGui::Text("etc");
    isChange |= ImGui::DragFloat("explotionTime", &explotionTime_);
    isChange |= ImGui::DragFloat("CollisionRadius", &collisionRadius_);
    isChange |= ImGui::DragFloat("launthSpeed", &launthSpeed_,0.1f);

    return isChange;
}

void BomStatus::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    _writer.Write("explotionTime", explotionTime_);
    _writer.Write<3, float>("positionOffset", positionOffset_);
    _writer.Write("CollisionRadius", collisionRadius_);
    _writer.Write("launthSpeed", launthSpeed_);
}

void BomStatus::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    _reader.Read("explotionTime", explotionTime_);
    _reader.Read<3, float>("positionOffset", positionOffset_);
    _reader.Read("CollisionRadius", collisionRadius_);
    _reader.Read("launthSpeed", launthSpeed_);
}

void BomStatus::Finalize() {}
