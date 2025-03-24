#pragma once

/// parent
#include "FrameWork.h"

/// Engine
class Engine;
class GlobalVariables;
class SceneManager;

class MyGame
 :public FrameWork{
public:
	MyGame();
	~MyGame();

	void Initialize()override;
	void Finalize()override;

	void Run();

private:

	Engine* engine_ = nullptr;
	GlobalVariables* variables_ = nullptr;
	SceneManager* sceneManager_ = nullptr;
};
