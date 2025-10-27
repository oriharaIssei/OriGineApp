#pragma once
#include "component/IComponent.h"

/// stl
#include <deque>

/// math
#include "math/Vector3.h"

/// <summary>
/// Splineの制御点を管理するコンポーネント
/// </summary>
struct SplinePoints
    : public IComponent {
    friend void to_json(nlohmann::json& j, const SplinePoints& c);
    friend void from_json(const nlohmann::json& j, SplinePoints& c);

public:
    SplinePoints();
    ~SplinePoints() override;

    void Initialize(Entity* _entity) override;
    void Edit(Scene* _scene, Entity* _entity, const std::string& _parentLabel) override;
    void Finalize() override;

public:
    int32_t capacity_      = 100;
    int32_t segmentDivide_ = 16;
    Vec2f startUv_         = Vec2f(0.0f, 0.0f);
    Vec2f endUv_           = Vec2f(1.0f, 1.0f); // endは splineの最長時のメッシュに依存する
    float segmentLength_   = 0.5f;
    float width_           = 0.2f;

    float fadeoutTime_  = 0.5f;
    float fadeoutTimer_ = 0.f;

    std::deque<Vec3f> points_;

    /// <summary>
    /// 制御点を追加する(capacityを超過した場合,popfrontされる)
    /// </summary>
    /// <param name="_point"></param>
    void pushPoint(const Vec3f& _point);
};
