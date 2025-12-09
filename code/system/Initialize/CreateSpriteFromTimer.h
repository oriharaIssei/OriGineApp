#pragma once
#include "system/ISystem.h"

struct TimerForSpriteComponent;

/// <summary>
/// Timerの数字をSpriteとして表示するためにSpriteを生成するシステム
/// </summary>
class CreateSpriteFromTimer
    : public ISystem {
public:
    CreateSpriteFromTimer();
    ~CreateSpriteFromTimer() override;
    void Initialize();
    // virtual void Update();
    // virtual void Edit();
    void Finalize();

    void UpdateEntity(Entity* _entity) override;
    void CreateSprites(Entity* _entity, TimerForSpriteComponent* _forSpriteComp);
};
