#pragma once
#include "component/IComponent.h"

/// debugReplayer
#include "debugReplayer/ReplayRecorder.h"

/// <summary>
/// プレイを記録するコンポーネント
/// </summary>
struct PlayRecorderComponent
    : public OriGine::IComponent {
    friend void to_json(nlohmann::json& _j, const PlayRecorderComponent& _c);
    friend void from_json(const nlohmann::json& _j, PlayRecorderComponent& _c);

public:
    PlayRecorderComponent();
    ~PlayRecorderComponent() override;

    void Initialize(OriGine::Scene* _scene, OriGine::EntityHandle _owner) override;
    void Finalize() override;

    void Edit(OriGine::Scene* _scene, OriGine::EntityHandle _owner, const std::string& _parentLabel) override;

public:
    std::shared_ptr<OriGine::ReplayRecorder> replayRecorder_ = nullptr;
};
