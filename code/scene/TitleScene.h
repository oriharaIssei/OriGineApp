#pragma once

/// parent
#include "iScene/IScene.h"

/// stl
#include <list>
#include <memory>
#include <string>

class TitleScene
    : public IScene {
public:
    TitleScene();
    ~TitleScene();

protected:
    virtual void registerComponents();
    virtual void registerSystems();
private:
};
