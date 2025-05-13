#include "BigExplotionCollision.h"
/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"
#include "myGui/MyGui.h"

void BigExplotionCollision::Initialize([[maybe_unused]] GameEntity* _entity) {
    adaptTime_ = 0.1f;
    plusScoreRatio_ = 3.0f;
}

bool BigExplotionCollision::Edit() {
    bool isChange = false;

    isChange = CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();

    ImGui::Text("Offset");
    isChange |= ImGui::InputFloat3("##BomOffset", positionOffset_.v);

    ImGui::Text("etc");
    isChange |= DragGuiCommand("adaptTime", adaptTime_);
    isChange |= DragGuiCommand("CollisionRadius", collisionRadius_);

    isChange |= DragGuiCommand("plusScoreRatio", plusScoreRatio_);
    return isChange;
}

void BigExplotionCollision::Finalize() {}

void BigExplotionCollision::TimeDecrement() {
    adaptTime_ -= Engine::getInstance()->getDeltaTime();
}

void to_json(nlohmann::json& j, const BigExplotionCollision& b) {
    j["isAlive"]         = b.isAlive_;
    j["adaptTime"]       = b.adaptTime_;
    j["positionOffset"]  = b.positionOffset_;
    j["collisionRadius"] = b.collisionRadius_;
    j["plusScoreRatio"]  = b.plusScoreRatio_;
}
void from_json(const nlohmann::json& j, BigExplotionCollision& b) {
    j.at("isAlive").get_to(b.isAlive_);
    j.at("adaptTime").get_to(b.adaptTime_);
    j.at("positionOffset").get_to(b.positionOffset_);
    j.at("collisionRadius").get_to(b.collisionRadius_);
    if (auto it = j.find("plusScoreRatio"); it != j.end()) {
        j.at("plusScoreRatio").get_to(b.plusScoreRatio_);
    }
}
