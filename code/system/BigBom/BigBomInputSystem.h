#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"
#include <Quaternion.h>
#include <Vector3.h>

class Input;
class PlayerStates;
class BigBomInputSystem
    : public ISystem {
private:
    bool isInited_;
    PlayerStates* entityPlayerStates_ = nullptr;
    Transform* transform_             = nullptr;
    Transform* pivotTransform_        = nullptr;

public:
    BigBomInputSystem();
    ~BigBomInputSystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;
    void TransformInit(GameEntity* _entity);

    Quaternion LookRotation(const Vec3f& forward, const Vec3f& up);

    //爆弾置く
    void PutBom(GameEntity* _entity);
  
    protected:
    void UpdateEntity(GameEntity* _entity) override;

    Input* input_ = nullptr;


};
