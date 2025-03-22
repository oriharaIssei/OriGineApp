#pragma once

#include "system/ISystem.h"

class BulletSpawner;

class ShotBulletSystem
    : public ISystem {
public:
    ShotBulletSystem() : ISystem(SystemType::Movement) {};
    ~ShotBulletSystem() = default;

    void Initialize() override;
    // void Update()override;
    void Finalize() override;

protected:
    virtual void UpdateEntity(GameEntity* _entity) override;
    void SpawnBullet(GameEntity* _entity, BulletSpawner* _spawner);
};
