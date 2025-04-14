#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"

class ComboStatus;
class ComboSystem
    : public ISystem {
private:
    ComboStatus* comboStatus_;

public:
    ComboSystem();
    ~ComboSystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;

   void ComboReset();
    protected:
    void UpdateEntity(GameEntity* _entity) override;

  

};
