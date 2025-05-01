#pragma once

#include "component/IComponent.h"
#include <cstdint>
#include <Entity.h>
#include<array>
#include<string>
#include <Vector3.h>
#include <Vector2.h>

enum class RankType {
    B,
    A,
    S,
    COUNT,
};

class ResultUIRankStatus
    : public IComponent {
public:
   

private: // variables
    bool isAlive_ = true;
    RankType rankType_; // コンボの桁
    float uvPos_;
    Vec3f offsetPos_;

    std::array<float, static_cast<int>(RankType::COUNT)> rankValue_;

    Vec2f textureSize_;
  
public:
    ResultUIRankStatus() {}
    virtual ~ResultUIRankStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);

    virtual void Finalize();

    void SetRankForScore(const float& score);
    float GetUVPos();

public: // accsessor
    /// getter
    RankType GetLevelUIDigit() const { return rankType_; }
    Vec3f GetOffsetPos() const { return offsetPos_; }
    Vec2f GetTextureSize() const { return textureSize_; }
    
    /// setter
    void SetTextureSize(const Vec2f& size) { textureSize_ = size; }
};
