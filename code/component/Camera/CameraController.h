#pragma once

/// parent
#include "component/IComponent.h"

/// engine
// component
#include "component/transform/Transform.h"

/// math
#include "math/Vector2.h"
#include "math/Vector3.h"

/// <summary>
/// カメラの挙動を制御するためのデータ
/// </summary>
struct CameraController
    : public OriGine::IComponent {
    /// <summary>
    /// JSON 変換用
    /// </summary>
    friend void to_json(nlohmann::json& j, const CameraController& _cameraController);
    /// <summary>
    /// JSON 復元用
    /// </summary>
    friend void from_json(const nlohmann::json& j, CameraController& _cameraController);

public:
    CameraController() {}
    ~CameraController() override {}

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

    /// <summary>
    /// PlayerのギアレベルからFovYを計算する
    /// </summary>
    /// <param name="_level"></param>
    /// <returns></returns>
    float CalculateFovYByPlayerGearLevel(int32_t _level) const;

public:
    OriGine::Transform* followTarget = nullptr;

    // 演出用の回転と分けるための基準回転
    OriGine::Quaternion baseRotate = OriGine::Quaternion::Identity();

    OriGine::Vec3f forward  = {0.f, 0.f, 1.f}; // カメラの前方向
    float angleLimitY       = 0.0f;
    bool isAutoLookAtPlayer = true; // 自動でプレイヤーを注視するかどうか

    /// <summary>
    /// 注視点のOffset
    /// </summary>
    OriGine::Vec3f currentTargetOffset   = {0.f, 0.f, 0.f};
    OriGine::Vec3f firstTargetOffset     = {0.f, 0.f, 0.f};
    OriGine::Vec3f targetOffsetOnDash    = {0.f, 0.f, 0.f};
    OriGine::Vec3f targetOffsetOnWallRun = OriGine::Vec3f(0.f, 0.f, 0.f);
    /// <summary>
    /// 注視点との距離のOffset
    /// </summary>
    OriGine::Vec3f currentOffset   = {0.f, 0.f, 0.f};
    OriGine::Vec3f firstOffset     = {0.f, 0.f, 0.f};
    OriGine::Vec3f offsetOnDash    = {0.f, 0.f, 0.f};
    OriGine::Vec3f offsetOnWallRun = OriGine::Vec3f(0.f, 0.f, 0.f);

    float currentRotateZ   = 0.0f;
    float rotateZOnWallRun = 0.0f;

    OriGine::Vec3f interTarget        = {0.f, 0.f, 0.f};
    OriGine::Vec2f destinationAngleXY = {0.0f, 0.0f};
    float rotateSpeedPadStick         = 0.0f;
    float rotateSpeedMouse            = 0.0f;
    float rotateSensitivity           = 0.0f;
    float interTargetInterpolation    = 0.0f;

    float maxRotateX = 0.0f;
    float minRotateX = 0.0f;

    float fovYInterpolate    = 0.1f;
    float baseFovY           = 45.0f * 3.141592654f / 180.0f; // 基準のFovY
    float fovYRateBase       = 1.0f; // FovYの倍率
    float fovYRateCommonRate = 1.0f; // FovYの倍率の共通値
    float fixForForwardSpeed = 0.1f; // 正面方向への 補正速度
};
