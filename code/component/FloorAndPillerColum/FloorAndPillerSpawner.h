#pragma once

#include "component/IComponent.h"
#include<array>
#include <cstdint>

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

      /// setter
    void SetIsCreated(const bool&is) {  isCreated_=is; }
    void SetRowNumber(const int32_t& num) { rowNumber_ = num; }
  
};
