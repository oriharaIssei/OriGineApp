#include "ShadowPlaneSyncPlayerPosition.h"

#include "component/transform/Transform.h"

using namespace OriGine;

ShadowPlaneSyncPlayerPosition::ShadowPlaneSyncPlayerPosition() : ISystem(SystemCategory::Movement) {}

void ShadowPlaneSyncPlayerPosition::Initialize() {}
void ShadowPlaneSyncPlayerPosition::Finalize() {}

void ShadowPlaneSyncPlayerPosition::UpdateEntity(EntityHandle _entity) {
    // プレイヤーエンティティを取得
    EntityHandle playerEntity = GetUniqueEntity("Player");
    // プレイヤーのTransformコンポーネントを取得
    auto* playerTransform = GetComponent<Transform>(playerEntity);
    if (playerTransform == nullptr) {
        LOG_ERROR("Player TransformComponent not found.");
        return;
    }

    // 影面プレーンのTransformコンポーネントを取得
    auto* shadowPlaneTransform = GetComponent<Transform>(_entity);
    if (shadowPlaneTransform == nullptr) {
        LOG_ERROR("ShadowPlane TransformComponent not found.");
        return;
    }

    // 影面プレーンの位置をプレイヤーの位置に同期させる
    Vector3 playerPosition          = playerTransform->GetWorldTranslate();
    shadowPlaneTransform->translate = playerPosition;
    shadowPlaneTransform->UpdateMatrix();
}
