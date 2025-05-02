#pragma once

#include "component/IComponent.h"
#include <Vector3.h>
class ExplotionCollision
    : public IComponent {
    friend void to_json(nlohmann::json& _json, const ExplotionCollision& _explotionCollision);
    friend void from_json(const nlohmann::json& _json, ExplotionCollision& _explotionCollision);

private: // variables
    bool isAlive_ = true;

    /// time
    float adaptTime_; // 当たり適応時間
    /// pos
    Vec3f positionOffset_;
    /// size
    float collisionRadius_;

public:
    ExplotionCollision() {}
    virtual ~ExplotionCollision() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Finalize();

    void TimeDecrement();

public: // accsessor
    /// getter
    float GetAdaptTime() const { return adaptTime_; }
    float GetCollisionRadius() const { return collisionRadius_; }
    Vec3f GetPositionOffset() const { return positionOffset_; }
    /// setter
    void SetAdaptTime(const float& time) { adaptTime_ = time; }
    void SetOffset(const Vec3f& time) { positionOffset_ = time; }
    void SetCollisionRadius(const float& time) { collisionRadius_ = time; }
};
