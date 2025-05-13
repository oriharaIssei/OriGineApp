#include "PlayerStates.h"
/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"
#include "myGui/MyGui.h"

void PlayerStates::Initialize([[maybe_unused]] GameEntity* _entity) {

  /*  offSetY_            = 1.5f;
    bomExplotionNum_    = 0;
    currentBigBomPoint_ = 0;*/
}

bool PlayerStates::Edit() {
    bool isChange = false;

    isChange = CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();

    isChange |= DragGuiCommand("MoveSpeed", moveSpeed_, 0.01f);
    isChange |= DragGuiCommand("MoveRadius", moveRadius_, 0.01f);
    isChange |= DragGuiCommand("OffSetY", offSetY_, 0.01f);
    isChange |= InputGuiCommand("bigBomPointMax", bigBomPointMax_);
    isChange |= DragGuiVectorCommand<3,float>("followCameraOffset", followCameraOffset_, 0.01f);
    return isChange;
}

void PlayerStates::Finalize() {}

void PlayerStates::IncrementCurrentBigBomPoint() {
    currentBigBomPoint_++;
}

void PlayerStates::ReSetCurrentBigBomPoint() {
    currentBigBomPoint_ = 0;
 }

void to_json(nlohmann::json& j, const PlayerStates& p) {
     j["isAlive"]            = p.isAlive_;
     j["moveSpeed"]          = p.moveSpeed_;
     j["moveRadius"]         = p.moveRadius_;
     j["OffSetY"]            = p.offSetY_;
     j["bigBomPointMax"]     = p.bigBomPointMax_;
     j["followCameraOffset"] = p.followCameraOffset_;
 }

 void from_json(const nlohmann::json& j, PlayerStates& p) {
     j.at("isAlive").get_to(p.isAlive_);
     j.at("moveSpeed").get_to(p.moveSpeed_);
     j.at("moveRadius").get_to(p.moveRadius_);
     j.at("OffSetY").get_to(p.offSetY_);
     j.at("bigBomPointMax").get_to(p.bigBomPointMax_);
     j.at("followCameraOffset").get_to(p.followCameraOffset_);
 }
