#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"


class ResultScoreAdaptSystem
    : public ISystem {
private:
 
    float cutNum_;

public:
    ResultScoreAdaptSystem();
    ~ResultScoreAdaptSystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;

   void ComboReset();
    protected:
    void UpdateEntity(GameEntity* _entity) override;

  

};
