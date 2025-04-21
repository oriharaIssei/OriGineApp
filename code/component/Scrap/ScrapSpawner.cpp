#include "ScrapSpawner.h"

/// externals
#include "imgui/imgui.h"

void ScrapSpawner::Initialize([[maybe_unused]] GameEntity* _entity) {}

bool ScrapSpawner::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);
  
    ImGui::Spacing();

      isChange |= ImGui::DragFloat("LifeTime", &lifeTime_, 0.1f);
    isChange |= ImGui::DragFloat("FallStopPosY", &fallStopPosY_, 0.1f);
    isChange |= ImGui::DragFloat2("blowValue", blowValue_.v, 0.01f);

    return isChange;
}

void ScrapSpawner::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    _writer.Write<2, float>("blowValue", blowValue_);
    _writer.Write("lifeTime", lifeTime_);
    _writer.Write("fallStopPosY", fallStopPosY_);
    
}

void ScrapSpawner::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    _reader.Read<2, float>("blowValue", blowValue_);
    _reader.Read("lifeTime", lifeTime_);
    _reader.Read("fallStopPosY", fallStopPosY_);
}

void ScrapSpawner::Finalize() {}

