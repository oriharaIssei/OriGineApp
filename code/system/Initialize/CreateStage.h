#pragma once
#include "system/ISystem.h"

/// <summary>
/// ステージ情報からMesh,Collider等を生成するシステム
/// </summary>
class CreateStage
    : public OriGine::ISystem {
public:
    CreateStage() : ISystem(OriGine::SystemCategory::Initialize, 0) {}
    void Initialize() override {}
    void Finalize() override {}

protected:
    void UpdateEntity(OriGine::EntityHandle _handle) override;
};
