#pragma once

#include "component/IComponent.h"

/// math
#include "math/Vector3.h"

/// <summary>
/// シェイクの種類
/// </summary>
enum class ShakeSourceType {
    SinCurve, // サインカーブ
    Noise // ノイズ
};

/// <summary>
/// カメラシェイクの発生源コンポーネント
/// </summary>
struct CameraShakeSourceComponent
    : public IComponent {
    friend void to_json(nlohmann::json& _j, const CameraShakeSourceComponent& _c);
    friend void from_json(const nlohmann::json& _j, CameraShakeSourceComponent& _c);

public:
    CameraShakeSourceComponent();
    ~CameraShakeSourceComponent() override;

    void Initialize(Entity* _entity) override;
    void Edit(Scene* _scene, Entity* _entity, const std::string& _parentLabel) override;
    void Finalize() override;

    void StartShake();
    void StopShake();

public:
    struct ShakeParameters {
        float amplitude; // シェイクの強さ
        float frequency; // シェイクの速さ
    };

public:
    bool isActive                = true; // シェイクの有効無効
    ShakeSourceType type         = ShakeSourceType::SinCurve; // シェイクの種類
    int32_t cameraTransformIndex = -1; // シェイクを適用するカメラのTransformコンポーネントインデックス

    Vec3<ShakeParameters> axisParameters = {
        {1.0f, 1.0f}, // X軸のパラメータ
        {1.0f, 1.0f}, // Y軸のパラメータ
        {1.0f, 1.0f} // Z軸のパラメータ
    };

    bool isLoop       = true; // シェイクのループ有無
    float duration    = 1.0f; // シェイクの継続時間
    float elapsedTime = 0.0f; // 経過時間

    Vec2f fragCoord  = Vec2f(0.0f, 0.0f); // フラグメント座標
    Vec2f resolution = Vec2f(1920.0f, 1080.0f); // 解像度
};
