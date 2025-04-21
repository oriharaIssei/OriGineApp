#include "BigBomSpawner.h"

/// externals
#include "imgui/imgui.h"

void BigBomSpawner::Initialize([[maybe_unused]] GameEntity* _entity) {

    currentPutCoolTime_ = 0.0f;
    isPut_              = false;
    putTotalNum_         = 0;
}

bool BigBomSpawner::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

  
    isChange |= ImGui::DragFloat("launthSpeed", &launthSpeed_); 
    isChange |= ImGui::DragFloat3("spawnOffset", spawnOffset_.v);
    ImGui::Text("collsion");
    isChange |= ImGui::DragFloat3("collisionCenter", collisionCenter_.v);
    isChange |= ImGui::DragFloat("collisionRadius", &collisionRadius_);
  
    return isChange;
}

void BigBomSpawner::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    _writer.Write("ableSetBomNum", ableSetBomNum_);
    _writer.Write("putCoolTimeMax", putCoolTimeMax_);
    _writer.Write<3,float>("spawnOffset", spawnOffset_);
    _writer.Write<3, float>("collisionCenter", collisionCenter_);
    _writer.Write("collisionRadius", collisionRadius_);
    _writer.Write("launthSpeed", launthSpeed_);
}

void BigBomSpawner::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    _reader.Read("ableSetBomNum", ableSetBomNum_);
    _reader.Read("putCoolTimeMax", putCoolTimeMax_);
    _reader.Read<3, float>("spawnOffset", spawnOffset_);
    _reader.Read<3, float>("collisionCenter", collisionCenter_);
    _reader.Read("collisionRadius", collisionRadius_);
    _reader.Read("launthSpeed", launthSpeed_);
}

void BigBomSpawner::Finalize() {}
