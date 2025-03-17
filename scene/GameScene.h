#pragma once

#include "iScene/IScene.h"

#include <list>
#include <memory>
#include <string>

// object
class GameScene
    : public IScene {
public:
    GameScene();
    ~GameScene();

    void Initialize();
    void Finalize() override;

protected:
private:
};
