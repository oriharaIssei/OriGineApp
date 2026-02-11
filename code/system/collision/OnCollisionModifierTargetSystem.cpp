#include "OnCollisionModifierTargetSystem.h"

/// stl
#include <unordered_set>

/// engine
#include "scene/Scene.h"

/// ECS
// component
#include "component/collision/collider/AABBCollider.h"
#include "component/collision/collider/ObbCollider.h"
#include "component/collision/collider/SphereCollider.h"
#include "component/physics/Rigidbody.h"
#include "component/SpeedModifiers.h"
// system
#include "system/Movement/ApplySpeedModifiers.h"

/// event
#include "event/AddForceEvent.h"

using namespace OriGine;

OnCollisionModifierTargetSystem::OnCollisionModifierTargetSystem() : ISystem(SystemCategory::Collision) {}
OnCollisionModifierTargetSystem::~OnCollisionModifierTargetSystem() {}

void OnCollisionModifierTargetSystem::Initialize() {}
void OnCollisionModifierTargetSystem::Finalize() {}

void OnCollisionModifierTargetSystem::UpdateEntity(OriGine::EntityHandle _handle) {
    auto speedModifiers = GetComponent<SpeedModifiers>(_handle);
    if (!speedModifiers) {
        return;
    }

    // ジャンプパッドと衝突したエンティティを取得
    auto& sphereColliders = GetComponents<SphereCollider>(_handle);
    auto& aabbColliders   = GetComponents<AABBCollider>(_handle);
    auto& obbColliders    = GetComponents<OBBCollider>(_handle);

    std::unordered_set<EntityHandle> collidedEntities;
    for (auto& collider : sphereColliders) {
        for (const auto& [otherHandle, state] : collider.GetCollisionStateMap()) {
            if (state == CollisionState::Enter) {
                collidedEntities.insert(otherHandle);
            }
        }
    }
    for (auto& collider : aabbColliders) {
        for (const auto& [otherHandle, state] : collider.GetCollisionStateMap()) {
            if (state == CollisionState::Enter) {
                collidedEntities.insert(otherHandle);
            }
        }
    }
    for (auto& collider : obbColliders) {
        for (const auto& [otherHandle, state] : collider.GetCollisionStateMap()) {
            if (state == CollisionState::Enter) {
                collidedEntities.insert(otherHandle);
            }
        }
    }

    // 衝突したエンティティのRigidbodyに対してSpeedModifierを適応するEntityを作成する
    for (const auto& otherHandle : collidedEntities) {
        auto rigidbodyComp = GetComponent<Rigidbody>(otherHandle);
        if (rigidbodyComp) {
            EntityHandle createdEntity = CreateEntity("SpeedModifier");
            ComponentHandle modifier   = AddComponent<SpeedModifiers>(createdEntity);
            auto* modifierComp         = GetComponent<SpeedModifiers>(modifier);
            if (modifierComp) {
                *modifierComp                       = *speedModifiers;
                modifierComp->targetRigidbodyHandle = rigidbodyComp->GetHandle();
                modifierComp->beforeSpeed           = rigidbodyComp->GetVelocity().length();

                // systemに登録
                GetScene()->GetSystem(nameof<ApplySpeedModifiers>())->AddEntity(createdEntity);
            }
        }
    }
}
