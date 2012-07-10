#pragma once

#include "persistencelayer.h"
#include "input/input.h"
#include "graphics/screen.h"
#include "audio/emyl.h"

class Game;
class Environment
{
public:
	static Environment& instance();
	static Environment* ptrInstance();

	virtual bool init(Game *game);
	virtual void run() = 0;

	virtual void pause() = 0;
	virtual void resume() = 0;
	virtual void exit() = 0;

	//frames 0 -> as fast as posible, stable true -> delta time fixed
	virtual void setFramesPerSecond(unsigned short frames = 0,
					bool stable = false,
					bool dropFrames = false) = 0;

	//Managers
	virtual Screen&           getScreenManager() = 0;
	virtual emyl::manager&    getAudioManager() = 0;
	virtual Input&            getInputManager() = 0;
	virtual PersistenceLayer& getPersitenceLayer() = 0;

protected:

	Environment();
	virtual ~Environment();

	Game	*m_game;
private:
	static Environment *s_instance;

};
