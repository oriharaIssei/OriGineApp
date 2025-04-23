#pragma once

#include "component/IComponent.h"
#include <Entity.h>
#include <Vector3.h>

enum class EffectType {
    SCORE,
    TIME,
};

class EffectByBlockStatus
    : public IComponent {

private: // variables
    bool isAlive_ = true;

    float settingValue_=0.0f;                           //取得した値
    EffectType effectType_=EffectType::TIME;            // エフェクトタイプ

public:
    EffectByBlockStatus() {}
    virtual ~EffectByBlockStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);

    virtual void Finalize();


public: // accsessor
    /// getter
   
  
    /// setter
    void SetValue(const float& scoreChange) { settingValue_ = scoreChange; }
    void SetEffectType(const EffectType& type) { effectType_ = type; }
  
};
