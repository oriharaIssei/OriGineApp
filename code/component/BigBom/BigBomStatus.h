#pragma once

#include "component/IComponent.h"
#include <Vector3.h>
#include <cstdint>
class BigBomStatus
    : public IComponent {

private: // variables

    bool isAlive_ = true;

    /// time
    float currentTime_;
    bool isExplotion_=false;
    float launthSpeed_;
    Vec3f offset_;

public:
    BigBomStatus() {}
    virtual ~BigBomStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);

    virtual void Finalize();

public: // accsessor
    /// getter
  
    float GetCurrentTimer() const { return currentTime_; }  
    bool GetIsExplotion() const { return isExplotion_; }
    float GetLaunghSpeed() const { return launthSpeed_; }
    Vec3f GetOffset() const { return offset_; }
    /// setter
    void CurrentTimeIncrement(const float& time) {currentTime_ += time;}
    void SetIsExplotion(const bool& is) { isExplotion_ = is; }
    void SetLaunchSpeed(const bool& is) { launthSpeed_ = is; }
    void SetOffset(const Vec3f& offset) { offset_ = offset; }
   /* void SetExplotionTime(const float& time) { explotionTime_ = time; }
    void SetCollisionRadius(const float& radius) { collisionRadius_ = radius; }
    void SetPositionOffset(const Vec3f& offset) { positionOffset_ = offset; }*/
};
