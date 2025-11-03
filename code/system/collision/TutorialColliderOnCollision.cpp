#include "TutorialColliderOnCollision.h"

#include "component/animation/SpriteAnimation.h"
#include "component/collision/collider/AABBCollider.h"
#include "component/renderer/Sprite.h"

TutorialColliderOnCollision::TutorialColliderOnCollision() : ISystem(SystemCategory::Collision) {}
TutorialColliderOnCollision::~TutorialColliderOnCollision() {}

void TutorialColliderOnCollision::Initialize() {
    playerEntity_ = nullptr;
}

void TutorialColliderOnCollision::Finalize() {
    playerEntity_ = nullptr;
}

void TutorialColliderOnCollision::Update() {
    playerEntity_ = getUniqueEntity("Player");
    ISystem::Update();
}

void TutorialColliderOnCollision::UpdateEntity(Entity* _entity) {
    if (_entity == nullptr) {
        return;
    }

    int32_t colliderIndex  = 0;
    bool playerIsColliding = false;

    // 衝突したコライダーを探す
    auto aabbColliders = getComponents<AABBCollider>(_entity);
    for (auto& aabbCollider : *aabbColliders) {
        for (const auto& [id, state] : aabbCollider.getCollisionStateMap()) {
            if (id != playerEntity_->getID()) {
                continue;
            }
            playerIsColliding = true;

            // 衝突開始時,終了時にそれぞれのSpriteアニメーションを再生
            auto sprite = getComponent<SpriteRenderer>(_entity);
            if (sprite != nullptr) {
                // 衝突開始時にSpriteのアニメーションを再生
                if (state == CollisionState::Enter) {
                    auto spriteAnimation = getComponent<SpriteAnimation>(_entity);
                    if (spriteAnimation != nullptr) {
                        spriteAnimation->PlayColorAnimation();
                    }
                } else if (state == CollisionState::Exit) { // 衝突終了時にSpriteのアニメーションを再生
                    auto spriteAnimation = getComponent<SpriteAnimation>(_entity, 1);
                    if (spriteAnimation != nullptr) {
                        spriteAnimation->PlayColorAnimation();
                    }
                }
            }
            break;
        }
        if (playerIsColliding) {
            break;
        }
        ++colliderIndex;
    }
}
