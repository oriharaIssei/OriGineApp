#pragma once

#include "component/IComponent.h"
#include<array>
#include <cstdint>
#include <Vector3.h>

class FloatingFloorSpawner
    : public IComponent {

private: // variables

    bool isAlive_ = true;
    bool isCreated_;

    float positionHeight_;
    float sideSpace_;
    
    int32_t columNumMax_;
    int32_t rowNumber_; // 列番号

     int32_t HPMax_;
   
    Vec3f fallCollisionSizeMin_;
    Vec3f fallCollisionSizeMax_;
    Vec3f fallCollisionCenterPos_;

  float revivalTime_;

public:
    FloatingFloorSpawner() {}
    virtual ~FloatingFloorSpawner() = default;

    void Initialize(GameEntity* _entity) override;
    bool Edit() override;
    void Save(BinaryWriter& _writer) override;
    void Load(BinaryReader& _reader) override;
    void Finalize() override;

    void GetSideOffSet();

public: // accsessor

    /// getter
    int32_t GetColumNumMax() const { return columNumMax_; }
    int32_t GetRowNumber() const { return rowNumber_; }
    float GetPositionHeight() const { return positionHeight_; }
    float GetPositionSideSpace() const { return sideSpace_; }
    float GetRevivalTime() const { return revivalTime_; }
    bool GetIsCreated() const { return isCreated_; }
    int32_t GetHpMax() const { return HPMax_; }
    Vec3f GetFallCollisionSizeMin() const { return fallCollisionSizeMin_; }
    Vec3f GetFallCollisionSizeMax() const { return fallCollisionSizeMax_; }
    Vec3f GetFallCollisionCenterPos() const { return fallCollisionCenterPos_; }
      /// setter
    void SetIsCreated(const bool&is) {  isCreated_=is; }
    void SetRowNumber(const int32_t& num) { rowNumber_ = num; }
   
};
