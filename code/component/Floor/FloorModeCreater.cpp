#include "FloorModeCreater.h"
/// externals
#include "imgui/imgui.h"
#include<string>

void FloorModeCreater::Initialize([[maybe_unused]] GameEntity* _entity) {
   
}

bool FloorModeCreater::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

  
    for (int i = 0; i < 10; ++i) {
        isChange |= ImGui::Checkbox(("IsAppearSafeZone" + std::to_string(i)).c_str(), &isAppearSafeZone_[i]);
    }
    return isChange;
}

void FloorModeCreater::Save(BinaryWriter& _writer) {
   
    for (int i = 0; i < 10; ++i) {
        _writer.Write(isAppearSafeZone_[i]);
    }
}

void FloorModeCreater::Load(BinaryReader& _reader) {
  
    for (int i = 0; i < 10; ++i) {
        _reader.Read(isAppearSafeZone_[i]);
    }
}

void FloorModeCreater::Finalize() {}

