#include "TimerStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"



void TimerStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
    
}

bool TimerStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

   
    isChange |= ImGui::DragFloat("startTimer", &currentTimer_, 0.01f);
    isChange |= ImGui::DragFloat("pulusTime", &pulusTime_, 0.01f);
    isChange |= ImGui::DragFloat("minusTime", &minusTime_, 0.01f);
 
    return isChange;

}

void TimerStatus::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    _writer.Write("pulusTime", pulusTime_);
    _writer.Write("minusTime", minusTime_);
   
}

void TimerStatus::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    _reader.Read("pulusTime", pulusTime_);
    _reader.Read("minusTime", minusTime_);
   
}

void TimerStatus::Finalize() {}

void TimerStatus::DecrementTimer() {
    currentTimer_ -= Engine::getInstance()->getDeltaTime();
    if (currentTimer_ < 0.0f) {
        currentTimer_ = 0.0f;
    }
}
