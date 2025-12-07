#include "UI3DUpdateSystem.h"

/// ECS
// component
#include "component/transform/Transform.h"

Ui3dUpdateSystem::Ui3dUpdateSystem() : ISystem(SystemCategory::Movement) {}

Ui3dUpdateSystem::~Ui3dUpdateSystem() {}

void Ui3dUpdateSystem::Initialize() {}

void Ui3dUpdateSystem::Finalize() {}

void Ui3dUpdateSystem::UpdateEntity(Entity* _entity) {
    auto playerEnt = GetUniqueEntity("Player");
    if (!playerEnt) {
        return;
    }
    auto playerTransform = GetComponent<Transform>(playerEnt);
    auto transform       = GetComponent<Transform>(_entity);
    auto camTransform    = transform->parent; // Camera の Transform
    if (!transform || !playerTransform || !camTransform)
        return;

    // --- ワールド方向 ---
    Vec3f worldDir = transform->GetWorldTranslate() - playerTransform->GetWorldTranslate();
    worldDir       = worldDir.normalize();

    // --- カメラのローカル空間へ変換 ---
    Matrix4x4 invCam = MakeMatrix4x4::RotateQuaternion(camTransform->rotate).inverse();
    Vec3f localDir   = Vec3f(worldDir * invCam).normalize();

    // --- UI のローカル回転を設定 ---
    transform->rotate = Quaternion::LookAt(localDir, axisY);

    // --- 傾き補正 ---
    constexpr float kTilt = 0.33f; // 0=正面固定, 1=完全にPlayer方向
    transform->rotate     = Slerp(
        Quaternion::Identity(),
        Quaternion::LookAt(localDir, axisY),
        kTilt);

    transform->UpdateMatrix();
}
