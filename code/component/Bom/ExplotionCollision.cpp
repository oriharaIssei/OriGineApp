#include "ExplotionCollision.h"
/// engine
#define ENGINE_INCLUDE

#define RESOURCE_DIRECTORY // Resource の Directory
// ECS
#define ENGINE_ECS
#include "engine/EngineInclude.h"
/// externals
#include "imgui/imgui.h"
#include "myGui/MyGui.h"

void ExplotionCollision::Initialize([[maybe_unused]] GameEntity* _entity) {
    adaptTime_ = 0.1f;
}

bool ExplotionCollision::Edit() {
    bool isChange = false;

#ifdef _DEBUG

    isChange = CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();

    ImGui::Text("Offset");
    isChange |= InputVectorGuiCommand("##BomOffset", positionOffset_);

    ImGui::Text("etc");
    isChange |= DragGuiCommand("adaptTime", adaptTime_);
    isChange |= DragGuiCommand("CollisionRadius", collisionRadius_);

#endif // _DEBUG

    return isChange;
}

void ExplotionCollision::Finalize() {}

void ExplotionCollision::TimeDecrement() {
    adaptTime_ -= Engine::getInstance()->getDeltaTime();
}

void to_json(nlohmann::json& _json, const ExplotionCollision& _explotionCollision) {
    _json["isAlive"]         = _explotionCollision.isAlive_;
    _json["adaptTime"]       = _explotionCollision.adaptTime_;
    _json["positionOffset"]  = _explotionCollision.positionOffset_;
    _json["CollisionRadius"] = _explotionCollision.collisionRadius_;
}
void from_json(const nlohmann::json& _json, ExplotionCollision& _explotionCollision) {
    _json.at("isAlive").get_to(_explotionCollision.isAlive_);
    _json.at("adaptTime").get_to(_explotionCollision.adaptTime_);
    _json.at("positionOffset").get_to(_explotionCollision.positionOffset_);
    _json.at("CollisionRadius").get_to(_explotionCollision.collisionRadius_);
}
