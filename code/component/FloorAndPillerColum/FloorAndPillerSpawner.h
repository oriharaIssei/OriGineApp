#pragma once

#include "component/IComponent.h"
#include<array>
#include <cstdint>

class FloorAndPillerSpawner
    : public IComponent {

private: // variables

    bool isAlive_ = true;
    float fallSpeed_;
    float pillerSpace_;
    float floorSpace_;
    int32_t columNum_;
    std::array<bool, 10> isAppearSafeZone_;

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
    int32_t GetColumNum() const { return columNum_; }
    bool GetAppearSafeZone(const int32_t& i) { return isAppearSafeZone_[i]; }
    float GetFloorSpace() const { return floorSpace_; }
    float GetPillerSpace() const { return pillerSpace_; }
      /// setter
 
  
};
