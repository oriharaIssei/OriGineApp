#pragma once

#include "component/IComponent.h"
#include <array>
#include <cstdint>
#include <Entity.h>
#include <Vector3.h>
#include"KetaEasing.h"

class LevelUIParentStatus
    : public IComponent {

private: // variables
    bool isAlive_ = true;

    int32_t currentLevel_ = 0; // 現在のレベル
    bool isLevelChange_   = false;
    Vec3f basePos_        = {0, 0, 0};
    Vec3f baseScale_      = {1, 1, 1};
    Easing moveEasing_;
    Easing scaleEasing_;
    Easing uvScrollEasing_;

public:
    LevelUIParentStatus() {}
    virtual ~LevelUIParentStatus() = default;

    void Initialize(GameEntity* _entity) override;
    virtual bool Edit();
    virtual void Save(BinaryWriter& _writer);
    virtual void Load(BinaryReader& _reader);

    virtual void Finalize();

public: // accsessor
    /// getter
    int32_t GetCurrentLevel() const { return currentLevel_; } // 現在のコンボ数
    bool GetIsLevelChange() const { return isLevelChange_; }
    Vec3f GetBasePos() const { return basePos_; }
    Vec3f GetBaseScale() const { return basePos_; }
    /// setter
    void LlvelIncrement() { currentLevel_++; } // 現在のコンボ数
    void SetIsLevelChange(const bool& currentComboNum) { isLevelChange_ = currentComboNum; } // 現在のコンボ数
};
