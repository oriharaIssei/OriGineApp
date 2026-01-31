#include "RailInitializeSystem.h"

/// ECS
// component
#include "component/collision/collider/CapsuleCollider.h"
#include "component/gimmick/RailPoints.h"
// system
#include "system/collision/CollisionCheckSystem.h"

using namespace OriGine;

RailInitializeSystem::RailInitializeSystem()
    : ISystem(SystemCategory::Initialize) {}
RailInitializeSystem::~RailInitializeSystem() {}

void RailInitializeSystem::Initialize() {}
void RailInitializeSystem::Finalize() {}

void RailInitializeSystem::UpdateEntity(OriGine::EntityHandle _handle) {
    // RailPoints コンポーネントを取得
    auto* railPoints = GetComponent<RailPoints>(_handle);
    if (railPoints == nullptr) {
        return;
    }
    if (railPoints->points.size() < 2) {
        return;
    }

    // CapsuleCollider コンポーネントを追加
    // Colliderが既に存在する場合は何もしない
    auto* haveColliderCheck = GetComponent<CapsuleCollider>(_handle);
    if (haveColliderCheck) {
        return;
    }
    // 各セグメントごとに CapsuleCollider を追加
    for (int i = 0; i < static_cast<int>(railPoints->points.size() - 1); ++i) {
        const auto& segmentStart = railPoints->points[i];
        const auto& segmentEnd   = railPoints->points[i + 1];
        // CapsuleCollider コンポーネントを追加
        auto capsuleColliderHandle = AddComponent<CapsuleCollider>(_handle);
        auto* capsuleCollider      = GetComponent<CapsuleCollider>(capsuleColliderHandle);
        if (capsuleCollider) {
            capsuleCollider->SetLocalStart(segmentStart);
            capsuleCollider->SetLocalEnd(segmentEnd);
            capsuleCollider->SetLocalRadius(railPoints->radius);
            capsuleCollider->CalculateWorldShape();
        }
    }
}
