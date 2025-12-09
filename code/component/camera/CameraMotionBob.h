#pragma once
#include "component/IComponent.h"

/// math
#include "math/Vector3.h"
#include "MyEasing.h"

/// <summary>
/// Playerの速度によるカメラの揺れを表すコンポーネント
/// </summary>
struct CameraMotionBob
    : public IComponent {
    friend void to_json(nlohmann::json& _j, const CameraMotionBob& _c);
    friend void from_json(const nlohmann::json& _j, CameraMotionBob& _c);

public:
    CameraMotionBob();
    ~CameraMotionBob() override;

    void Initialize(Entity* _entity) override;
    void Edit(Scene* _scene, Entity* _entity, const std::string& _parentLabel) override;
    void Finalize() override;

public:
    int32_t cameraShakeSourceComponentId = -1; ///< カメラシェイクソースコンポーネントID
    int32_t thresholdGearLevel           = 3; ///< 揺れを開始するギアレベル

    Vec3f minAmplitude = Vec3f(0.02f, 0.02f, 0.0f); ///< 最小振幅
    Vec3f maxAmplitude = Vec3f(0.1f, 0.1f, 0.0f); ///< 最大振幅
    Vec3f minFrequency = Vec3f(10.0f, 20.0f, 0.0f); ///< 最小周波数
    Vec3f maxFrequency = Vec3f(30.0f, 50.0f, 0.0f); ///< 最大周波数
};
