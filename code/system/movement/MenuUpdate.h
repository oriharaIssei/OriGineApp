#pragma once

#include "system/ISystem.h"

/// <summary>
/// メニューはSubSceneとして実装されている想定. MenuがActiveの間 GameのUpdateを止める.
/// </summary>
class MenuUpdate
    : public ISystem {
public:
    MenuUpdate();
    ~MenuUpdate() override;

    void Initialize() override;
    void Finalize()override;

private:
    void UpdateEntity(Entity* _entity) override;
};
