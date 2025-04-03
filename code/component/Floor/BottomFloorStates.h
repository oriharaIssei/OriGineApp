#pragma once

#include "component/IComponent.h"
#include"component/transform/Transform.h"
#include <cstdint>

class BottomFloorStates
    : public IComponent {

private: // variables
    bool isAlive_ = true;

    float fieldRadius_;
    float settingAngle_;
    int32_t floorNum_;

   

public:
    BottomFloorStates() {}
    virtual ~BottomFloorStates() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);

    virtual void Finalize();

public: // accsessor
    /// getter
    float GetFieldRadius() const { return fieldRadius_; }
    float GetSettingAngle() const { return settingAngle_; }
    int32_t GetFloorNum() const { return floorNum_; }
   

    /// setter
   
   
};
