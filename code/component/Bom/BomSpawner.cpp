#include "BomSpawner.h"

/// externals
#include "imgui/imgui.h"

void BomSpawner::Initialize([[maybe_unused]] GameEntity* _entity) {

    currentPutCoolTime_ = 0.0f;
    isLaunch_           = false;
    putTotalNum_        = 0;
}

bool BomSpawner::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::DragFloat("PutCoolTime", &putCoolTimeMax_);
    isChange |= ImGui::InputInt("SettingBomNum", &ableSetBomNum_);

    return isChange;
}

void BomSpawner::Finalize() {}

void to_json(nlohmann::json& _json, const BomSpawner& _bomSpawner) {
    _json["isAlive"]        = _bomSpawner.isAlive_;
    _json["ableSetBomNum"]  = _bomSpawner.ableSetBomNum_;
    _json["putCoolTimeMax"] = _bomSpawner.putCoolTimeMax_;
}
void from_json(const nlohmann::json& _json, BomSpawner& _bomSpawner) {
    _json.at("isAlive").get_to(_bomSpawner.isAlive_);
    _json.at("ableSetBomNum").get_to(_bomSpawner.ableSetBomNum_);
    _json.at("putCoolTimeMax").get_to(_bomSpawner.putCoolTimeMax_);
}
