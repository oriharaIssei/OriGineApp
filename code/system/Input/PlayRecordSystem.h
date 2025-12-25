#pragma once
#include "system/ISystem.h"
#include <entity/Entity.h>

/// <summary>
/// PlayRecorderの更新
/// </summary>
class PlayRecordSystem
    : public OriGine::ISystem {
public:
    PlayRecordSystem();
    ~PlayRecordSystem() override;

    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(OriGine::EntityHandle _handle) override;
};
