#include "TimeScaleEffectComponent.h"

/// engine
#include "Engine.h"

#ifdef _DEBUG
#include "myGui/MyGui.h"
#endif // _DEBUG

using namespace OriGine;

TimeScaleEffectComponent::TimeScaleEffectComponent() {}

TimeScaleEffectComponent::~TimeScaleEffectComponent() {}

void TimeScaleEffectComponent::Initialize(OriGine::Scene* /*_scene*/, OriGine::EntityHandle /*_owner*/) {
}

void TimeScaleEffectComponent::Finalize() {
}

void TimeScaleEffectComponent::Edit(OriGine::Scene* /*_scene*/, OriGine::EntityHandle /*_owner*/, [[maybe_unused]] const std::string& _parentLabel) {
#ifdef _DEBUG
    CheckBoxCommand("IsActive##" + _parentLabel, isActive_);

    DragGuiCommand("Duration##" + _parentLabel, timeScaleDuration_, 0.01f);

    SlideGuiCommand("FadeIn##" + _parentLabel, fadeInDuration_, 0.0f, timeScaleDuration_ - fadeOutDuration_);
    SlideGuiCommand("FadeOut##" + _parentLabel, fadeOutDuration_, 0.0f, timeScaleDuration_ - fadeInDuration_);

    DragGuiCommand("TimeScale##" + _parentLabel, timeScale_, 0.01f);

    ImGui::Spacing();

    std::string label = "TimeScaleTags##" + _parentLabel;
    if (ImGui::TreeNode(label.c_str())) {
        DeltaTimer* deltaTimer = Engine::GetInstance()->GetDeltaTimer();
        for (const auto& [key, scale] : deltaTimer->GetDeltaTimeScaleMap()) {
            bool isActive = timeScaleTags_.end() != std::find(timeScaleTags_.begin(), timeScaleTags_.end(), key);
            bool preState = isActive;
            label         = key + "##" + _parentLabel;
            if (ImGui::Checkbox(label.c_str(), &isActive)) {
                if (isActive && !preState) {
                    timeScaleTags_.insert(key);
                } else if (!isActive && preState) {
                    timeScaleTags_.erase(key);
                }
            }
        }
        ImGui::TreePop();
    }

#endif // _DEBUG
}

void to_json(nlohmann::json& _j, const TimeScaleEffectComponent& _component) {
    _j = nlohmann::json{
        {"isActive", _component.isActive_},
        {"timeScaleTags", _component.timeScaleTags_},
        {"timeScale", _component.timeScale_},
        {"timeScaleDuration", _component.timeScaleDuration_},
        {"fadeInDuration", _component.fadeInDuration_},
        {"fadeOutDuration", _component.fadeOutDuration_},
    };
}

void from_json(const nlohmann::json& _j, TimeScaleEffectComponent& _component) {
    _j.at("isActive").get_to(_component.isActive_);
    _j.at("timeScaleTags").get_to(_component.timeScaleTags_);
    _j.at("timeScale").get_to(_component.timeScale_);
    _j.at("timeScaleDuration").get_to(_component.timeScaleDuration_);
    _j.at("fadeInDuration").get_to(_component.fadeInDuration_);
    _j.at("fadeOutDuration").get_to(_component.fadeOutDuration_);
}
