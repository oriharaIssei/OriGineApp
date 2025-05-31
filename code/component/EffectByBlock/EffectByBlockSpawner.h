#pragma once

#include "component/IComponent.h"
#include <Vector3.h>
#include"KetaEasing.h"

enum class EffectType;
class GameEntity;
class EffectByBlockSpawner
    : public IComponent {
    friend void to_json(nlohmann::json& _json, const EffectByBlockSpawner& _e);
    friend void from_json(const nlohmann::json& _json, EffectByBlockSpawner& _e);

public:
private: // variables
    bool isAlive_ = true;

    float rotateSpeed_;
    float fadeOutTime_;
    float fadeOutValueX_;
    Easing ApearEasing_;
    Easing FadeOutEasing_;
    Vec3f maxScale_;

public:
    EffectByBlockSpawner() {}
    virtual ~EffectByBlockSpawner() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();

    virtual void Finalize();

    void EffectUISpawn(const Vec3f& position, const float& value, const EffectType& type);

 

public: // accsessor
    /// getter
 
};
