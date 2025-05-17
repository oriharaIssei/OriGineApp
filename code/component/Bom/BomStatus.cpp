#include "BomStatus.h"

/// externals
#include "imgui/imgui.h"
#include "myGui/MyGui.h"

void BomStatus::Initialize([[maybe_unused]] GameEntity* _entity) {
    currentTime_     = 0.0f;
    explotionTime_   = 2.0f;
    collisionRadius_ = 2.0f;
    positionOffset_  = Vec3f(0.0f, 0.0f, 0.0f);
    launthSpeed_     = 31.0f;
}

bool BomStatus::Edit() {
    bool isChange = false;

    isChange = CheckBoxCommand("IsAlive", isAlive_);

    ImGui::Spacing();

    ImGui::Text("Offset");
    isChange |= InputVectorGuiCommand("##BomOffset", positionOffset_);

    ImGui::Text("etc");
    isChange |= DragGuiCommand("explotionTime", explotionTime_);
    isChange |= DragGuiCommand("CollisionRadius", collisionRadius_);
    isChange |= DragGuiCommand("launthSpeed", launthSpeed_, 0.1f);

    return isChange;
}

void BomStatus::Finalize() {}

void to_json(nlohmann::json& _json, const BomStatus& _block) {
    _json["isAlive"]         = _block.isAlive_;
    _json["explotionTime"]   = _block.explotionTime_;
    _json["positionOffset"]  = _block.positionOffset_;
    _json["CollisionRadius"] = _block.collisionRadius_;
    _json["launthSpeed"]     = _block.launthSpeed_;
}
void from_json(const nlohmann::json& _json, BomStatus& _block) {
    _json.at("isAlive").get_to(_block.isAlive_);
    _json.at("explotionTime").get_to(_block.explotionTime_);
    _json.at("positionOffset").get_to(_block.positionOffset_);
    _json.at("CollisionRadius").get_to(_block.collisionRadius_);
    _json.at("launthSpeed").get_to(_block.launthSpeed_);
}
