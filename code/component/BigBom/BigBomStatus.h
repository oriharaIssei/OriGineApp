#pragma once

#include "component/IComponent.h"
#include <Vector3.h>
#include <cstdint>



class BigBomStatus
    : public IComponent {
    friend void to_json(nlohmann::json& j, const BigBomStatus& b);
    friend void from_json(const nlohmann::json& j, BigBomStatus& b);

private: // variables

    bool isAlive_ = true;
    bool islaunch_ = false;

    /// time
    float currentTime_;
    bool isExplotion_=false;
    float launthSpeed_;
    Vec3f offset_;
    Vec3f launchDirection_ = {0.0f,0.0f,0.0f};
   

public:
    BigBomStatus() {}
    virtual ~BigBomStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();

    virtual void Finalize();

    void IncrementLaunchDirectin(const float&value);
    void LaunchDirectionNormalize();

public: // accsessor
    /// getter
  
    float GetCurrentTimer() const { return currentTime_; }  
    bool GetIsExplotion() const { return isExplotion_; }
    bool GetIsLaunch() const { return islaunch_; }
    float GetLaunghSpeed() const { return launthSpeed_; }
    Vec3f GetOffset() const { return offset_; }
    Vec3f GetLaunchDirection() const { return launchDirection_; }
  
    /// setter
    void CurrentTimeIncrement(const float& time) {currentTime_ += time;}
    void SetIsExplotion(const bool& is) { isExplotion_ = is; }
    void SetIsLaunch(const bool& is) { islaunch_ = is; }
    void SetLaunchSpeed(const float& is) { launthSpeed_ = is; }
    void SetOffset(const Vec3f& offset) { offset_ = offset; }
    void SetLaunchDirection(const Vec3f& offset) { launchDirection_ = offset; }
    void SetLaunchDirectionX(const float& offset) { launchDirection_[X] = offset; }
    void SetLaunchDirectionY(const float& offset) { launchDirection_[Y] = offset; }
    void SetLaunchDirectionZ(const float& offset) { launchDirection_[Z] = offset; }
    
   /* void SetExplotionTime(const float& time) { explotionTime_ = time; }
    void SetCollisionRadius(const float& radius) { collisionRadius_ = radius; }
    void SetPositionOffset(const Vec3f& offset) { positionOffset_ = offset; }*/
};
