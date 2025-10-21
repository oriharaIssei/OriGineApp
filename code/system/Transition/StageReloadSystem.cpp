#include "StageReloadSystem.h"

#ifndef _RELEASE

/// ECS
#include "component/Stage/Stage.h"
#include "system/Initialize/CreateStage.h"

void StageReloadSystem::Initialize() {
}

void StageReloadSystem::Finalize() {
}
void StageReloadSystem::UpdateEntity(Entity* _entity) {
    // StageReloadComponentを持っているエンティティを探す
    auto stageReloadComponents = getComponents<Stage>(_entity);
    if (stageReloadComponents == nullptr) {
        return;
    }
    for (auto& stageComponent : *stageReloadComponents) {
        auto stageFileWatcher = stageComponent.getFileWatcher();
        if (!stageFileWatcher) {
            continue;
        }

        if (stageFileWatcher->isChanged()) {
            stageComponent.ReloadFile();
            auto createStageSystem = dynamic_cast<CreateStage*>(getScene()->getSystem(nameof<CreateStage>()));
            if (createStageSystem) {
                createStageSystem->Update();
            }
        }
    }
}

#endif // _RELEASE
