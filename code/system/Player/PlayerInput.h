#pragma once
#include "system/ISystem.h"
#include "component/Player/PlayerStates.h"
class Input;

class PlayerInputSystem
    : public ISystem {
private:
    bool isInited_;
    PlayerStates* entityPlayerStates_;
    Transform* transform_;
    Transform* pivotTransform_;

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
