#pragma once
#include "component/transform/CameraTransform.h"
#include "component/transform/Transform.h"
#include "system/ISystem.h"
#include <Matrix4x4.h>
#include <Vector3.h>

class PlayerStates;
class PlayerFollowCameraSystem
    : public ISystem {
private:
   

public:
    PlayerFollowCameraSystem();
    ~PlayerFollowCameraSystem();

    void Initialize() override;
    // void Update() override;
    void Finalize() override;
    void GetTransformForPlayer(GameEntity* _entity);

protected:
    void UpdateEntity(GameEntity* _entity) override;

    Vec3f Multiply(const Matrix4x4& rotationMatrix, const Vec3f& offset);
    Quaternion FromMatrix(const Matrix4x4& matrix);
    Quaternion LookAt(const Vec3f& direction, const Vec3f& up);
};
