#pragma once
#include "component/IComponent.h"

/// stl
#include <deque>

/// math
#include "math/MyEasing.h"
#include "math/Vector3.h"

/// <summary>
/// Splineの制御点を管理するコンポーネント
/// </summary>
struct SplinePoints
    : public OriGine::IComponent {
    friend void to_json(nlohmann::json& j, const SplinePoints& c);
    friend void from_json(const nlohmann::json& j, SplinePoints& c);

public:
    SplinePoints();
    ~SplinePoints() override;

    void Initialize(OriGine::Entity* _entity) override;
    void Edit(OriGine::Scene* _scene, OriGine::Entity* _entity, const std::string& _parentLabel) override;
    void Finalize() override;

public:
    bool isCrossMesh_       = true;
    OriGine::EaseType widthEaseType_ = OriGine::EaseType::EaseInOutSine;
    OriGine::EaseType uvEaseType_    = OriGine::EaseType::EaseInOutSine;

    int32_t capacity_      = 100;
    int32_t segmentDivide_ = 16;

    OriGine::Vec2f startUv_       = OriGine::Vec2f(0.0f, 0.0f);
    OriGine::Vec2f endUv_         = OriGine::Vec2f(1.0f, 1.0f); // endは splineの最長時のメッシュに依存する
    float segmentLength_ = 0.5f;
    float startWidth_    = 0.2f;
    float endWidth_      = 0.2f;

    float fadeoutTime_  = 0.5f;
    float fadeoutTimer_ = 0.f;

    std::deque<OriGine::Vec3f> points_;

public:
    /// <summary>
    /// 制御点を追加する(capacityを超過した場合,popfrontされる)
    /// </summary>
    /// <param name="_point"></param>
    void PushPoint(const OriGine::Vec3f& _point);
};
