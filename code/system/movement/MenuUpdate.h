#pragma once

#include "system/ISystem.h"

/// <summary>
/// メニューはSubSceneとして実装されている想定. MenuがActiveの間 GameのUpdateを止める.
/// </summary>
class MenuUpdate
    : public OriGine::ISystem {
public:
    MenuUpdate();
    ~MenuUpdate() override;

    void Initialize() override;
    void Finalize()override;

private:
    void UpdateEntity(OriGine::Entity* _entity) override;
};
