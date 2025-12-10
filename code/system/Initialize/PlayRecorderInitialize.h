#pragma once

#include "system/ISystem.h"

/// <summary>
/// PlayRecorderの初期化
/// </summary>
class PlayRecorderInitialize
    : public OriGine::ISystem {
public:
    PlayRecorderInitialize();
    ~PlayRecorderInitialize() override;

    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(OriGine::Entity* _entity) override;
};
