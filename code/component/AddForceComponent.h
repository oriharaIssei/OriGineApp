#pragma once

#include "component/IComponent.h"

/// math
#include "math/Vector3.h"

/// <summary>
/// Rigidbodyを持つエンティティが触れると加速する
/// </summary>
struct AddForceComponent
    : public OriGine::IComponent {
    /// <summary>
    /// JSON 変換用
    /// </summary>
    friend void to_json(nlohmann::json& j, const AddForceComponent& c);
    /// <summary>
    /// JSON 復元用
    /// </summary>
    friend void from_json(const nlohmann::json& j, AddForceComponent& c);

public:
    AddForceComponent();
    ~AddForceComponent() override;

    /// <summary>
    /// 初期化処理
    /// </summary>
    void Initialize(OriGine::Scene* _scene, OriGine::EntityHandle _owner) override;
    /// <summary>
    /// 終了処理
    /// </summary>
    void Finalize() override;
    /// <summary>
    /// エディタ用編集UI
    /// </summary>
    void Edit(OriGine::Scene* _scene, OriGine::EntityHandle _owner, const std::string& _parentLabel) override;

public:
    // 追加する力
    OriGine::Vec3f addForce_ = OriGine::Vec3f();
};
