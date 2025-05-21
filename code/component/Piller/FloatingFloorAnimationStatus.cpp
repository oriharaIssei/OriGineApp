#include "FloatingFloorAnimationStatus.h"
/// externals
#include "imgui/imgui.h"
#include "myGui/MyGui.h"
#include <string>

void FloatingFloorAnimationStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
    /// 初期化でパラメータ編集してるから大丈夫、ここ消したら未定義値が出る
   
}

bool FloatingFloorAnimationStatus::Edit() {
    bool isChange = false;

#ifdef _DEBUG

    isChange = CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();
   
    isChange |= DragGuiCommand("shakeValue_", shakeValue_, 0.01f);
    isChange |= DragGuiCommand("shakeMaxTime_", shakeMaxTime_, 0.01f);

#endif // _DEBUG

    return isChange;
}

void FloatingFloorAnimationStatus::Finalize() {}

void to_json(nlohmann::json& _json, const FloatingFloorAnimationStatus& _block) {
    _json["shakeValue_"] = _block.shakeValue_;
    _json["shakeMaxTime_"] = _block.shakeMaxTime_;
}

void from_json(const nlohmann::json& _json, FloatingFloorAnimationStatus& _block) {
    _json.at("shakeValue_").get_to(_block.shakeValue_);
    _json.at("shakeMaxTime_").get_to(_block.shakeMaxTime_);
}
