#include "PlayerSpeedFor3dUI.h"

/// ECS
// component
#include "component/material/Material.h"
#include "component/physics/Rigidbody.h"
#include "component/ui/SpeedFor3dUIComponent.h"

/// math
#include "math/mathEnv.h"

PlayerSpeedFor3dUI::PlayerSpeedFor3dUI() : ISystem(SystemCategory::Effect) {}
PlayerSpeedFor3dUI::~PlayerSpeedFor3dUI() {}

void PlayerSpeedFor3dUI::Initialize() {}
void PlayerSpeedFor3dUI::Finalize() {}

void PlayerSpeedFor3dUI::UpdateEntity(Entity* _entity) {
    auto playerEnt = GetUniqueEntity("Player");
    if (!playerEnt) {
        return;
    }
    // component取得
    auto rigidbodyComp = GetComponent<Rigidbody>(playerEnt);
    auto speedUIComp   = GetComponent<SpeedFor3dUIComponent>(_entity);
    if (!speedUIComp || !rigidbodyComp) {
        return;
    }
    auto ui3dEntity = GetEntity(speedUIComp->GetPlaneEntityId());
    if (!ui3dEntity) {
        return;
    }

    // 速度取得
    float speed = Vec2f(rigidbodyComp->GetVelocity(X),
        rigidbodyComp->GetVelocity(Z))
                      .length();
    // 各桁の数字を抽出
    std::vector<int> digits = CalculateDigitsFromFloat(
        speed,
        speedUIComp->digitInteger,
        speedUIComp->digitDecimal);

    // uv を設定
    for (int32_t i = 0; i < speedUIComp->digitInteger + speedUIComp->digitDecimal; ++i) {
        auto material = GetComponent<Material>(ui3dEntity, i);
        if (!material) {
            continue; // スプライトがない場合は何もしない
        }

        if (int32_t(digits.size()) <= i) {
            return;
        }

        material->uvTransform_.translate_ = Vec2f(0.1f * digits[i], 0.f);
    }
}
