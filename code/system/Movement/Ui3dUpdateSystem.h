#pragma once
#include "C:\Users\ira34\source\repos\OriGine\project\engine\code\ECS\system\ISystem.h"

/// <summary>
/// 3D UI更新処理を行うシステム
/// </summary>
class Ui3dUpdateSystem
    : public OriGine::ISystem {
public:
    Ui3dUpdateSystem();
    ~Ui3dUpdateSystem();

    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(OriGine::Entity* _entity) override;
};
