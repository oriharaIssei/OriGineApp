#include "EffectByBlockDrawSystem.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib

// include

#include"component/transform/CameraTransform.h"
#include "engine/EngineInclude.h"
#include <cstdint>

EffectByBlockDrawSystem::EffectByBlockDrawSystem()
    : ISystem(SystemType::Movement) {}

EffectByBlockDrawSystem::~EffectByBlockDrawSystem() {}

void EffectByBlockDrawSystem::Initialize() {
  
}

void EffectByBlockDrawSystem::Finalize() {}

void EffectByBlockDrawSystem::UpdateEntity(GameEntity* _entity) {

    if (!_entity) {
        return;
    }

    /// component取得
    Transform* transform     = getComponent<Transform>(_entity);
    SpriteRenderer* spriteRender = getComponent<SpriteRenderer>(_entity);

    if (!transform || !spriteRender) { // Componentが存在しない場合の早期リターン
        return;
    }

      // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* PlayerEntity                  = ecsManager->getUniqueEntity("Player");

    if (!PlayerEntity) {
        return;
    }

    /*/// component取得
    CameraTransform* cameraStatus = getComponent<CameraTransform>(PlayerEntity);

    if (!cameraStatus) {
        return;
    }*/

   // // ワールド座標からスクリーン座標に変換
   // Vec3f positionScreen = KScreenTransform(transform->translate, *cameraStatus);
   // // Vector2に格納
   // Vec2f positionScreenV2(Vec2f(positionScreen[X] - 5.0f, positionScreen[Y] - 5.0f));
   // // Hpバーの座標確定
   // Vec2f hpBarPosition = positionScreenV2;
   // // Hpバーのサイズ
   ///* hpbar_->SetSize(hpbarSize_);*/
   // // HPBarスプライト
   // spriteRender->setTranslate(hpBarPosition);
}

//
//Vec3f EffectByBlockDrawSystem::KScreenTransform(Vec3f worldPos, const CameraTransform& viewProjection) {
//    // ビューポート行列
//    Matrix4x4 matViewport = MakeMatrix::ViewPort(0.0f, 0.0f, 1280.0f,720.0f, 0.0f, 1.0f);
//    // ビュー行列とプロジェクション行列、ビューポート行列を合成する
//    Matrix4x4 matViewProjectionViewport = viewProjection.projectionMat * viewProjection.projectionMat * matViewport;
//    // ワールド→スクリーン変換
//    return KMatrixTransform(worldPos, matViewProjectionViewport);
//}
//
//
