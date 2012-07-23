#pragma once

#include "persistencelayer.h"
#include "input/input.h"
#include "graphics/screen.h"
#include "audio/emyl.h"

namespace Guy {

class Game;
class Environment
{
public:
	static Environment& instance();
	static Environment* ptrInstance();

	virtual bool init(Game *game);
	virtual void destroy();

	virtual void run() = 0;

	virtual void pause() = 0;
	virtual void resume() = 0;
	virtual void exit() = 0;

	Game& game();

	enum DeltaTimeType
	{
		VariableDeltaTime,
		StableDeltaTime,
		StableWithFrameSkipDeltaTime
	};

	virtual void setFrameRate(unsigned short frames = 0,
							   DeltaTimeType type = VariableDeltaTime) = 0;

	//Managers
	virtual Screen&           screen() = 0;
	virtual emyl::manager&    audio() = 0;
	virtual Input&            input() = 0;
	virtual PersistenceLayer& persitenceLayer() = 0;

protected:

	Environment();
	virtual ~Environment();

	Game	*m_game;
private:
	static Environment *s_instance;

};

} //namespace Guy
