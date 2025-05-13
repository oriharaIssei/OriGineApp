#include "TutorialMenuStatus.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"
#include "myGui/MyGui.h"

void TutorialMenuStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool TutorialMenuStatus::Edit() {
    bool isChange = false;

    isChange = CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();

   

    isChange |= DragGuiVectorCommand("offsetPos", offsetPos_);
    isChange |= DragGuiVectorCommand("textureSize", textureSize_);

    return isChange;
}


void to_json(nlohmann::json& j, const TutorialMenuStatus& m) {
    j["isAlive"]      = m.isAlive_;
  /*  j["levelUIDigit"] = static_cast<int32_t>(m.uiDigit_);*/
    j["offsetPos"]    = m.offsetPos_;
    j["textureSize"]  = m.textureSize_;
}

void from_json(const nlohmann::json& j, TutorialMenuStatus& m) {
    j.at("isAlive").get_to(m.isAlive_);
   /* int32_t digit = 0;*/
  /*  j.at("levelUIDigit").get_to(digit);
    m.uiDigit_ = static_cast<TutorialPauge>(digit);*/
    j.at("offsetPos").get_to(m.offsetPos_);
    j.at("textureSize").get_to(m.textureSize_);
}


void TutorialMenuStatus::Finalize() {}


