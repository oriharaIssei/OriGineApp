#include "FloatingFloorSpawner.h"
/// externals
#include "imgui/imgui.h"
#include <string>

void FloatingFloorSpawner::Initialize([[maybe_unused]] GameEntity* _entity) {
    /// 初期化でパラメータ編集してるから大丈夫、ここ消したら未定義値が出る
    sideSpace_      = 11.0f;
    positionHeight_ = 10.0f;
    columNumMax_    = 1;
    isCreated_      = false;
    HPMax_          = 2;
}

bool FloatingFloorSpawner::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

    isChange |= ImGui::InputInt("pillarHP", &HPMax_);
    isChange |= ImGui::InputInt("rowNumber", &rowNumber_);
    isChange |= ImGui::DragFloat3("FallCollisionMin", fallCollisionSizeMin_.v, 0.01f);
    isChange |= ImGui::DragFloat3("FallCollisionMax", fallCollisionSizeMax_.v, 0.01f);
    isChange |= ImGui::DragFloat3("fallCollisionCenterPos", fallCollisionCenterPos_.v, 0.01f);
    isChange |= ImGui::DragFloat("sideSpace", &sideSpace_, 0.01f);
    isChange |= ImGui::DragFloat("positionHeight", &positionHeight_, 0.01f);
    isChange |= ImGui::DragFloat("ratio", &ratio_, 0.01f);
    ImGui::Text("No Edit");

    isChange |= ImGui::DragFloat("revivalTime", &revivalTime_);
    return isChange;
}

void FloatingFloorSpawner::Finalize() {}

void to_json(nlohmann::json& _json, const FloatingFloorSpawner& _block) {
    _json["pillerSpace"]            = _block.positionHeight_;
    _json["columNumMax"]            = _block.columNumMax_;
    _json["rowNum"]                 = _block.rowNumber_;
    _json["pillarHP"]               = _block.HPMax_;
    _json["FallCollisionMin"]       = _block.fallCollisionSizeMin_;
    _json["FallCollisionMax"]       = _block.fallCollisionSizeMax_;
    _json["fallCollisionCenterPos"] = _block.fallCollisionCenterPos_;
    _json["sideSpace"]              = _block.sideSpace_;
    _json["revivalTime"]            = _block.revivalTime_;
    _json["ratio"]                  = _block.ratio_;
}

void from_json(const nlohmann::json& _json, FloatingFloorSpawner& _block) {
    _json.at("pillerSpace").get_to(_block.positionHeight_);
    _json.at("columNumMax").get_to(_block.columNumMax_);
    _json.at("rowNum").get_to(_block.rowNumber_);
    _json.at("pillarHP").get_to(_block.HPMax_);
    _json.at("FallCollisionMin").get_to(_block.fallCollisionSizeMin_);
    _json.at("FallCollisionMax").get_to(_block.fallCollisionSizeMax_);
    _json.at("fallCollisionCenterPos").get_to(_block.fallCollisionCenterPos_);
    _json.at("sideSpace").get_to(_block.sideSpace_);
    _json.at("revivalTime").get_to(_block.revivalTime_);
    _json.at("ratio").get_to(_block.ratio_);
}
