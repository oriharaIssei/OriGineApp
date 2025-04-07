#pragma once
#include "system/ISystem.h"
#include "component/transform/Transform.h"
#include "component/transform/CameraTransform.h"
#include <Matrix4x4.h>
#include <Vector3.h>

class PlayerStates;
class PlayerFollowCameraSystem
    : public ISystem {
private:
    PlayerStates* entityPlayerStates_ = nullptr;
    Transform* transform_             = nullptr;
    Transform* pivotTransform_        = nullptr;
    CameraTransform* cameraTransform_ = nullptr;
    bool isInited_                    = false;

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

};
