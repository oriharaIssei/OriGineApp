#pragma once
#include "component/IComponent.h"

/// math
#include <cstdint>
#include <math/Vector2.h>

namespace OriGine {
class Scene;
}

/// <summary>
/// Timerコンポーネント
/// </summary>
class TimerComponent
    : public OriGine::IComponent {
    /// <summary>
    /// JSON 変換用
    /// </summary>
    friend void to_json(nlohmann::json& j, const TimerComponent& c);
    /// <summary>
    /// JSON 復元用
    /// </summary>
    friend void from_json(const nlohmann::json& j, TimerComponent& c);

public:
    TimerComponent();
    ~TimerComponent() override;

    /// <summary>
    /// 初期化処理
    /// </summary>
    void Initialize(OriGine::Scene* _scene, OriGine::EntityHandle _owner) override;
    /// <summary>
    /// エディタ用編集UI
    /// </summary>
    void Edit(OriGine::Scene* _scene, OriGine::EntityHandle _owner, const std::string& _parentLabel) override;
    /// <summary>
    /// 終了処理
    /// </summary>
    void Finalize() override;

private:
    /// <summary>
    /// システムで使用を開始したかどうか
    /// </summary>
    bool isStarted_ = false;

    float maxTime_     = 0.f;
    float currentTime_ = 0.f;

    OriGine::Vec2f spriteSize_;

public:
    bool IsStarted() const { return isStarted_; }
    void SetStarted(bool started) { isStarted_ = started; }

    float GetMaxTime() const { return maxTime_; }
    void SetMaxTime(float time) { maxTime_ = time; }

    float GetTime() const { return currentTime_; }
    void SetCurrentTime(float time) { currentTime_ = time; }

    void ResetCurrentTime() { currentTime_ = maxTime_; }
};
