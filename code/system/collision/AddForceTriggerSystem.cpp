#include "AddForceTriggerSystem.h"

/// stl
#include <unordered_set>

/// ECS
// component
#include "component/AddForceComponent.h"
#include "component/collision/collider/AABBCollider.h"
#include "component/collision/collider/ObbCollider.h"
#include "component/collision/collider/SphereCollider.h"
#include "component/physics/Rigidbody.h"

using namespace OriGine;

AddForceTriggerSystem::AddForceTriggerSystem() : ISystem(SystemCategory::Collision) {}
AddForceTriggerSystem::~AddForceTriggerSystem() {}

void AddForceTriggerSystem::Initialize() {}
void AddForceTriggerSystem::Finalize() {}

void AddForceTriggerSystem::UpdateEntity(OriGine::EntityHandle _handle) {
    auto addForceComp = GetComponent<AddForceComponent>(_handle);
    if (!addForceComp) {
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

    // 衝突したエンティティにジャンプ力を加える
    for (const auto& otherHandle : collidedEntities) {
        auto rigidbodyComp = GetComponent<Rigidbody>(otherHandle);
        if (rigidbodyComp) {
            // 上方向にジャンプ力を加える
            rigidbodyComp->SetVelocity(rigidbodyComp->GetVelocity() + addForceComp->addForce_);
        }
    }
}
