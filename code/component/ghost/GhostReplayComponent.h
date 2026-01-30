#pragma once
#include "component/IComponent.h"

/// stl
#include <memory>

/// engine
#include "debugReplayer/ReplayPlayer.h"
#include "scene/Scene.h"
// input
#include "input/GamepadInput.h"
#include "input/KeyboardInput.h"
#include "input/MouseInput.h"

/// <summary>
/// Ghostのリプレイシーン用コンポーネント
/// 入力、物理演算のみ別シーンで行い、 衝突判定、レンダリングは本シーンで行う
/// </summary>
struct GhostReplayComponent
    : public OriGine::IComponent {
    /// <summary>
    /// JSON 変換用
    /// </summary>
    friend void to_json(nlohmann::json& _j, const GhostReplayComponent& _c);
    /// <summary>
    /// JSON 復元用
    /// </summary>
    friend void from_json(const nlohmann::json& _j, GhostReplayComponent& _c);

public:
    GhostReplayComponent()  = default;
    ~GhostReplayComponent() = default;

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
    /// 入力を適用し、現在の時間を進める
    /// </summary>
    /// <returns>現在の再生時間</returns>
    float ApplyInput();

public:
    std::shared_ptr<OriGine::ReplayPlayer> replayPlayer_ = nullptr;
    // 親シーンでのエンティティID
    OriGine::EntityHandle ghostEntityId_       = OriGine::EntityHandle();
    OriGine::EntityHandle ghostCameraEntityId_ = OriGine::EntityHandle();

    float frameElapsedTime_ = 0.0f;

    // input
    std::shared_ptr<OriGine::KeyboardInput> keyboardInput_ = nullptr;
    std::shared_ptr<OriGine::MouseInput> mouseInput_       = nullptr;
    std::shared_ptr<OriGine::GamepadInput> gamepadInput_   = nullptr;
};
