#include "BillboardTransform.h"

/// engine
// camera
#include "camera/CameraManager.h"

// component
#include "component/transform/Transform.h"

using namespace OriGine;

void BillboardTransform::Initialize() {}
void BillboardTransform::Finalize() {}

void BillboardTransform::UpdateEntity(OriGine::EntityHandle _handle) {
    int32_t componentIndex = 0;

    const CameraTransform& cameraTransform = CameraManager::GetInstance()->GetTransform();

    auto transforms = GetComponents<OriGine::Transform>(_entity);
    for (auto& transform : *transforms) {
        // カメラの視線ベクトル（ワールド空間でのカメラ→ビルボード方向、正規化済み）
        OriGine::Vec3f lookAt = OriGine::Vec3f(transform.translate - cameraTransform.translate).normalize(); // D3DXVECTOR3 LookAt(-cx, -cy, -cz);

        // ビルボードのローカルY軸（上方向）
        OriGine::Vec3f nAxis = OriGine::Vec3f(0.0f, 0.f, 1.0f);

        // 回転軸を計算（外積）
        OriGine::Vec3f normal = OriGine::Vec3f::Cross(nAxis, lookAt);

        // 回転角度を計算（内積）
        float dot   = OriGine::Vec3f::Dot(lookAt, nAxis);
        dot         = std::clamp(dot, -1.0f, 1.0f); // acosの範囲外防止
        float angle = std::acos(dot);

        // 回転軸を正規化
        if (normal.lengthSq() > 0.0f) {
            normal = normal.normalize();
        } else {
            // lookAtとnAxisが平行な場合、回転不要
            normal = OriGine::Vec3f(0.0f, 1.0f, 0.0f);
            angle  = 0.0f;
        }

        // クォータニオン作成
        transform.rotate = Quaternion::RotateAxisAngle(normal, angle);
        transform.UpdateMatrix();
    }

    while (true) {

        // 次のコンポーネントへ
        ++componentIndex;
    }
}
