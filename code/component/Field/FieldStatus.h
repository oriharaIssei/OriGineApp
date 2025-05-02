#pragma once

#include "component/IComponent.h"
#include <cstdint>
#include <Entity.h>
#include <component/transform/Transform.h>
#include <Vector3.h>

class FieldStatus
    : public IComponent {

private: // variables
    bool isAlive_ = true;
    float fieldLeftMax_;
    float fieldRightMax_;

public:
    FieldStatus() {}
    virtual ~FieldStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);

    virtual void Finalize();

    void IncrementCurrentBigBomPoint();
    void ReSetCurrentBigBomPoint();

public: // accsessor

    /// getter
    float GetFieldLeftMax() const { return fieldLeftMax_; }
    float GetFieldRightMax() const { return fieldRightMax_; }
  
    /// setter
   
};
