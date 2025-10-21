#pragma once
#include "system/ISystem.h"

/// <summary>
/// InputDeviceに応じてゲームUIの表示/非表示を切り替えるシステム
/// </summary>
class ShowGameUIByInputDevice
    : public ISystem {
public:
    ShowGameUIByInputDevice();
    ~ShowGameUIByInputDevice() override = default;

    void Initialize() override;
    void Finalize() override;

private:
    void UpdateEntity(Entity* _entity) override;
};
