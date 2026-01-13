#pragma once

#include "system/ISystem.h"

/// <summary>
/// 衝突した際にAddForceComponentを適応する処理を行うシステム
/// </summary>
class AddForceTriggerSystem
    : public OriGine::ISystem {
public:
    AddForceTriggerSystem();
    ~AddForceTriggerSystem() override;

    void Initialize() override;
    void Finalize() override;

private:
    void UpdateEntity(OriGine::EntityHandle _handle) override;
};
