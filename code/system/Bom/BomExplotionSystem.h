#pragma once

#include "system/ISystem.h"

class BomStatus;
class BomExplotionSystem
    : public ISystem {
private:



public:
    BomExplotionSystem() : ISystem(SystemType::Movement) {};
    ~BomExplotionSystem() = default;

    void Initialize() override;
    // void Update()override;
    void Finalize() override;


protected:
    virtual void UpdateEntity(GameEntity* _entity) override;
};
