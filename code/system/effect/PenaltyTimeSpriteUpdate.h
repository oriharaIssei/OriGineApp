#pragma once
#include "system/ISystem.h"

class PenaltyTimeSpriteUpdate
    : public ISystem {
public:
    PenaltyTimeSpriteUpdate();
    ~PenaltyTimeSpriteUpdate() override;
    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(Entity* _entity) override;

private:
    float animationTime_ = 1.2f;
};
