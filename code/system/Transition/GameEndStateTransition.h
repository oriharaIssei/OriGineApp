#pragma once
#include "system/ISystem.h"

class GameEndStateTransition
    : public ISystem {
public:
    GameEndStateTransition();
    ~GameEndStateTransition() override;

    virtual void Initialize();
    // virtual void Update();
    // virtual void Edit();
    virtual void Finalize();

protected:
    virtual void UpdateEntity(GameEntity* _entity) override;

    void SettingsTransitionEffect(GameEntity* _effectEntity);

private:
    bool isTransitioning_ = false; // 状態遷移中かどうか
};
