#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"

class EffectByBlockUIStatus;
class EffectByBlockDeleteSystem
    : public ISystem {
private:
    EffectByBlockUIStatus* effectByBlock_;

public:
    EffectByBlockDeleteSystem();
    ~EffectByBlockDeleteSystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;

   void ComboReset();
    protected:
    void UpdateEntity(GameEntity* _entity) override;

  float Lerp(const float& start, const float& end, float t);

};
