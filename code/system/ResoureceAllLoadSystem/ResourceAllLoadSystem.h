#pragma once
#include "system/ISystem.h"

#include <Matrix4x4.h>
#include <Vector3.h>
#include <Vector4.h>

class CameraTransform;
class ResourceAllLoadSystem
    : public ISystem {
private:

public:
    static bool isLoaded_;
    ResourceAllLoadSystem();
    ~ResourceAllLoadSystem();

    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(GameEntity* _entity) override;
};
