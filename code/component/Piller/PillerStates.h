#pragma once

#include "component/IComponent.h"
#include <cstdint>

class PillerStates
    : public IComponent {

private: // variables

    bool isAlive_ = true;
    bool isBroken_ = false;

     int32_t pillarHP_;
    float collisionSize_;

public:
    PillerStates() {}
    virtual ~PillerStates() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);

    virtual void Finalize();

public: // accsessor
    /// getter
    float GetCollisionSize() const { return collisionSize_; }
     
    /// setter
};
