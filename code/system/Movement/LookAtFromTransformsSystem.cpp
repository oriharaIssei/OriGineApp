#include "LookAtFromTransformsSystem.h"

/// ECS
// component
#include "component/LookAtFromTransforms.h"
#include "component/transform/Transform.h"

using namespace OriGine;

LookAtFromTransformsSystem::LookAtFromTransformsSystem() : ISystem(OriGine::SystemCategory::Effect) {}
LookAtFromTransformsSystem::~LookAtFromTransformsSystem() {}

void LookAtFromTransformsSystem::Initialize() {}
void LookAtFromTransformsSystem::Finalize() {}

void LookAtFromTransformsSystem::UpdateEntity(OriGine::Entity* _entity) {
    auto lookAtComps = GetComponents<LookAtFromTransforms>(_entity);
    if (!lookAtComps) {
        return;
    }

    // 適応する Transform コンポーネントを取得
    auto transform = GetComponent<OriGine::Transform>(_entity);

    for (auto& lookAtComp : *lookAtComps) {
        auto fromTransformComp = GetComponent<OriGine::Transform>(GetEntity(lookAtComp.fromTransformEntity));
        auto toTransformComp   = GetComponent<OriGine::Transform>(GetEntity(lookAtComp.toTransformEntity));
        if (!fromTransformComp || !toTransformComp) {
            continue;
        }
        // 注視点の方向を計算
        OriGine::Vec3f direction = toTransformComp->GetWorldTranslate() - fromTransformComp->GetWorldTranslate();
        direction       = direction.normalize();

        Quaternion lookAtRotate = Quaternion::LookAt(direction, axisY);
        OriGine::Vec3f eulerAngles       = lookAtRotate.ToEulerAngles();

        if (!lookAtComp.rotateAxis.HasFlag(LookAtFromTransforms::RotateAxis::X)) {
            eulerAngles[X] = 0.f;
        }
        if (!lookAtComp.rotateAxis.HasFlag(LookAtFromTransforms::RotateAxis::Y)) {
            eulerAngles[Y] = 0.f;
        }
        if (!lookAtComp.rotateAxis.HasFlag(LookAtFromTransforms::RotateAxis::Z)) {
            eulerAngles[Z] = 0.f;
        }

        transform->rotate = Quaternion::FromEulerAngles(eulerAngles);
    }
}
