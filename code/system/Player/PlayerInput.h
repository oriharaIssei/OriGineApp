#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"

class Input;
class PlayerStates;
class PlayerInputSystem
    : public ISystem {
private:
    bool isInited_;
    PlayerStates* entityPlayerStates_ = nullptr;
    Transform* transform_             = nullptr;
    Transform* pivotTransform_        = nullptr;

public:
    PlayerInputSystem();
    ~PlayerInputSystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;
    void TransformInit(GameEntity* _entity);   

    //爆弾置く
    void PutBom(GameEntity* _entity);
  
    protected:
    void UpdateEntity(GameEntity* _entity) override;

    Input* input_ = nullptr;


};
