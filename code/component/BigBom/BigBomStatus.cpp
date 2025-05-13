#include "BigBomStatus.h"

/// externals
#include "imgui/imgui.h"
#include "myGui/MyGui.h"

void BigBomStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
    currentTime_ = 0.0f;
   
}

bool BigBomStatus::Edit() {
    bool isChange = false;

    isChange = CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();

      ImGui::Text("Offset");
  
      ImGui::Text("etc");
   
   

    return isChange;
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

void to_json(nlohmann::json& j, const BigBomStatus& b) {
    j["isAlive"]         = b.isAlive_;
}
void from_json(const nlohmann::json& j, BigBomStatus& b) {
    j.at("isAlive").get_to(b.isAlive_);
}
