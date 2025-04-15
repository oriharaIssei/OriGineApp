#pragma once

#include "component/IComponent.h"
#include<array>
#include <cstdint>
#include <Vector3.h>

class FloorAndPillerSpawner
    : public IComponent {

private: // variables

    bool isAlive_ = true;
    bool isCreated_;
    float pillerSpace_;
    float floorSpace_;
    float firstPillerOffset_;

    int32_t columNumMax_;
    int32_t rowNumber_; // 列番号

    int32_t safeZoneCostMax_;
    int32_t normalCostMax_;

     int32_t HPMax_;
    float collisionSize_;

    Vec3f fallCollisionSizeMin_;
    Vec3f fallCollisionSizeMax_;

public:
    FloorAndPillerSpawner() {}
    virtual ~FloorAndPillerSpawner() = default;

    void Initialize(GameEntity* _entity) override;
    bool Edit() override;
    void Save(BinaryWriter& _writer) override;
    void Load(BinaryReader& _reader) override;
    void Finalize() override;

public: // accsessor

    /// getter
    int32_t GetColumNumMax() const { return columNumMax_; }
    float GetFloorSpace() const { return floorSpace_; }
    float GetPillerSpace() const { return pillerSpace_; }
    float GetFirstPillerOffset() const { return firstPillerOffset_; }
    bool GetIsCreated() const { return isCreated_; }
    int32_t GetRowNumber() const { return rowNumber_; }
    int32_t GetSafeZoneCostMax() const { return safeZoneCostMax_; }
    int32_t GetNormalCostMax() const { return normalCostMax_; }
    float GetCollisionSize() const { return collisionSize_; }
    int32_t GetHpMax() const { return HPMax_; }
    Vec3f GetFallCollisionSizeMin() const { return fallCollisionSizeMin_; }
    Vec3f GetFallCollisionSizeMax() const { return fallCollisionSizeMax_; }
      /// setter
    void SetIsCreated(const bool&is) {  isCreated_=is; }
    void SetRowNumber(const int32_t& num) { rowNumber_ = num; }
  
   
};
