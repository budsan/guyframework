#pragma once

#include "base/gamestate.h"
#include "logic/scene.h"

class EngineState : public GameState
{
public:
	 EngineState();
	~EngineState();

	void Update(float GameTime);
	void Draw();
	
	void Load();
	void Unload();

private:

	Scene myScene;
};

