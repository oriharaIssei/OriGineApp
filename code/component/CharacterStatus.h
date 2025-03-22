#pragma once

#include "component/IComponent.h"

class CharacterStatus
    : public IComponent {
public:
    CharacterStatus() {}
    virtual ~CharacterStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);

    virtual void Finalize();

private:
    bool isAlive_ = true;

    int32_t life_   = 1;
    int32_t attack_ = 1;

public: // accsessor
    int32_t getHP() const { return life_; }
    void takeDamage(int32_t _damage) { life_ -= _damage; }
    void setHP(int32_t _hp) { life_ = _hp; }

    int32_t getAttack() const { return attack_; }
    void setAttack(int32_t _attack) { attack_ = _attack; }
};
