#pragma once

#include "component/IComponent.h"
#include "KetaEasing.h"
#include <array>
#include <cstdint>
#include <Entity.h>
#include <Vector2.h>
#include <Vector3.h>


class BlockBreakEffectSpawner;
class BlockBreakEffectStatus
    : public IComponent {
    friend void to_json(nlohmann::json& _json, const BlockBreakEffectStatus& _component);
    friend void from_json(const nlohmann::json& _json, BlockBreakEffectStatus& _component);

public:
    enum class EffectType {
        ADVANTAGE,
        SKULL,
    };
    enum class Type {
        FRONTSTART,
        BACKLIGHT,
        BACKFRAME,
    };

private: // variables
    bool isAlive_ = true;


public:
    BlockBreakEffectStatus() {}
    virtual ~BlockBreakEffectStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Finalize();

    void GetParameters(const BlockBreakEffectSpawner& parm);
    void CloseEasing(const float& deltaTime);
    void Reset();

public: // accsessor
  
};
