#pragma once

#include "component/IComponent.h"
#include "KetaEasing.h"
#include <array>
#include <Entity.h>
#include <Vector3.h>

class BlockBreakEffectSpawner
    : public IComponent {
    friend void to_json(nlohmann::json& _json, const BlockBreakEffectSpawner& _component);
    friend void from_json(const nlohmann::json& _json, BlockBreakEffectSpawner& _component);

public:
private: // variables
    bool isAlive_ = true;

    Easing frontStartEasing_;
    Easing backLightEasing_;
    Easing backLightFrameEasing_;

    float startRotateSpeed_;
    std::array<float, 3> alphaSpeed_;
    std::array<Vec3f, 3> maxScale_;
    std::array<Vec3f, 3> positionOffst_;

public:
    BlockBreakEffectSpawner() {}
    virtual ~BlockBreakEffectSpawner() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Finalize();

    void Reset();

    void BreakEffectSpawn(const Vec3f& position);

public: // accsessor
        /// getter
};
