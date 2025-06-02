#pragma once

#include "system/ISystem.h"

class BomStatus;
class ExplotionCollision;
class BomSpawner;

class PutBomSystem
    : public ISystem {
public:
    PutBomSystem() : ISystem(SystemType::Movement) {};
    ~PutBomSystem() = default;

    void Initialize() override;
    // void Update()override;
    void Finalize() override;

protected:
    virtual void UpdateEntity(GameEntity* _entity) override;
    void SpawnBom(GameEntity* _entity, BomStatus* _spawner);

private:
    ExplotionCollision* bomExCollision_;
    BomSpawner* bomSpawner_;
};
