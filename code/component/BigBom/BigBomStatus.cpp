#include "BigBomStatus.h"

/// externals
#include "imgui/imgui.h"

void BigBomStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
    currentTime_ = 0.0f;
   
}

bool BigBomStatus::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

      ImGui::Text("Offset");
  
      ImGui::Text("etc");
   
   

    return isChange;
}

void BigBomStatus::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_); 
    
}

void BigBomStatus::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
   
}

void BigBomStatus::Finalize() {}

void BigBomStatus::IncrementLaunchDirectin(const float& value) {
    launchDirection_[X] += value;
    launchDirection_[Y] += value;
    launchDirection_[Z] += value;
   }

void BigBomStatus::LaunchDirectionNormalize() {
       launchDirection_.normalize();
}
