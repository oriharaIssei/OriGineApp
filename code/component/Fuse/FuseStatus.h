#pragma once

#include "component/IComponent.h"
#include <cstdint>
#include <Entity.h>
#include <Vector3.h>

class FuseStatus
    : public IComponent {
    friend void to_json(nlohmann::json& _json, const FuseStatus& _timerStatus);
    friend void from_json(const nlohmann::json& _json, FuseStatus& _timerStatus);

private: // variables
    bool isAlive_ = true;

    const Vec3f* basePos_ = nullptr;
    const bool* isDeath_ = nullptr; 

public:
    FuseStatus() {}
    virtual ~FuseStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Finalize();

  

public: // accsessor
    /// getter
    const Vec3f* GetBasePos() const { return basePos_; }
    const bool* GetIsDeath() const { return isDeath_; }

    /// setter
    void SetBasePosition(const Vec3f* isFrontPlaneClose) { basePos_ = isFrontPlaneClose; }
    void SetIsDeath(const bool* is) { isDeath_ = is; }
  
};
