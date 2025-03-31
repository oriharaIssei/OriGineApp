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
    int32_t columNum_;
   

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
    float GetFloorSpace() const { return floorSpace_; }
    float GetPillerSpace() const { return pillerSpace_; }
    bool GetIsCreated() const { return isCreated_; }
      /// setter
    void SetIsCreated(const bool&is) {  isCreated_=is; }
 
  
};
