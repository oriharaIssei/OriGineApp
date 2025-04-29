#pragma once

#include "component/IComponent.h"
#include"component/LevelUPUI/LevelUIParentStatus.h"
#include <cstdint>
#include <Entity.h>
#include<array>
#include<string>
#include <Vector3.h>



class LevelUIStatus
    : public IComponent {
public:
   

private: // variables
    bool isAlive_ = true;
    LevelUIDigit levelUIDigit_; // コンボの桁
    int32_t valueForDigit_;
    Vec3f offsetPos_;
  
public:
    LevelUIStatus() {}
    virtual ~LevelUIStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);

    virtual void Finalize();

    int32_t GetValueForDigit(const int32_t&value);

public: // accsessor
    /// getter
    LevelUIDigit GetLevelUIDigit() const { return levelUIDigit_; }
    Vec3f GetOffsetPos() const { return offsetPos_; }
    
    /// setter
   
};
