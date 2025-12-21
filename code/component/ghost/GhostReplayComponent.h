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
    friend void to_json(nlohmann::json& _j, const GhostReplayComponent& _c);
    friend void from_json(const nlohmann::json& _j, GhostReplayComponent& _c);

public:
    GhostReplayComponent()  = default;
    ~GhostReplayComponent() = default;

    void Initialize(OriGine::Scene* _scene, OriGine::EntityHandle _owner) override;
    void Finalize() override;
    void Edit(OriGine::Scene* _scene, OriGine::EntityHandle _owner, const std::string& _parentLabel) override;

    float ApplyInput();

public:
    std::shared_ptr<OriGine::ReplayPlayer> replayPlayer_ = nullptr;
    // 親シーンでのエンティティID
    int32_t ghostEntityId_       = -1;
    int32_t ghostCameraEntityId_ = -1;

    float frameElapsedTime_ = 0.0f;

    // input
    std::shared_ptr<OriGine::KeyboardInput> keyboardInput_ = nullptr;
    std::shared_ptr<OriGine::MouseInput> mouseInput_       = nullptr;
    std::shared_ptr<OriGine::GamepadInput> gamepadInput_   = nullptr;
};
