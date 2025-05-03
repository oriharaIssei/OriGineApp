#pragma once

#include "component/IComponent.h"
#include <cstdint>
#include <Entity.h>
#include<array>
#include<string>
#include <Vector3.h>
#include <Vector2.h>

class TutorialMenuStatus
    : public IComponent {
    friend void to_json(nlohmann::json& j, const TutorialMenuStatus& m);
    friend void from_json(const nlohmann::json& j, TutorialMenuStatus& m);

public:
   
    enum class TutorialPauge {
        ONE,
        TWO,
        THREE,
        COUNT,
    };

private: // variables
    bool isAlive_ = true;
    //TutorialPauge uiDigit_; // コンボの桁
    int32_t valueForDigit_;
    Vec2f offsetPos_;

    Vec2f textureSize_;
  
public:
    TutorialMenuStatus() {}
    virtual ~TutorialMenuStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
   
    virtual void Finalize();

public: // accsessor
    /// getter
 /*   TutorialPauge GetPaugeNum() const { return uiDigit_; }*/
    Vec2f GetOffsetPos() const { return offsetPos_; }
    Vec2f GetTextureSize() const { return textureSize_; }
    
    /// setter
    void SetTextureSize(const Vec2f& size) { textureSize_ = size; }
};
