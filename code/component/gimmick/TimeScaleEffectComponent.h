#pragma once

#include "component/IComponent.h"

/// stl
#include <set>

/// <summary>
/// DeltaTimeにScaleをかけるギミックのコンポーネント
/// </summary>
class TimeScaleEffectComponent
    : public OriGine::IComponent {
    friend void to_json(nlohmann::json& _j, const TimeScaleEffectComponent& _component);
    friend void from_json(const nlohmann::json& _j, TimeScaleEffectComponent& _component);

public:
    TimeScaleEffectComponent();
    ~TimeScaleEffectComponent() override;
    void Initialize(OriGine::Scene* _scene, OriGine::EntityHandle _owner) override;
    void Finalize() override;
    void Edit(OriGine::Scene* _scene, OriGine::EntityHandle _owner, const std::string& _parentLabel) override;

private:
    bool isActive_{false};
    std::set<std::string> timeScaleTags_{};
    float timeScale_{1.0f};
    float timeScaleDuration_{0.0f}; // タイムスケールの効果時間
    float fadeInDuration_{0.0f}; // タイムスケールの効果のフェードイン時間
    float fadeOutDuration_{0.0f}; // タイムスケールの効果のフェードアウト時間
    float timeScaleElapsed_{0.0f}; // タイムスケールの効果が開始してからの経過時間

public:
    bool IsActive() const { return isActive_; }
    void SetActive(bool _isActive) { isActive_ = _isActive; }
    const std::set<std::string>& GetTimeScaleTags() const { return timeScaleTags_; }
    float GetTimeScale() const { return timeScale_; }
    float GetTimeScaleDuration() const { return timeScaleDuration_; }
    float GetTimeScaleElapsed() const { return timeScaleElapsed_; }
    void SetTimeScaleElapsed(float _elapsed) { timeScaleElapsed_ = _elapsed; }
    float GetFadeInDuration() const { return fadeInDuration_; }
    float GetFadeOutDuration() const { return fadeOutDuration_; }
};
