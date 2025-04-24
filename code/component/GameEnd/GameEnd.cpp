#include "GameEnd.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"

void GameEnd::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool GameEnd::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    return isChange;
}

void GameEnd::Save(BinaryWriter& _writer) {
    _writer.Write("isAlive", isAlive_);
    /* _writer.Write("timerDigit", static_cast<int32_t>(digit_));*/
}

void GameEnd::Load(BinaryReader& _reader) {
    _reader.Read("isAlive", isAlive_);
    /*  int32_t digit = 0;
      _reader.Read("timerDigit", digit);
      digit_ = static_cast<UIDigit>(digit);*/
}


void GameEnd::Finalize() {

}
