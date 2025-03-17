#include "GameScene.h"

/// ECS
#include "ECS/ECSManager.h"
// component
#include "component/renderer/MeshRender.h"
#include "component/transform/Transform.h"
// system
#include "system/render/TexturedMeshRenderSystem.h"

GameScene::GameScene()
    : IScene("GameScene") {}

GameScene::~GameScene() {}

void GameScene::Initialize() {
    IScene::Initialize();
}

void GameScene::Finalize() {
    IScene::Finalize();
}
