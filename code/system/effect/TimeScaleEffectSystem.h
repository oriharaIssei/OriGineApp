#pragma once

#include "system/ISystem.h"

/// stl
#include <string>
#include <unordered_map>

/// <summary>
/// タイムスケールタイマー
/// </summary>
struct TimeScaleTimer {
    float timeScale  = 0.f; // タイムスケール
    float scaleTimer = 0.f; // どれだけ時間を止めるか
};

/// <summary>
/// タイムスケールエフェクトを処理するシステム
/// </summary>
class TimeScaleEffectSystem
    : public OriGine::ISystem {
public:
    TimeScaleEffectSystem();
    ~TimeScaleEffectSystem() override;
    void Initialize() override;
    void Finalize() override;

private:
    void UpdateEntity(OriGine::EntityHandle _handle) override;
    void Update() override;

private:
    std::unordered_map<std::string, TimeScaleTimer> stopTimesByTimeScale_;
};
