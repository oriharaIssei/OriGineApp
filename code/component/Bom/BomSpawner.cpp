#include "BomSpawner.h"

/// externals
#include "imgui/imgui.h"

void BomSpawner::Initialize([[maybe_unused]] GameEntity* _entity) {

    currentPutCoolTime_ = 0.0f;
    isPut_              = false;
}

bool BomSpawner::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::DragFloat("PutCoolTime", &putCoolTimeMax_);
    isChange |= ImGui::InputInt ("SettingBomNum", &ableSetBomNum_);

    return isChange;
}

void BomSpawner::Save(BinaryWriter& _writer) {
    _writer.Write(isAlive_);
    _writer.Write(ableSetBomNum_);
    _writer.Write(putCoolTimeMax_);
  
}

void BomSpawner::Load(BinaryReader& _reader) {
    _reader.Read(isAlive_);
    _reader.Read(ableSetBomNum_);
    _reader.Read(putCoolTimeMax_);
    

}

void BomSpawner::Finalize() {}
