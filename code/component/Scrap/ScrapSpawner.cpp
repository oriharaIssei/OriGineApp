#include "ScrapSpawner.h"

/// externals
#include "imgui/imgui.h"

void ScrapSpawner::Initialize([[maybe_unused]] GameEntity* _entity) {}

bool ScrapSpawner::Edit() {
    bool isChange = false;

    isChange = CheckBoxCommand("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::InputInt("createNum", &createNum_);
    isChange |= DragGuiCommand("LifeTime", &lifeTime_, 0.1f);
    isChange |= DragGuiCommand("FallStopPosY", &fallStopPosY_, 0.1f);
    isChange |= ImGui::DragFloat2("blowValue", blowValue_.v, 0.01f);
    isChange |= DragGuiCommand("mass", &mass_, 0.01f);
    isChange |= DragGuiCommand("colliderRadius", &colliderRadius_, 0.01f);
    return isChange;
}

void ScrapSpawner::Finalize() {}

void to_json(nlohmann::json& _json, const ScrapSpawner& _component) {
    _json["isAlive"]        = _component.isAlive_;
    _json["blowValue"]      = _component.blowValue_;
    _json["lifeTime"]       = _component.lifeTime_;
    _json["fallStopPosY"]   = _component.fallStopPosY_;
    _json["createNum"]      = _component.createNum_;
    _json["mass"]           = _component.mass_;
    _json["colliderRadius"] = _component.colliderRadius_;
}

void from_json(const nlohmann::json& _json, ScrapSpawner& _component) {
    _json.at("isAlive").get_to(_component.isAlive_);
    _json.at("blowValue").get_to(_component.blowValue_);
    _json.at("lifeTime").get_to(_component.lifeTime_);
    _json.at("fallStopPosY").get_to(_component.fallStopPosY_);
    _json.at("createNum").get_to(_component.createNum_);
    _json.at("mass").get_to(_component.mass_);
    _json.at("colliderRadius").get_to(_component.colliderRadius_);
}
