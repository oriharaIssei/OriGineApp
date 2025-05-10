#include "MenuSystem.h"

/// Engine
#define ENGINE_INCLUDE
/// ECS
// component
#define ENGINE_COMPONENTS
// lib

// include
#include <Quaternion.h>
#include <Vector3.h>
// component
#include "component/Menu/MenuStatus.h"
#include"component/Menu/TutorialMenuParentStatus.h"
#include"component/GameEnd/SceneChangerStatus.h"

#include "engine/EngineInclude.h"
#include <Vector2.h>

MenuSystem::MenuSystem()
    : ISystem(SystemType::Movement) {}

MenuSystem::~MenuSystem() {}

void MenuSystem::Initialize() {
    time_  = 0.0f;
    input_ = Input::getInstance();
}

void MenuSystem::Finalize() {}

void MenuSystem::UpdateEntity(GameEntity* entity) {

     // ComboEntityを取得
    EntityComponentSystemManager* ecsManager = ECSManager::getInstance();
    GameEntity* tutorialParentEntity         = ecsManager->getUniqueEntity("TutorialMenuParent");
    GameEntity* endEntity                    = ecsManager->getUniqueEntity("GameEnd");

    
    if (!tutorialParentEntity || !endEntity) { // Entityが存在しない場合の早期リターン
        return;
    }

    MenuStatus* menu       = getComponent<MenuStatus>(entity);
    SpriteRenderer* sprite = getComponent<SpriteRenderer>(entity);
    TutorialMenuParentStatus* tutorialUIParent = getComponent<TutorialMenuParentStatus>(tutorialParentEntity);
    SceneChangerStatus* gameend                = getComponent<SceneChangerStatus>(endEntity);

    //Audio
    Audio* audioOpenMenu = getComponent<Audio>(entity);
    Audio* selectCategory = getComponent<Audio>(entity,1);
    Audio* closeMenu = getComponent<Audio>(entity,2);

    if (!menu || !sprite || !tutorialUIParent) {
        return;
    }

    float deltaTime = Engine::getInstance()->getDeltaTime();

    switch (menu->GetMenuMode()) {
    case MenuMode::NONE:
        if (input_->isTriggerKey(DIK_ESCAPE)) {
            audioOpenMenu->Play();
            menu->Reset();
            menu->SetMenuMode(MenuMode::MENUSELECT);
        }
        break;

    case MenuMode::MENUSELECT:

        menu->OpenMenuAnimation(deltaTime);
        menu->ArrowMoveAnimation(deltaTime);

        // 下/右入力で次のカテゴリーへ
        if (input_->isTriggerKey(DIK_DOWN) || input_->isTriggerKey(DIK_S)) {
            selectCategory->Play();
            menu->SelectNextCategory();
            /* moved = true;*/
        }

        // 上/左入力で前のカテゴリーへ
        else if (input_->isTriggerKey(DIK_UP) || input_->isTriggerKey(DIK_W)) {
            selectCategory->Play();
            menu->SelectPreviousCategory();
            /* moved = true;*/
        }

        else if (input_->isTriggerKey(DIK_ESCAPE)) {
            menu->SetMenuMode(MenuMode::CLOSEINIT);
            closeMenu->Play();
        }

        menu->UpdateArrowPos();
        menu->DesideForCategory(input_,tutorialUIParent);

        break;

    case MenuMode::WATCHINGTUTORIAL:
      /*  tutorialUIParent->SetIsAnimation(true);*/
        break;

    case MenuMode::CLOSEINIT:
        menu->ScrollTimeReset();
        menu->SetMenuMode(MenuMode::CLOSE);
        break;

    case MenuMode::CLOSE:
        menu->CloseAnimation(deltaTime);
        break;

    case MenuMode::END:

        menu->SetMenuMode(MenuMode::NONE);
        break;

    case MenuMode::GAMEEND:
        // ポーズ中は通さない

        
        if (!gameend) {
            return;
        }

        gameend->SetIsBackTitle(true);

        break;

    default:
        break;
    }

    ///* ------------------------------calucration------------------------------

    Vec2f basePos  = menu->GetBasePos();
    Vec2f baseSize = sprite->getTextureSize() * menu->GetBaseScale();
    /*  resultRank->SetRankForScore(resultUIParent->GetCurrentScore());*/

    ///* ------------------------------adapt------------------------------

    // pos
    sprite->setTranslate(basePos);
    sprite->setSize(baseSize);
    /* sprite->setUVScale(Vec2f(tutorialMenu->GetUVScale(),1.0f));*/
}

void MenuSystem::ComboReset() {
}
