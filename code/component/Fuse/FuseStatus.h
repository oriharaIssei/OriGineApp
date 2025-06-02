#pragma once

#include "component/IComponent.h"
#include"component/Block/BlockStatus.h"
#include <cstdint>
#include <Entity.h>
#include <Vector3.h>
#include "KetaEasing.h"

class FuseStatus
    : public IComponent {
    friend void to_json(nlohmann::json& _json, const FuseStatus& _timerStatus);
    friend void from_json(const nlohmann::json& _json, FuseStatus& _timerStatus);

private: // variables
    bool isAlive_ = true;

    const Vec3f* basePos_ = nullptr;
    const bool* isDeath_ = nullptr;
    Vec3f offsetPos_;
    Vec3f maxScale_;
    Vec3f StartScale_;
    const FuseMode* fuseMode_ = nullptr;

    Easing easing_;
    Vec3f scale_;


    bool isStarted_ = false;

public:
    FuseStatus() {}
    virtual ~FuseStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Finalize();

   void ScaleEasing(const float& deltaTime);
    void Reset();

public: // accsessor
    /// getter
    const Vec3f* GetBasePos() const { return basePos_; }
    const bool* GetIsDeath() const { return isDeath_; }
    const FuseMode* GetFuseMode() const { return fuseMode_; }
    Vec3f GetOffsetPos() const { return offsetPos_; }
    Vec3f GetScale() const { return scale_; }
    bool GetIsStarted() const { return isStarted_; }

    /// setter
    void SetBasePosition(const Vec3f* isFrontPlaneClose) { basePos_ = isFrontPlaneClose; }
    void SetIsDeath(const bool* is) { isDeath_ = is; }
    void SetFuseMode(const FuseMode* is) { fuseMode_ = is; }
    void SetOffSetPos(const Vec3f offset) { offsetPos_ = offset; }
    void SetMaxScale(const Vec3f offset) { maxScale_ = offset; }
    void SetStartScale(const Vec3f offset) { StartScale_ = offset; }
    void SetisStarted(const bool& is) { isStarted_=is; }
  
};
