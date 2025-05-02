#pragma once

#include "component/IComponent.h"
#include <array>
#include <cstdint>
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
    bool isSpawn_ = false;

    float lifeTime_;
    float fallStopPosY_;
    Vec2f blowValue_;
   
    //math
    float mass_;
    float colliderRadius_;

    //生成数
    int32_t createNum_;

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
    bool GetIsSpawn() const { return isSpawn_; }
    int32_t GetCreateNum() const { return createNum_; }
    float GetMass() const { return mass_; }
    float GetColliderRadius() const { return colliderRadius_; }
    float GetLifeTime() const { return lifeTime_; }
    /// setter
    void SetIsSpawn(const bool& is) { isSpawn_ = is; }
};
