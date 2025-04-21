#pragma once

#include "component/IComponent.h"
#include <array>
#include <Vector2.h>

   enum class BlowDirection {
    LEFT,
    RIGHT,
};
   

class ScrapSpawner
    : public IComponent {
public:


private: // variables
    bool isAlive_ = true;

    float lifeTime_;
    float fallStopPosY_;
    Vec2f blowValue_;
   
  
public:
    ScrapSpawner() {}
    virtual ~ScrapSpawner() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);
    virtual void Finalize();

public: // accsessor
    /// getter
    Vec2f GetBlowValue() const { return blowValue_; }
    float GetFallStopPosY() const { return fallStopPosY_; }
    /// setter
  
};
