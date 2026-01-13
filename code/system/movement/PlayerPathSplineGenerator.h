#pragma once
#include "system/ISystem.h"

/// <summary>
/// プレイヤーの移動経路スプラインを生成するシステム
/// </summary>
class PlayerPathSplineGenerator
    : public OriGine::ISystem {
public:
    PlayerPathSplineGenerator();
    ~PlayerPathSplineGenerator() override;

    void Initialize() override;
    void Finalize() override;

private:
    void UpdateEntity(OriGine::EntityHandle _handle) override;
};
