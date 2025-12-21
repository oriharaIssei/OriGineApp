#pragma once
#include "component/IComponent.h"

/// debugReplayer
#include "debugReplayer/ReplayRecorder.h"

/// <summary>
/// プレイを記録するコンポーネント
/// </summary>
struct PlayRecordeComponent
    : public OriGine::IComponent {
    friend void to_json(nlohmann::json& _j, const PlayRecordeComponent& _c);
    friend void from_json(const nlohmann::json& _j, PlayRecordeComponent& _c);

public:
    PlayRecordeComponent();
    ~PlayRecordeComponent() override;

    void Initialize(OriGine::Scene* _scene, OriGine::EntityHandle _owner) override;
    void Finalize() override;

    void Edit(OriGine::Scene* _scene, OriGine::EntityHandle _owner, const std::string& _parentLabel) override;

public:
    std::shared_ptr<OriGine::ReplayRecorder> replayRecorder_ = nullptr;
};
