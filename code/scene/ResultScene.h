#pragma once

/// parent
#include "iScene/IScene.h"

/// stl
#include <list>
#include <memory>
#include <string>

class ResultScene
    : public IScene {
public:
    ResultScene();
    ~ResultScene();

protected:
    virtual void registerComponents();
    virtual void registerSystems();
private:
};
