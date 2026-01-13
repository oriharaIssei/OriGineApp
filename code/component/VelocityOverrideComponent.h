#pragma once

#include "component/IComponent.h"

/// math
#include "math/Vector3.h"

struct VelocityOverrideComponent
    : public OriGine::IComponent {
    friend void to_json(nlohmann::json& j, const VelocityOverrideComponent& c);
    friend void from_json(const nlohmann::json& j, VelocityOverrideComponent& c);

public:
    VelocityOverrideComponent();
    ~VelocityOverrideComponent() override;
    void Initialize(OriGine::Scene* _scene, OriGine::EntityHandle _owner) override;
    void Finalize() override;
    void Edit(OriGine::Scene* _scene, OriGine::EntityHandle _owner, const std::string& _parentLabel) override;

public:
    // 強制移動ベクトル
    OriGine::Vec3f forcedMovementVector_ = OriGine::Vec3f();
    // 硬直時間
    float stunTime_ = 0.f;
};
