#pragma once
#include "system/ISystem.h"

/// <summary>
/// ボタングループで指定されているボタンのシーンプレビューを行うシステム
/// </summary>
class ButtonScenePreviewSystem
    : public ISystem {
public:
    ButtonScenePreviewSystem();
    ~ButtonScenePreviewSystem();

    void Initialize() override;
    void Finalize() override;

protected:
    void UpdateEntity(Entity* _entity) override;
};
