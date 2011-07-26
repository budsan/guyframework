#pragma once

#include "defines.h"
#include "game.h"
#include "gamestate.h"

class MenuState : public GameState
{
public:
	 MenuState();
	~MenuState();

	void Update(float GameTime);
	void Draw();

	void Load();
	void Unload();

private:

};

