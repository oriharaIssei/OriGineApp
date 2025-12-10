#pragma once
#include "system/ISystem.h"

class PenaltyTimeSpriteUpdate
    : public OriGine::ISystem {
public:
    PenaltyTimeSpriteUpdate();
    ~PenaltyTimeSpriteUpdate() override;
    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(OriGine::Entity* _entity) override;

private:
    float animationTime_ = 1.2f;
};
