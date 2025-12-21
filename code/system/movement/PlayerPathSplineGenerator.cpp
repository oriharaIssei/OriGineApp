#include "PlayerPathSplineGenerator.h"

/// engine
#define DELTA_TIME
#include "EngineInclude.h"

// component
#include "component/physics/Rigidbody.h"
#include "component/player/state/PlayerState.h"
#include "component/spline/SplinePoints.h"
#include "component/transform/Transform.h"

/// math
#include <math/mathEnv.h>

using namespace OriGine;

PlayerPathSplineGenerator::PlayerPathSplineGenerator() : ISystem(OriGine::SystemCategory::Movement) {}
PlayerPathSplineGenerator::~PlayerPathSplineGenerator() {}

void PlayerPathSplineGenerator::Initialize() {}
void PlayerPathSplineGenerator::Finalize() {}

void PlayerPathSplineGenerator::UpdateEntity(OriGine::EntityHandle _handle) {
    constexpr float kThresholdSplitOrMerger = 0.3f;

    auto splinePoints = GetComponent<SplinePoints>(_entity);
    if (splinePoints == nullptr) {
        return;
    }

    auto playerEntity = GetUniqueEntity("Player");
    if (playerEntity == nullptr) {
        return;
    }
    auto playerTransform = GetComponent<OriGine::Transform>(playerEntity);
    if (playerTransform == nullptr) {
        return;
    }

    // 速度が 0以上ならポイント追加処理
    constexpr float kFadeOutThresholdSpeed = 8.f;
    Rigidbody* rigidBody                   = GetComponent<Rigidbody>(playerEntity);
    if (rigidBody->GetVelocity().lengthSq() > kFadeOutThresholdSpeed * kFadeOutThresholdSpeed) {

        // segmentLength の安全チェック
        if (splinePoints->commonSettings.segmentLength <= kEpsilon) {
            return; // 無効な設定
        }

        OriGine::Vec3f newPoint = playerTransform->GetWorldTranslate();

        // 最低4点確保
        if (splinePoints->points.size() < 4) {
            int32_t diff = 4 - static_cast<int32_t>(splinePoints->points.size());
            for (int32_t i = 0; i < diff; ++i) {
                splinePoints->PushPoint(newPoint);
            }
        }

        // --- 新しいポイントの追加処理 ---
        const float segLen              = splinePoints->commonSettings.segmentLength;
        const OriGine::Vec3f& lastPoint = splinePoints->points.back();
        OriGine::Vec3f distance         = newPoint - lastPoint;
        float distLen                   = distance.length();

        if (distLen - segLen >= kThresholdSplitOrMerger) {
            OriGine::Vec3f direction = distance;
            if (distLen > kEpsilon) {
                direction /= distLen;
            } else {
                direction = OriGine::Vec3f(0.0f, 0.0f, 0.0f);
            }

            // 分割して追加
            // あまりにも離れている場合は飛ばす
            constexpr int32_t kSkipThreshold = 18;
            int32_t segmentsToAdd            = static_cast<int32_t>(distLen / segLen);
            segmentsToAdd                    = (std::min)(segmentsToAdd, kSkipThreshold);
            for (int32_t i = 1; i <= segmentsToAdd; ++i) {
                OriGine::Vec3f dividedPoint = lastPoint + direction * (segLen * static_cast<float>(i));
                splinePoints->PushPoint(dividedPoint);
            }
        }

        // --- 新しい配列で再構築 ---
        std::deque<OriGine::Vec3f> newPoints;

        for (int32_t i = 0; i < static_cast<int32_t>(splinePoints->points.size()) - 1; ++i) {
            OriGine::Vec3f current = splinePoints->points[i];
            OriGine::Vec3f next    = splinePoints->points[i + 1];
            OriGine::Vec3f delta   = next - current;
            float len              = delta.length();

            // 現在の点は常に残す
            newPoints.push_back(current);

            if (len - segLen > segLen * kThresholdSplitOrMerger) {
                // 長すぎる → 分割
                int divs = (std::max)(1, static_cast<int>(len / segLen));

                OriGine::Vec3f dir = OriGine::Vec3f(0.0f, 0.0f, 0.0f);
                if (len > kEpsilon) {
                    dir = delta / len;
                }

                for (int j = 1; j < divs; ++j) {
                    newPoints.push_back(current + dir * (segLen * static_cast<float>(j)));
                }
            } else if (len - segLen < segLen * kThresholdSplitOrMerger && (i + 1) < static_cast<int>(splinePoints->points.size() - 1)) {
                // 短すぎる → 統合
                OriGine::Vec3f merged = (current + next) * 0.5f;
                newPoints.back()      = merged;
                ++i; // 次をスキップ（統合済み）
            }
        }
        // 最後の点を追加
        newPoints.push_back(splinePoints->points.back());

        // 容量制限
        while (newPoints.size() > splinePoints->capacity) {
            newPoints.pop_front();
        }

        // 差し替え
        splinePoints->points = std::move(newPoints);

        // fadeoutTimer リセット(次回の削除がすぐに始まるように)
        splinePoints->commonSettings.fadeoutTimer = splinePoints->commonSettings.fadeoutTime;
    } else {
        // 古いポイントの削除処理
        if (!splinePoints->points.empty()) {
            // fadeoutTimer 処理
            splinePoints->commonSettings.fadeoutTimer += GetMainDeltaTime();
            if (splinePoints->commonSettings.fadeoutTimer >= splinePoints->commonSettings.fadeoutTime) {
                splinePoints->commonSettings.fadeoutTimer = 0.0f;
                splinePoints->points.pop_front();
            }
        }
    }
}
