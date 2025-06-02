#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"


class CameraShakeSystem
    : public ISystem {
private:
   
    

public:
    CameraShakeSystem();
    ~CameraShakeSystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;

   void ComboReset();
    protected:
    void UpdateEntity(GameEntity* _entity) override;


};
