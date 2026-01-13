#pragma once

#include "component/IComponent.h"

/// math
#include "math/Vector3.h"

/// <summary>
/// Rigidbodyを持つエンティティが触れると加速する
/// </summary>
struct AddForceComponent
    : public OriGine::IComponent {
    friend void to_json(nlohmann::json& j, const AddForceComponent& c);
    friend void from_json(const nlohmann::json& j, AddForceComponent& c);

public:
    AddForceComponent();
    ~AddForceComponent() override;

    void Initialize(OriGine::Scene* _scene, OriGine::EntityHandle _owner) override;
    void Finalize() override;
    void Edit(OriGine::Scene* _scene, OriGine::EntityHandle _owner, const std::string& _parentLabel) override;

public:
    // 追加する力
    OriGine::Vec3f addForce_ = OriGine::Vec3f();
};
