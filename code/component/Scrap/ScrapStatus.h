#pragma once

#include "component/IComponent.h"
#include "component/Scrap/ScrapSpawner.h"
#include <array>

class ScrapStatus
    : public IComponent {
public:
private: // variables
    bool isAlive_ = true;
    float lifeTime_;
    float fallStopPosY_;
    BlowDirection blowDirection_;

public:
    ScrapStatus() {}
    virtual ~ScrapStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);

    virtual void Finalize();

public: // accessor
        /// getter
    bool IsAlive() const { return isAlive_; }
    float GetLifeTime() const { return lifeTime_; }
    float GetFallStopPosY() const { return fallStopPosY_; }
    BlowDirection GetBlowDirection() const { return blowDirection_; }

    /// setter
    void SetIsAlive(bool isAlive) { isAlive_ = isAlive; }
    void SetLifeTime(const float& lifeTime) { lifeTime_ = lifeTime; }
    void SetFallStopPosY(const float& y) { fallStopPosY_ = y; }
    void SetBlowDirection(const BlowDirection& dir) { blowDirection_ = dir; }
};
