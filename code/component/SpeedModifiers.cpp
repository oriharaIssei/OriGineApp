#include "SpeedModifiers.h"

/// stl
#include <algorithm>

/// ECS
// component
#include "component/physics/Rigidbody.h"

/// math
#include "math/MyEasing.h"
#include <math/Interpolation.h>

#ifdef _DEBUG
#include "myGui/MyGui.h"
#endif // _DEBUG

using namespace OriGine;

SpeedModifiers::SpeedModifiers() {}
SpeedModifiers::~SpeedModifiers() {}

void SpeedModifiers::Initialize(Scene* /*_scene*/, EntityHandle /*_owner*/) {}

void SpeedModifiers::Finalize() {}

void SpeedModifiers::Edit(Scene* /*_scene*/, EntityHandle /*_owner*/, [[maybe_unused]] const std::string& _parentLabel) {
#ifdef _DEBUG

    CheckBoxCommand("Is Auto Destroyed##" + _parentLabel, isAutoDestroyed);

    ImGui::Spacing();

    DragGuiCommand("Restore Speed##" + _parentLabel, restoreSpeed, 0.01f);

    ImGui::Spacing();

    ImGui::SeparatorText("Additive");
    ImGui::Spacing();

    EasingComboGui("Additive Lerp Ease Type##" + _parentLabel, additiveLerpEaseType);
    ImGui::Spacing();

    DragGuiCommand("Additive Target##" + _parentLabel, additiveTarget, 0.01f);
    DragGuiCommand("Additive Duration##" + _parentLabel, additiveDuration, 0.01f);
    DragGuiCommand("Additive Lerp Duration##" + _parentLabel, additiveLerpDuration, 0.01f);

    ImGui::Spacing();

    ImGui::SeparatorText("Multiplier");
    ImGui::Spacing();
    EasingComboGui("Multiplier Lerp Ease Type##" + _parentLabel, multiplierLerpEaseType);
    ImGui::Spacing();

    DragGuiCommand("Multiplier Target##" + _parentLabel, multiplierTarget, 0.01f);
    DragGuiCommand("Multiplier Duration##" + _parentLabel, multiplierDuration, 0.01f);
    DragGuiCommand("Multiplier Lerp Duration##" + _parentLabel, multiplierLerpDuration, 0.01f);
#endif // _DEBUG
}

void SpeedModifiers::Reset() {
    additiveTarget       = kZeroVec3f;
    additiveDuration     = 0.0f;
    additiveTimer        = 0.0f;
    additiveLerpDuration = 0.0f;
    additiveLerpTimer    = 0.0f;

    multiplierTarget       = kOneVec3f;
    multiplierDuration     = 0.0f;
    multiplierTimer        = 0.0f;
    multiplierLerpDuration = 0.0f;
    multiplierLerpTimer    = 0.0f;
}

void SpeedModifiers::StartAdditiveEffect(
    const OriGine::Vec3f& _target,
    float _lerpDuration,
    float _effectDuration,
    OriGine::EaseType _easeType,
    float _beforeSpeed,
    float _restoreSpeed) {
    additiveTarget       = _target;
    additiveDuration     = _effectDuration;
    additiveTimer        = 0.0f;
    additiveLerpDuration = _lerpDuration;
    additiveLerpTimer    = 0.0f;
    additiveLerpEaseType = _easeType;

    beforeSpeed  = _beforeSpeed;
    restoreSpeed = _restoreSpeed;
}

void SpeedModifiers::StartMultiplierEffect(
    const OriGine::Vec3f& _target,
    float _lerpDuration,
    float _effectDuration,
    OriGine::EaseType _easeType,
    float _beforeSpeed,
    float _restoreSpeed) {
    multiplierTarget       = _target;
    multiplierDuration     = _effectDuration;
    multiplierLerpDuration = _lerpDuration;
    multiplierTimer        = 0.f;
    multiplierTimer        = 0.0f;
    multiplierLerpEaseType = _easeType;

    beforeSpeed  = _beforeSpeed;
    restoreSpeed = _restoreSpeed;
}

void to_json(nlohmann::json& _j, const SpeedModifiers& _c) {
    _j = nlohmann::json{
        {"isAutoDestroyed", _c.isAutoDestroyed},
        {"additiveTarget", _c.additiveTarget},
        {"additiveDuration", _c.additiveDuration},
        {"additiveLerpDuration", _c.additiveLerpDuration},
        {"additiveLerpEaseType", static_cast<int>(_c.additiveLerpEaseType)},
        {"multiplierTarget", _c.multiplierTarget},
        {"multiplierDuration", _c.multiplierDuration},
        {"multiplierLerpDuration", _c.multiplierLerpDuration},
        {"multiplierLerpEaseType", static_cast<int>(_c.multiplierLerpEaseType)},
        {"restoreSpeed", _c.restoreSpeed}};
}

void from_json(const nlohmann::json& _j, SpeedModifiers& _c) {
    _j.at("isAutoDestroyed").get_to(_c.isAutoDestroyed);
    _j.at("additiveTarget").get_to(_c.additiveTarget);
    _j.at("additiveDuration").get_to(_c.additiveDuration);
    _j.at("additiveLerpDuration").get_to(_c.additiveLerpDuration);
    int additiveEaseType;
    _j.at("additiveLerpEaseType").get_to(additiveEaseType);
    _c.additiveLerpEaseType = static_cast<OriGine::EaseType>(additiveEaseType);
    _j.at("multiplierTarget").get_to(_c.multiplierTarget);
    _j.at("multiplierDuration").get_to(_c.multiplierDuration);
    _j.at("multiplierLerpDuration").get_to(_c.multiplierLerpDuration);
    int multiplierEaseType;
    _j.at("multiplierLerpEaseType").get_to(multiplierEaseType);
    _c.multiplierLerpEaseType = static_cast<OriGine::EaseType>(multiplierEaseType);
    _j.at("restoreSpeed").get_to(_c.restoreSpeed);
}
