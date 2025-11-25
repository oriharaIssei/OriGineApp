#pragma once
#include "system/ISystem.h"

class PenaltySystem
    : public ISystem {
public:
    PenaltySystem();
    ~PenaltySystem();
    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(Entity* _entity) override;
};
