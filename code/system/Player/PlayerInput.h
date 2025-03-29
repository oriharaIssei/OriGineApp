#pragma once
#include "system/ISystem.h"
#include <Vector3.h>
#include <Quaternion.h>
#include "component/Player/PlayerStates.h"
class Input;

class PlayerInputSystem
    : public ISystem {
public:
    PlayerInputSystem();
    ~PlayerInputSystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;
    void TransformInit(GameEntity* _entity);   

  
    protected:
    void UpdateEntity(GameEntity* _entity) override;

    Input* input_ = nullptr;

private:
    bool isInited_;
    PlayerStates* entityPlayerStates;
    Transform* transform_;
    Transform* pivotTransform_;
};
