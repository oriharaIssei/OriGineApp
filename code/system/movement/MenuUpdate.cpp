#include "MenuUpdate.h"

/// component
#include "component/SubScene.h"
/// system
#include "system/SystemRunner.h"

#include "system/movement/FollowCameraUpdateSystem.h"
#include "system/movement/MoveSystemByRigidBody.h"
#include "system/movement/PlayerMoveSystem.h"

using namespace OriGine;

MenuUpdate::MenuUpdate() : ISystem(OriGine::SystemCategory::Movement) {}
MenuUpdate::~MenuUpdate() {}

void MenuUpdate::Initialize() {}
void MenuUpdate::Finalize() {}

void MenuUpdate::UpdateEntity(OriGine::Entity* _entity) {
    auto currentScene = GetScene();
    auto menuScene    = GetComponent<SubScene>(_entity);
    if (!menuScene || !currentScene) {
        return;
    }

    auto systemRunner = currentScene->GetSystemRunnerRef();

    if (menuScene->IsActive()) {
        // メニューがアクティブな間は ゲームのUpdateを止める
        systemRunner->SetCategoryActivity(SystemCategory::Input, false);
        systemRunner->SetCategoryActivity(SystemCategory::StateTransition, false);
        systemRunner->SetCategoryActivity(SystemCategory::Collision, false);
        systemRunner->SetCategoryActivity(SystemCategory::Effect, false);

        systemRunner->DeactivateSystem<PlayerMoveSystem>();
        systemRunner->DeactivateSystem<MoveSystemByRigidBody>();
        systemRunner->DeactivateSystem<FollowCameraUpdateSystem>();
    } else {
        // メニューが非アクティブなら ゲームのUpdateを再開
        systemRunner->SetCategoryActivity(SystemCategory::Input, true);
        systemRunner->SetCategoryActivity(SystemCategory::StateTransition, true);
        systemRunner->SetCategoryActivity(SystemCategory::Collision, true);
        systemRunner->SetCategoryActivity(SystemCategory::Effect, true);

        systemRunner->ActivateSystem<PlayerMoveSystem>();
        systemRunner->ActivateSystem<MoveSystemByRigidBody>();
        systemRunner->ActivateSystem<FollowCameraUpdateSystem>();
    }
}
