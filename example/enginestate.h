#pragma once

#include "defines.h"
#include "gamestate.h"
#include "game.h"

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

};

