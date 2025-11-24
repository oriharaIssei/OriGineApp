#pragma once
#include "component/IComponent.h"

/// math
#include "math/Vector3.h"

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
    int32_t controlPointIndex_ = -1; // 対応する制御点のインデックス
    float penaltyTime_         = 1.f; // 障害物に当たったときのペナルティ時間 /sec (制限時間からマイナスする)

    Vec3f size_ = {1.f, 1.f, 1.f}; // 障害物のサイズ
public:
    const Vec3f& GetSize() const { return size_; }
    void SetSize(const Vec3f& _size) { size_ = _size; }

    float GetPenaltyTime() const { return penaltyTime_; }
};
