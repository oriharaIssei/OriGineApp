#include "PlayerPathSplineGenerator.h"

/// engine
#define DELTA_TIME
#include "EngineInclude.h"

// component
#include "component/Player/State/PlayerState.h"

#include "component/spline/SplinePoints.h"
#include "component/transform/Transform.h"

/// math
#include <math/mathEnv.h>

PlayerPathSplineGenerator::PlayerPathSplineGenerator() : ISystem(SystemCategory::Movement) {}
PlayerPathSplineGenerator::~PlayerPathSplineGenerator() {}

void PlayerPathSplineGenerator::Initialize() {}
void PlayerPathSplineGenerator::Finalize() {}

void PlayerPathSplineGenerator::UpdateEntity(Entity* _entity) {
    constexpr float kThresholdSplitOrMerger = 0.3f;

    auto playerEntity = getUniqueEntity("Player");
    if (playerEntity == nullptr) {
        return;
    }
    auto playerTransform = getComponent<Transform>(playerEntity);
    if (playerTransform == nullptr) {
        return;
    }

    auto splinePoints = getComponent<SplinePoints>(_entity);
    if (splinePoints == nullptr) {
        return;
    }

    // gearLevelが一定以上ならポイント追加処理
    constexpr int32_t kThresholdTrailActivateGearLevel = 1;
    PlayerState* state                                 = getComponent<PlayerState>(playerEntity);
    if (state->getGearLevel() > kThresholdTrailActivateGearLevel) {

        // segmentLength_ の安全チェック
        if (splinePoints->segmentLength_ <= kEpsilon) {
            return; // 無効な設定
        }

        Vec3f newPoint = playerTransform->getWorldTranslate();

        // 最低4点確保
        if (splinePoints->points_.size() < 4) {
            int32_t diff = 4 - static_cast<int32_t>(splinePoints->points_.size());
            for (int32_t i = 0; i < diff; ++i) {
                splinePoints->pushPoint(newPoint);
            }
        }

        // --- 新しいポイントの追加処理 ---
        const float segLen     = splinePoints->segmentLength_;
        const Vec3f& lastPoint = splinePoints->points_.back();
        Vec3f distance         = newPoint - lastPoint;
        float distLen          = distance.length();

        if (distLen - segLen >= kThresholdSplitOrMerger) {
            Vec3f direction = distance;
            if (distLen > kEpsilon) {
                direction /= distLen;
            } else {
                direction = Vec3f(0.0f, 0.0f, 0.0f);
            }

            // 分割して追加
            int32_t segmentsToAdd = static_cast<int32_t>(distLen / segLen);
            for (int32_t i = 1; i <= segmentsToAdd; ++i) {
                Vec3f dividedPoint = lastPoint + direction * (segLen * static_cast<float>(i));
                splinePoints->pushPoint(dividedPoint);
            }
        }

        // --- 新しい配列で再構築 ---
        std::deque<Vec3f> newPoints;

        for (int32_t i = 0; i < static_cast<int32_t>(splinePoints->points_.size()) - 1; ++i) {
            Vec3f current = splinePoints->points_[i];
            Vec3f next    = splinePoints->points_[i + 1];
            Vec3f delta   = next - current;
            float len     = delta.length();

            // 現在の点は常に残す
            newPoints.push_back(current);

            if (len - segLen > segLen * kThresholdSplitOrMerger) {
                // 長すぎる → 分割
                int divs = std::max(1, static_cast<int>(len / segLen));

                Vec3f dir = Vec3f(0.0f, 0.0f, 0.0f);
                if (len > kEpsilon) {
                    dir = delta / len;
                }

                for (int j = 1; j < divs; ++j) {
                    newPoints.push_back(current + dir * (segLen * static_cast<float>(j)));
                }
            } else if (len - segLen < segLen * kThresholdSplitOrMerger && (i + 1) < static_cast<int>(splinePoints->points_.size() - 1)) {
                // 短すぎる → 統合
                Vec3f merged     = (current + next) * 0.5f;
                newPoints.back() = merged;
                ++i; // 次をスキップ（統合済み）
            }
        }
        // 最後の点を追加
        newPoints.push_back(splinePoints->points_.back());

        // 容量制限
        while (newPoints.size() > splinePoints->capacity_) {
            newPoints.pop_front();
        }

        // 差し替え
        splinePoints->points_ = std::move(newPoints);

    } else {
        // IDLE状態なら fadeoutTimer を進める
        // fadeoutTimer 処理
        splinePoints->fadeoutTimer_ += getMainDeltaTime();
        if (splinePoints->fadeoutTimer_ >= splinePoints->fadeoutTime_) {
            splinePoints->fadeoutTimer_ = 0.0f;
            if (!splinePoints->points_.empty()) {
                splinePoints->points_.pop_front();
            }
        }
    }
}
