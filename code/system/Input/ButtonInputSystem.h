#pragma once

#include "system/ISystem.h"

/// <summary>
/// ボタン入力を処理するシステム
/// </summary>
class ButtonInputSystem
    : public ISystem {
public:
    ButtonInputSystem() : ISystem(SystemCategory::Input) {}
    ~ButtonInputSystem() {}

    virtual void Initialize();
    virtual void Finalize();

protected:
    virtual void UpdateEntity(Entity* _entity);
};
