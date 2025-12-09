#pragma once

#include "system/ISystem.h"

/// <summary>
/// PlayRecorderの初期化
/// </summary>
class PlayRecorderInitialize
    : public ISystem {
public:
    PlayRecorderInitialize();
    ~PlayRecorderInitialize() override;

    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(Entity* _entity) override;
};
