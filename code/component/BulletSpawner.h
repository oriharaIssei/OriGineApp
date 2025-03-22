#pragma once

/// parent
#include "component/IComponent.h"

/// math
#include "math/Vector3.h"

/// <summary>
/// Playerが 弾を発射するためのコンポーネント
/// </summary>
class BulletSpawner
    : public IComponent {
public:
    BulletSpawner();
    ~BulletSpawner() = default;

    void Initialize(GameEntity* _hostEntity) override;

    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);

    virtual void Finalize();

private:
    Vec3f bulletOffset_ = Vec3f(0.0f, 0.0f, 0.0f);

    float fireCoolTime_    = 0.0f;
    float fireCoolTimeMax_ = 0.1f;

    float bulletSpeed_    = 10.0f;
    float bulletLifeTime_ = 1.0f;

    float bulletSize_ = 0.5f;

    bool isFire_ = false;

public: // accessor
    Vec3f getBulletOffset() const { return bulletOffset_; }

    float getFireCoolTime() const { return fireCoolTime_; }
    void setFireCoolTime(float _time) { fireCoolTime_ = _time; }
    float getMaxFireCoolTime() const { return fireCoolTimeMax_; }

    float getBulletSpeed() const { return bulletSpeed_; }
    float getBulletLifeTime() const { return bulletLifeTime_; }
    float getBulletSize() const { return bulletSize_; }

    bool getIsFire() const { return isFire_; }
    void setIsFire(bool _isFire) { isFire_ = _isFire; }
};
