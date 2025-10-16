#include "MenuUpdate.h"

/// component
#include "component/SubScene.h"
/// system
#include "system/movement/MoveSystemByRigidBody.h"
#include "system/movement/PlayerMoveSystem.h"
#include "system/movement/UpdateCameraForward.h"

MenuUpdate::MenuUpdate() : ISystem(SystemCategory::Movement) {}
MenuUpdate::~MenuUpdate() {}

void MenuUpdate::Initialize() {}
void MenuUpdate::Finalize() {}

void MenuUpdate::UpdateEntity(GameEntity* _entity) {
    auto currentScene = getScene();
    auto menuScene    = getComponent<SubScene>(_entity);
    if (!menuScene || !currentScene) {
        return;
    }

    auto systemRunner = currentScene->getSystemRunnerRef();

    if (menuScene->isActive()) {
        // メニューがアクティブな間は ゲームのUpdateを止める
        systemRunner->setCategoryActivity(SystemCategory::Input, false);
        systemRunner->setCategoryActivity(SystemCategory::StateTransition, false);
        systemRunner->setCategoryActivity(SystemCategory::Collision, false);
        systemRunner->setCategoryActivity(SystemCategory::Effect, false);

        systemRunner->DeactivateSystem<PlayerMoveSystem>();
        systemRunner->DeactivateSystem<MoveSystemByRigidBody>();
        systemRunner->DeactivateSystem<UpdateCameraForward>();
    } else {
        // メニューが非アクティブなら ゲームのUpdateを再開
        systemRunner->setCategoryActivity(SystemCategory::Input, true);
        systemRunner->setCategoryActivity(SystemCategory::StateTransition, true);
        systemRunner->setCategoryActivity(SystemCategory::Collision, true);
        systemRunner->setCategoryActivity(SystemCategory::Effect, true);

        systemRunner->ActivateSystem<PlayerMoveSystem>();
        systemRunner->ActivateSystem<MoveSystemByRigidBody>();
        systemRunner->ActivateSystem<UpdateCameraForward>();
    }
}
