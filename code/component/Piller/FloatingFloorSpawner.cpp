#include "FloatingFloorSpawner.h"
/// externals
#include "imgui/imgui.h"
#include "myGui/MyGui.h"
#include <string>

void FloatingFloorSpawner::Initialize([[maybe_unused]] GameEntity* _entity) {
    /// 初期化でパラメータ編集してるから大丈夫、ここ消したら未定義値が出る
    sideSpace_      = 11.0f;
    positionHeight_ = 40.0f;
    columNumMax_    = 1;
    isCreated_      = false;
    HPMax_          = 2;
}

bool FloatingFloorSpawner::Edit() {
    bool isChange = false;

    isChange = CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();

    isChange |= InputGuiCommand("pillarHP", HPMax_);
    isChange |= InputGuiCommand("rowNumber", rowNumber_);
    isChange |= DragGuiVectorCommand<3,float>("FallCollisionMin", fallCollisionSizeMin_, 0.01f);
    isChange |= DragGuiVectorCommand<3,float>("FallCollisionMax", fallCollisionSizeMax_, 0.01f);
    isChange |= DragGuiVectorCommand<3,float>("fallCollisionCenterPos", fallCollisionCenterPos_, 0.01f);
    isChange |= DragGuiCommand("sideSpace", sideSpace_, 0.01f);
    isChange |= DragGuiCommand("positionHeight", positionHeight_, 0.01f);
    isChange |= DragGuiCommand("ratio", scoreUpRate_, 0.01f);
    ImGui::Text("No Edit");

    isChange |= DragGuiCommand("revivalTime", revivalTime_);
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
    _json["ratio"]                  = _block.scoreUpRate_;
    /*_json["positionHeight"]         = _block.positionHeight_;*/
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
    _json.at("ratio").get_to(_block.scoreUpRate_);
   /* _json.at("positionHeight").get_to(_block.positionHeight_);*/
}
