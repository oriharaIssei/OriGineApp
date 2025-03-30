#pragma once

#include "component/IComponent.h"


class BottomFloorStates
    : public IComponent {

private: // variables
    bool isAlive_ = true;

    float fieldRadius_;
    float settingAngle_;
  

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

    /// setter
};
