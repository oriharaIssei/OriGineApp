#include "RailInitializeSystem.h"

/// engine
#include "scene/Scene.h"

/// ECS
// component
#include "component/collision/collider/CapsuleCollider.h"
#include "component/collision/CollisionPushBackInfo.h"
#include "component/gimmick/RailPoints.h"
// system
#include "system/collision/CollisionCheckSystem.h"
#include <nameof.h>

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
    auto& colliders = GetComponents<CapsuleCollider>(_handle);

    int32_t segmentCount = static_cast<int32_t>(railPoints->points.size()) - 1;
    int32_t diff         = static_cast<int32_t>(colliders.size()) - segmentCount;
    if (diff < 0) {
        // 足りない分を追加
        for (size_t i = 0; i < static_cast<size_t>(-diff); ++i) {
            AddComponent<CapsuleCollider>(_handle);
        }

        colliders = GetComponents<CapsuleCollider>(_handle);
    }

    // 各セグメントごとに CapsuleCollider を追加
    for (int i = 0; i < segmentCount; ++i) {
        const auto& segmentStart = railPoints->points[i];
        const auto& segmentEnd   = railPoints->points[i + 1];

        // CapsuleCollider コンポーネントを追加
        auto* capsuleCollider = GetComponent<CapsuleCollider>(_handle, i);
        if (capsuleCollider) {
            capsuleCollider->SetLocalStart(segmentStart);
            capsuleCollider->SetLocalEnd(segmentEnd);
            capsuleCollider->SetLocalRadius(railPoints->radius);
            capsuleCollider->CalculateWorldShape();
        }
    }

    // pushbackInfoを持っていなければ追加
    auto* info = GetComponent<CollisionPushBackInfo>(_handle);
    if (!info) {
        AddComponent(_handle, nameof<CollisionPushBackInfo>());
    }

    // Systemに登録
    GetScene()->GetSystem(nameof<CollisionCheckSystem>())->AddEntity(_handle);
}
