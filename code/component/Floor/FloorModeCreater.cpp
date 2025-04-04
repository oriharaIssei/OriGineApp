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

  
    /*for (int i = 0; i < 10; ++i) {
        isChange |= ImGui::Checkbox(("IsAppearSafeZone" + std::to_string(i)).c_str(), &isAppearSafeZone_[i]);
    }*/
    return isChange;
}

void FloorModeCreater::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
   /* _writer.Write("isAppearSafeZone", isAppearSafeZone_);*/
   /* for (int i = 0; i < 10; ++i) {
        _writer.Write("isAppearSafeZone" isAppearSafeZone_[i]);
    }*/
}

void FloorModeCreater::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    /*for (int i = 0; i < 10; ++i) {
        _reader.Read(isAppearSafeZone_[i]);
    }*/
}

void FloorModeCreater::Finalize() {}

