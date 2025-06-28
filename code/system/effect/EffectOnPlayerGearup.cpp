#include "EffectOnPlayerGearup.h"

/// Engine
#define ENGINE_ECS
#define DELTA_TIME
#include "EngineInclude.h"

// component
#include "component/effect/CameraAction.h"
#include "component/Player/PlayerStatus.h"

EffectOnPlayerGearup::EffectOnPlayerGearup() : ISystem(SystemType::Effect) {}

EffectOnPlayerGearup::~EffectOnPlayerGearup() {}

void EffectOnPlayerGearup::Initialize() {}

void EffectOnPlayerGearup::Finalize() {}

void EffectOnPlayerGearup::UpdateEntity(GameEntity* /*_entity*/) {
    GameEntity* player         = getUniqueEntity("Player");
    PlayerStatus* playerStatus = getComponent<PlayerStatus>(player);

    /*  DistortionEffectParam* distortionEffectParam = getComponent<DistortionEffectParam>(_entity);
      auto& shockWaveRings                         = distortionEffectParam->getDistortionObjects();*/

    // 前フレームにPlayerがGearupしていたかどうか
    if (playerStatus->isGearUp()) {
        GameEntity* gameCamera = getUniqueEntity("GameCamera");
        if (gameCamera == nullptr) {
            LOG_ERROR("GameCamera is not found.");
            return;
        }
        CameraAction* cameraAction = getComponent<CameraAction>(gameCamera);
        cameraAction->Play();
    }
}
