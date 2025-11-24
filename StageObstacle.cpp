#include "StageObstacle.h"

#ifdef _DEBUG
#include "myGui/MyGui.h"
#endif // _DEBUG

void StageObstacle::Edit(Scene* /*_scene*/, Entity* /*_entity*/, const std::string& _parentLabel) {
#ifdef _DEBUG

    DragGuiCommand("Penalty Time (sec)", penaltyTime_, 0.1f);

#endif // _DEBUG
}

void to_json(nlohmann::json& j, const StageObstacle& component) {
    j = nlohmann::json{
        {"controlPointIndex", component.controlPointIndex_},
        {"penaltyTime", component.penaltyTime_},
        {"size", component.size},
    };
}

void from_json(const nlohmann::json& j, StageObstacle& component) {
    j.at("controlPointIndex").get_to(component.controlPointIndex_);
    j.at("penaltyTime").get_to(component.penaltyTime_);
    j.at("size").get_to(component.size);
}
