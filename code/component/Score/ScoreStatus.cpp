#include "ScoreStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"



void ScoreStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
    
}

bool ScoreStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

   
    isChange |= ImGui::InputInt("currentScore", &currentScore_);
    isChange |= ImGui::DragFloat("pulusScore", &pulusScore_);
  

  
 
    return isChange;

}

void ScoreStatus::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    _writer.Write("currentScore", currentScore_);
    _writer.Write("pulusScore", pulusScore_);
   
}

void ScoreStatus::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    _reader.Read("currentScore", currentScore_);
    _reader.Read("pulusScore", pulusScore_);
}

void ScoreStatus::Finalize() {}

