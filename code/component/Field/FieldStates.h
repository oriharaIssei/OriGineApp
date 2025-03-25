#pragma once

#include "component/IComponent.h"

class FieldStates
    : public IComponent {

private: // variables
    bool isAlive_ = true;

    float fieldRadius_;
    

public:
    FieldStates() {}
    virtual ~FieldStates() = default;

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
