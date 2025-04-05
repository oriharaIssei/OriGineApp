#pragma once

#include "component/IComponent.h"
#include <Vector3.h>
#include <cstdint>
class BomStatus
    : public IComponent {

private: // variables

    bool isAlive_ = true;

    /// time
    float explotionTime_; // 爆発までの時間
    float currentTime_;
    /// pos
    Vec3f positionOffset_;
    /// size
    float collisionRadius_;

    int32_t bomNumber_;
    bool isExplotion_=false;

public:
    BomStatus() {}
    virtual ~BomStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);

    virtual void Finalize();

public: // accsessor
    /// getter
    float GetExplotionTime() const { return explotionTime_; }
    float GetCollisionRadius() const { return collisionRadius_; }
    float GetCurrentTimer() const { return currentTime_; }
    Vec3f GetPositionOffset() const { return positionOffset_; }
    int32_t GetBomNumber() const { return bomNumber_; }
    bool GetIsExplotion() const { return isExplotion_; }
    /// setter
    void CurrentTimeIncrement(const float& time) {currentTime_ += time;}
    void SetBomNumber(const int32_t& num) { bomNumber_ = num; }
    void SetIsExplotion(const bool& is) { isExplotion_ = is; }
   /* void SetExplotionTime(const float& time) { explotionTime_ = time; }
    void SetCollisionRadius(const float& radius) { collisionRadius_ = radius; }
    void SetPositionOffset(const Vec3f& offset) { positionOffset_ = offset; }*/
};
