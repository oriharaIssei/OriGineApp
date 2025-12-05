#pragma once
#include "system/ISystem.h"

/// <summary>
/// BackFireの初期化を行うシステム
/// </summary>
class BackFireInitialize
    : public ISystem {
public:
    BackFireInitialize();
    ~BackFireInitialize() override;

    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(Entity* _entity) override;
};
