#pragma once
#include "component/IComponent.h"

/// debugReplayer
#include "debugReplayer/ReplayRecorder.h"

/// <summary>
/// プレイを記録するコンポーネント
/// </summary>
struct PlayRecordeComponent
    : public IComponent {
    friend void to_json(nlohmann::json& _j, const PlayRecordeComponent& _c);
    friend void from_json(const nlohmann::json& _j, PlayRecordeComponent& _c);

public:
    PlayRecordeComponent();
    ~PlayRecordeComponent() override;

    void Initialize(Entity* _entity) override;
    void Finalize() override;

    void Edit(Scene* _scene, Entity* _entity, const std::string& _parentLabel) override;

public:
    std::shared_ptr<ReplayRecorder> replayRecorder_ = nullptr;
};
