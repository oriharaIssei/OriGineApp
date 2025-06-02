#include "ScaleByBlockStatus.h"

/// externals
#include "imgui/imgui.h"
#include "myGui/MyGui.h"

void ScaleByBlockStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
}

bool ScaleByBlockStatus::Edit() {
    bool isChange = false;

#ifdef _DEBUG

    isChange = CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();

    isChange |= DragGuiVectorCommand("scaleOffset", scaleOffset_);
    isChange |= DragGuiVectorCommand("uvScrollSpeed_", uvScrollSpeed_);
    isChange |= CheckBoxCommand("isUVScroll_", isUVScroll_);

#endif

    return isChange;
}

void ScaleByBlockStatus::UVScrollUPdate(const float& deltaTime) {
    if (!isUVScroll_) {
        return;
    }

    uvPos_[X] += uvScrollSpeed_[X] * deltaTime;
    uvPos_[Y] += uvScrollSpeed_[Y] * deltaTime;
}

void ScaleByBlockStatus::Finalize() {}

void to_json(nlohmann::json& _json, const ScaleByBlockStatus& _bomSpawner) {
    _json["isAlive"]        = _bomSpawner.isAlive_;
    _json["scaleOffset"]    = _bomSpawner.scaleOffset_;
    _json["uvScrollSpeed_"] = _bomSpawner.uvScrollSpeed_;
    _json["isUVScroll"]    = _bomSpawner.isUVScroll_;
}
void from_json(const nlohmann::json& _json, ScaleByBlockStatus& _bomSpawner) {
    _json.at("isAlive").get_to(_bomSpawner.isAlive_);
    _json.at("scaleOffset").get_to(_bomSpawner.scaleOffset_);

    if (auto it = _json.find("isUVScroll"); it != _json.end()) {
        _json.at("isUVScroll").get_to(_bomSpawner.isUVScroll_);
    }
    if (auto it = _json.find("uvScrollSpeed_"); it != _json.end()) {
        _json.at("uvScrollSpeed_").get_to(_bomSpawner.uvScrollSpeed_);
    }
}
