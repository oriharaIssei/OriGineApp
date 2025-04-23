#include "UsingCameraSetSystem.h"

#define ENGINE_ECS
#include "EngineInclude.h"

#include "camera/CameraManager.h"

void UsingCameraSetSystem::Initialize() {
}

void UsingCameraSetSystem::Finalize() {
}

void UsingCameraSetSystem::UpdateEntity(GameEntity* _entity) {
    CameraTransform* cameraTransform = getComponent<CameraTransform>(_entity);
    if (!cameraTransform) {
        return;
    }

    CameraManager::getInstance()->setTransform(*cameraTransform);
}
