#include "BigBomSpawner.h"

/// externals
#include "imgui/imgui.h"

void BigBomSpawner::Initialize([[maybe_unused]] GameEntity* _entity) {

    currentPutCoolTime_ = 0.0f;
    isPut_              = false;
    putTotalNum_         = 0;
}

bool BigBomSpawner::Edit() {
    bool isChange = false;

    isChange = ImGui::Checkbox("IsAlive", &isAlive_);

    ImGui::Spacing();

  
    isChange |= ImGui::DragFloat("launthSpeed", &launthSpeed_); 
    isChange |= ImGui::DragFloat3("spawnOffset", spawnOffset_.v);
    ImGui::Text("collsion");
    isChange |= ImGui::DragFloat3("collisionCenter", collisionCenter_.v);
    isChange |= ImGui::DragFloat("collisionRadius", &collisionRadius_);
  
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
