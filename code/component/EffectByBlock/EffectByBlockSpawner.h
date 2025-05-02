#pragma once

#include "component/IComponent.h"


enum class EffectType;
class GameEntity;
class EffectByBlockSpawner
    : public IComponent {
public:
private: // variables
    bool isAlive_ = true;

public:
    EffectByBlockSpawner() {}
    virtual ~EffectByBlockSpawner() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);

    virtual void Finalize();

    void EffectUISpawn(GameEntity* _entity, const float& value, const EffectType& type);

 

public: // accsessor
    /// getter
 
};
