#include "EffectOnPlayerGearup.h"

#define ENGINE_ECS
#define DELTA_TIME
#include "EngineInclude.h"

#include "component/Player/PlayerStatus.h"

// FOVをイージングする関数
static float FovYEasing(float t) {
    auto smoothstep = [](float x) {
        return x * x * (3 - 2 * x);
    };
    if (t < 0.5f) {
        return smoothstep(t / 0.5f); // 0.0～0.5で0→1
    } else {
        return smoothstep(1.0f - (t - 0.5f) / 0.5f); // 0.5～1.0で1→0
    }
}

EffectOnPlayerGearup::EffectOnPlayerGearup() : ISystem(SystemType::Effect) {}

EffectOnPlayerGearup::~EffectOnPlayerGearup() {}

void EffectOnPlayerGearup::Initialize() {}

void EffectOnPlayerGearup::Finalize() {}

void EffectOnPlayerGearup::UpdateEntity(GameEntity* _entity) {
    GameEntity* player                           = getUniqueEntity("Player");
    PlayerStatus* playerStatus                   = getComponent<PlayerStatus>(player);
    DistortionEffectParam* distortionEffectParam = getComponent<DistortionEffectParam>(_entity);
    auto& shockWaveRings                         = distortionEffectParam->getDistortionObjects();

    if (distortionEffectParam == nullptr) {
        return;
    }

    //! TODO : Updateにかく処理 は Updateに

    // 本当ならUpdateに書かないと行けないけどめんどくさいから ここに書いちゃうよーん
    //  playerのギアレベルが3以下の場合は衝撃波を発生させない
    if (playerStatus->getGearLevel() > 3) {

        // ギアレベルが上がった瞬間を検知するために、前フレームの状態を保存
        if (!prePlayerIsGearup_ && playerStatus->isGearUp()) {
            isPlay_ = playerStatus->isGearUp();

            // shockWaveRingの初期化
            for (auto& [object, type] : shockWaveRings) {
                if (type != PrimitiveType::Ring) {
                    continue;
                }
                if (object == nullptr) {
                    continue;
                }

                RingRenderer* ringRenderer = dynamic_cast<RingRenderer*>(object.get());

                // 初期位置
                Transform* playerTransform             = getComponent<Transform>(player);
                Vec3f spawnPos                         = Vec3f(playerTransform->translate + shockWaveOffset_) * MakeMatrix::RotateQuaternion(playerTransform->rotate);
                ringRenderer->getTransform().translate = spawnPos;
                ringRenderer->getTransform().rotate    = playerTransform->rotate;

                ringRenderer->getPrimitive().setInnerRadius(minInnerRadius_);
                ringRenderer->getPrimitive().setOuterRadius(minouterRadius_);

                currentTime_ = 0.f; // 時間をリセット
            }
        }
        prePlayerIsGearup_ = playerStatus->isGearUp();
    }

    if (!isPlay_) {
        // shockWaveRingのを見えない位置に隠す
        for (auto& [object, type] : shockWaveRings) {
            if (type != PrimitiveType::Ring) {
                continue;
            }
            if (object == nullptr) {
                continue;
            }

            RingRenderer* ringRenderer = dynamic_cast<RingRenderer*>(object.get());

            // 初期位置
            ringRenderer->getTransform().translate[Y] = -10000.f;
        }
        return;
    }

    currentTime_ += getMainDeltaTime();
    float t = currentTime_ / maxTime_;

    if (t >= 1.f) {
        isPlay_ = false;
    }

    // FOVの補間
    GameEntity* camera               = getUniqueEntity("GameCamera");
    CameraTransform* cameraTransform = getComponent<CameraTransform>(camera);
    cameraTransform->fovAngleY       = std::lerp(defaultFovY_, maxFovY_, FovYEasing(t));
    // Ring
    for (auto& [object, type] : shockWaveRings) {
        if (type != PrimitiveType::Ring) {
            continue;
        }
        if (object == nullptr) {
            continue;
        }

        RingRenderer* ringRenderer = dynamic_cast<RingRenderer*>(object.get());
        ringRenderer->getPrimitive().setInnerRadius(std::lerp(minInnerRadius_, maxInnerRadius_, t));
        ringRenderer->getPrimitive().setOuterRadius(std::lerp(minouterRadius_, maxouterRadius_, t));

        ringRenderer->createMesh(&ringRenderer->getMeshGroup()->back());
    }
}
