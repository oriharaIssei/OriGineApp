#include "BigBomSpawner.h"

/// externals
#include "imgui/imgui.h"
#include "myGui/MyGui.h"

void BigBomSpawner::Initialize([[maybe_unused]] GameEntity* _entity) {

    currentPutCoolTime_ = 0.0f;
    isPut_              = false;
    putTotalNum_         = 0;
}

bool BigBomSpawner::Edit() {
    bool isChange = false;

    isChange = CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();
    
  
    isChange |= DragGuiCommand("launthSpeed", launthSpeed_); 
    isChange |= DragGuiVectorCommand<3,float>("spawnOffset", spawnOffset_);
    ImGui::Text("collsion");
    isChange |= DragGuiVectorCommand<3,float>("collisionCenter", collisionCenter_);
    isChange |= DragGuiCommand("collisionRadius", collisionRadius_);
  
    return isChange;
}

void BigBomSpawner::Finalize() {}

void to_json(nlohmann::json& j, const BigBomSpawner& b) {
    j["isAlive"]         = b.isAlive_;
    j["ableSetBomNum"]   = b.ableSetBomNum_;
    j["putCoolTimeMax"]  = b.putCoolTimeMax_;
    j["spawnOffset"]     = b.spawnOffset_;
    j["collisionCenter"] = b.collisionCenter_;
    j["collisionRadius"] = b.collisionRadius_;
    j["launthSpeed"]     = b.launthSpeed_;
}
void from_json(const nlohmann::json& j, BigBomSpawner& b) {
    j.at("isAlive").get_to(b.isAlive_);
    j.at("ableSetBomNum").get_to(b.ableSetBomNum_);
    j.at("putCoolTimeMax").get_to(b.putCoolTimeMax_);
    j.at("spawnOffset").get_to(b.spawnOffset_);
    j.at("collisionCenter").get_to(b.collisionCenter_);
    j.at("collisionRadius").get_to(b.collisionRadius_);
    j.at("launthSpeed").get_to(b.launthSpeed_);
}
