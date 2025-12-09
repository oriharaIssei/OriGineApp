#pragma once
#include "system/ISystem.h"

/// <summary>
/// LookAtFromTransforms で指定された2つのTransformコンポーネントを参照し、1つ目のTransformの位置から2つ目のTransformの位置を向くように設定するシステム
/// </summary>
class LookAtFromTransformsSystem
    : public ISystem {
public:
    LookAtFromTransformsSystem();
    ~LookAtFromTransformsSystem() override;
    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(Entity* _entity) override;
};
