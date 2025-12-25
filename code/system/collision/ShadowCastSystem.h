#pragma once

#include "system/ISystem.h"

class ShadowCastSystem
    : public OriGine::ISystem {
public:
    ShadowCastSystem();
    ~ShadowCastSystem() override = default;
    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(OriGine::EntityHandle _entity) override;
};
