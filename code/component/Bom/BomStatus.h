#pragma once

#include "component/IComponent.h"
#include <Vector3.h>
class BomStatus
    : public IComponent {

private: // variables

    bool isAlive_ = true;

    /// time
    float explotionTime_; // 爆発までの時間
    /// pos
    Vec3f positionOffset_;
    /// size
    float collisionRadius_;

public:
    BomStatus() {}
    virtual ~BomStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);

    virtual void Finalize();

public: // accsessor
    /// getter
    float GetExplotionTime() const { return explotionTime_; }
    float GetCollisionRadius() const { return collisionRadius_; }
    Vec3f GetPositionOffset() const { return positionOffset_; }
    /// setter

};
