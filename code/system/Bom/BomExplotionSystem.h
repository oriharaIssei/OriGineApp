#pragma once

#include "system/ISystem.h"

class BomStatus;
class Input;
class BomExplotionSystem
    : public ISystem {
private:

    Input* input_ = nullptr;

public:
    BomExplotionSystem() : ISystem(SystemType::Input) {};
    ~BomExplotionSystem() = default;

    void Initialize() override;
    // void Update()override;
    void Finalize() override;


protected:
    virtual void UpdateEntity(GameEntity* _entity) override;
};
