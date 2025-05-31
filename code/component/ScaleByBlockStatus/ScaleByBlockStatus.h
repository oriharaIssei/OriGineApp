#pragma once

#include "component/IComponent.h"
#include <Entity.h>
#include <nlohmann/json.hpp>
#include <Vector2.h>
#include <Vector3.h>

class ScaleByBlockStatus
    : public IComponent {
    friend void to_json(nlohmann::json& _json, const ScaleByBlockStatus& _bomSpawner);
    friend void from_json(const nlohmann::json& _json, ScaleByBlockStatus& _bomSpawner);

private: // variables
    bool isAlive_;
    Vec3f scaleOffset_;
    bool isUVScroll_;

    Vec2f uvPos_{0.0f,0.0f};
    Vec2f uvScrollSpeed_;

public:
    ScaleByBlockStatus() {}
    virtual ~ScaleByBlockStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Finalize();

    void UVScrollUPdate(const float&deltaTime);

public: // accsessor
    /// getter
    Vec3f GetScaleByBlockStatus() const { return scaleOffset_; }
    Vec2f GetUVPos() const { return uvPos_; }
    bool GetIsUVScroll() const { return isUVScroll_; }

    /// setter
};
