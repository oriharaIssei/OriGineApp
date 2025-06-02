#pragma once

#include "component/IComponent.h"
#include "KetaEasing.h"
#include <array>
#include <cstdint>
#include <Entity.h>
#include <string>
#include <Vector2.h>
#include <Vector3.h>

enum class RankAnimationStep {
    NONE,
    SCALING,
    END,
};

class ResultUIRankStatus
    : public IComponent {
    friend void to_json(nlohmann::json& _json, const ResultUIRankStatus& _component);
    friend void from_json(const nlohmann::json& _json, ResultUIRankStatus& _component);

public:
    enum class RankType {
        C,
        B,
        A,
        S,
        COUNT,
    };

private: // variables
    bool isAlive_ = true;
    RankType rankType_; // コンボの桁

    // pos
    Vec3f position_;

    // rotate
    float rotateValue_;
    float rotate_ = 0.0f;

    // scale
    Vec2f scale_;
    Vec2f easeScale_;

    // ease
    Easing scalingEasing_;
    Easing rotateEasing_;

    // step
    RankAnimationStep step_ = RankAnimationStep::NONE;

    std::array<float, static_cast<int>(RankType::COUNT)> rankValue_;

    // uv
    float uvPos_;

    // size
    Vec2f textureSize_;

public:
    ResultUIRankStatus() {}
    virtual ~ResultUIRankStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();

    virtual void Finalize();

    void SetRankForScore(const float& score);
    float GetUVPos();

    void ScalingEasing(const float& time);
    void RotateEasing(const float& time);

    void Reset();

public: // accsessor
    /// getter
    RankAnimationStep GetAnimationStep() const { return step_; }
    RankType GetLevelUIDigit() const { return rankType_; }
    Vec3f GetPosition() const { return position_; }
    Vec2f GetScale() const { return scale_; }
    Vec2f GetTextureSize() const { return textureSize_; }
    float GetRotate() const { return rotate_; }

    /// setter
    void SetTextureSize(const Vec2f& size) { textureSize_ = size; }
    void SetAnimationStep(const RankAnimationStep& step) { step_ = step; }
};
