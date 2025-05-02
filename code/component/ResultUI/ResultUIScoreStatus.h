#pragma once

#include "component/IComponent.h"
#include <array>
#include <cstdint>
#include <Entity.h>
#include <string>
#include <Vector2.h>
#include <Vector3.h>

class ResultUIScoreStatus
    : public IComponent {
    friend void to_json(nlohmann::json& _json, const ResultUIScoreStatus& _component);
    friend void from_json(const nlohmann::json& _json, ResultUIScoreStatus& _component);

public:
    enum class ResultUIDigit {
        ONE,
        TWO,
        THREE,
        FOUR,
        FIVE,
        COUNT,
    };

private: // variables
    bool isAlive_ = true;
    ResultUIDigit uiDigit_; // コンボの桁
    int32_t valueForDigit_;
    Vec3f offsetPos_;

    Vec2f textureSize_;

public:
    ResultUIScoreStatus() {}
    virtual ~ResultUIScoreStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();

    virtual void Finalize();

    int32_t GetValueForDigit(const int32_t& value);

public: // accsessor
    /// getter
    ResultUIDigit GetLevelUIDigit() const { return uiDigit_; }
    Vec3f GetOffsetPos() const { return offsetPos_; }
    Vec2f GetTextureSize() const { return textureSize_; }

    /// setter
    void SetTextureSize(const Vec2f& size) { textureSize_ = size; }
};
