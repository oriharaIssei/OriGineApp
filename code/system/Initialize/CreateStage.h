#pragma once
#include "system/ISystem.h"

/// <summary>
/// ステージ情報からMesh,Collider等を生成するシステム
/// </summary>
class CreateStage
    : public ISystem {
public:
    CreateStage() : ISystem(SystemCategory::Initialize, 0) {}
    void Initialize() override {}
    void Finalize() override {}

protected:
    void UpdateEntity(Entity* _entity) override;
};
