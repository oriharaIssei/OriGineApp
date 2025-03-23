#pragma once

#include "component/IComponent.h"

class PlayerStates
    : public IComponent {

private: // variables
    bool isAlive_ = true;

    float moveSpeed_;
    int32_t settingBomNum_;

public:
    PlayerStates() {}
    virtual ~PlayerStates() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);

    virtual void Finalize();

public: // accsessor
    /// getter

    /// setter
};
