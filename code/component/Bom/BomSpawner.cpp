#include "BomSpawner.h"

/// externals
#include "imgui/imgui.h"

void BomSpawner::Initialize([[maybe_unused]] GameEntity* _entity) {

    currentPutCoolTime_ = 0.0f;
    isPut_              = false;
    puttingNum_         = 0;
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
    _writer.Write("isAlive", isAlive_);
    _writer.Write("ableSetBomNum", ableSetBomNum_);
    _writer.Write("putCoolTimeMax", putCoolTimeMax_);
  
}

void BomSpawner::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    _reader.Read("ableSetBomNum", ableSetBomNum_);
    _reader.Read("putCoolTimeMax", putCoolTimeMax_);
    

}

void BomSpawner::Finalize() {}
