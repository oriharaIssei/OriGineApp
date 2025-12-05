#pragma once
#include "component/IComponent.h"

class StageObstacle
    : public IComponent {
    friend void to_json(nlohmann::json& j, const StageObstacle& component);
    friend void from_json(const nlohmann::json& j, StageObstacle& component);

public:
    StageObstacle()           = default;
    ~StageObstacle() override = default;

    void Initialize(Entity* /*_entity*/) override {}
    void Edit(Scene* _scene, Entity* _entity, const std::string& _parentLabel) override;
    void Finalize() override {}

private:
    float penaltyTime_ = 1.5f; // 障害物に当たったときのペナルティ時間 /sec (制限時間からマイナスする)

public:
    float GetPenaltyTime() const { return penaltyTime_; }
};
