#pragma once
#include "system/ISystem.h"

#include <Vector3.h>
#include <Matrix4x4.h>
#include <Vector4.h>

class CameraTransform;
class EffectByBlockDrawSystem
    : public ISystem {
private:
   /* std::array<std::string, 10> textureName_;*/

public:
    EffectByBlockDrawSystem();
    ~EffectByBlockDrawSystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;

    Vec3f KScreenTransform(Vec3f worldPos, const CameraTransform& viewProjection);

   
    protected:
    void UpdateEntity(GameEntity* _entity) override;

  

};
