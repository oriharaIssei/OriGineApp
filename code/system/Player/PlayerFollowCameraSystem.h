#pragma once
#include "system/ISystem.h"
#include "component/Player/PlayerStates.h"


class PlayerFollowCameraSystem
    : public ISystem {
private:
   

public:
    PlayerFollowCameraSystem();
    ~PlayerFollowCameraSystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;
  
    protected:
    void UpdateEntity(GameEntity* _entity) override;

  

};
