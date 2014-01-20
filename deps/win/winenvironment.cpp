#include "deps/win/winenvironment.h"
#include "deps/win/winpersistencelayer.h"

#include "debug.h"
#include "game.h"

#include <SDL2/SDL.h>

namespace Guy {

WinEnvironment::WinEnvironment()
{
}

WinEnvironment::~WinEnvironment()
{
}

bool WinEnvironment::init(Game *game)
{
    m_persistenceLayer = new WinPersistenceLayer();
	m_persistenceLayer->load("data/game.cfg");

	return SDLEnvironment::init(game);
}

void WinEnvironment::destroy()
{
	m_persistenceLayer->persist();
	SDLEnvironment::destroy();
}

PersistenceLayer& WinEnvironment::persitenceLayer()
{
	return *(PersistenceLayer*)m_persistenceLayer;
}

} // namespace Guy
