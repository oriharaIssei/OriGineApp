#include "StageObstacle.h"

#ifdef _DEBUG
#include "myGui/MyGui.h"
#endif // _DEBUG

void StageObstacle::Edit(Scene* /*_scene*/, Entity* /*_entity*/, [[maybe_unused]] const std::string& _parentLabel) {
#ifdef _DEBUG

    DragGuiCommand("Penalty Time (sec)##" + _parentLabel, penaltyTime_, 0.1f);

#endif // _DEBUG
}

void to_json(nlohmann::json& j, const StageObstacle& component) {
    j = nlohmann::json{
        {"penaltyTime", component.penaltyTime_}};
}

void from_json(const nlohmann::json& j, StageObstacle& component) {
    j.at("penaltyTime").get_to(component.penaltyTime_);
}
