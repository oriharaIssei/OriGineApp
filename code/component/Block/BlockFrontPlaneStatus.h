#pragma once

#include "component/IComponent.h"
#include "KetaEasing.h"
#include <array>
#include <cstdint>
#include <Entity.h>
#include <Vector2.h>
#include <Vector3.h>



class BlockFrontPlaneStatus
    : public IComponent {
    friend void to_json(nlohmann::json& _json, const BlockFrontPlaneStatus& _component);
    friend void from_json(const nlohmann::json& _json, BlockFrontPlaneStatus& _component);

public:
    enum class PlaneStep {
        NONE,
        CLOSE,
        END,
    };

private: // variables
    bool isAlive_ = true;

    float rotateSpeed_;
    float rotate_;
    Vec3f scale_;
    Vec3f startScale_;
    Easing closeEasing_;
    bool isDeath_;
    PlaneStep planestep_=PlaneStep::NONE;

public:
    BlockFrontPlaneStatus() {}
    virtual ~BlockFrontPlaneStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Finalize();

    void Reset();
    void CloseEasing(const float& deltaTime);

public: // accsessor
        /// getter
    bool GetIsDeath() const { return isDeath_; }
    PlaneStep GetPlaneStep() const { return planestep_; }
    Vec3f GetScale() const { return scale_; }
    float GetRotate() const { return rotate_; }

    void SetRotate(const float& speed) { rotate_ = speed; }
    void SetRotateSpeed(const float& speed) { rotateSpeed_ = speed; }
    void SetStartScale(const Vec3f& speed) { startScale_ = speed; }
    void SetScale(const Vec3f& speed) { scale_ = speed; }
    void SetCloseEaseTime(const float& speed) { closeEasing_.maxTime = speed; }
    void SetIsDeath(const bool& is) { isDeath_ = is; }
    void SetPlaneStep(const PlaneStep& step) { planestep_ = step; }
};
