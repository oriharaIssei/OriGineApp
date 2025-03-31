#pragma once

#include "component/IComponent.h"
#include<array>
#include <cstdint>

class FloorModeCreater
    : public IComponent {

private: // variables

    bool isAlive_ = true;
 
    std::array<bool, 10> isAppearSafeZone_;

public:
    FloorModeCreater() {}
    virtual ~FloorModeCreater() = default;

    void Initialize(GameEntity* _entity) override;
    bool Edit() override;
    void Save(BinaryWriter& _writer) override;
    void Load(BinaryReader& _reader) override;
    void Finalize() override;

 

public: // accsessor

    /// getter
  
    bool GetAppearSafeZone(const int32_t& i) { return isAppearSafeZone_[i]; }
  
      /// setter
 
  
};
