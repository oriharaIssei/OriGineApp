#pragma once

#include "system/ISystem.h"

/// stl
#include <memory>

/// System
#include "system/initialize/CreateSpriteFromTimer.h"

/// <summary>
/// ゲーム開始のタイマー初期化システム
/// </summary>
class StartTimerInitialize
    : public OriGine::ISystem {
public:
    StartTimerInitialize();
    ~StartTimerInitialize();

    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(OriGine::Entity* _entity) override;

private:
    std::unique_ptr<CreateSpriteFromTimer> createSpriteFromTimerSystem_ = nullptr;
};
