#pragma once

#include "component/IComponent.h"
#include"component/LevelUPUI/LevelUIParentStatus.h"
#include <cstdint>
#include <Entity.h>
#include<array>
#include<string>
#include <Vector3.h>
#include <Vector2.h>



class LevelUIStatus
    : public IComponent {
public:
    friend void to_json(nlohmann::json& j, const LevelUIStatus& l);
    friend void from_json(const nlohmann::json& j, LevelUIStatus& l);

private: // variables
    bool isAlive_ = true;
    LevelUIDigit levelUIDigit_; // コンボの桁
    int32_t valueForDigit_;
    Vec3f offsetPos_;

    Vec2f textureSize_;
  
public:
    LevelUIStatus() {}
    virtual ~LevelUIStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();

    virtual void Finalize();

    int32_t GetValueForDigit(const int32_t&value);

public: // accsessor
    /// getter
    LevelUIDigit GetLevelUIDigit() const { return levelUIDigit_; }
    Vec3f GetOffsetPos() const { return offsetPos_; }
    Vec2f GetTextureSize() const { return textureSize_; }
    
    /// setter
    void SetTextureSize(const Vec2f& size) { textureSize_ = size; }
};
