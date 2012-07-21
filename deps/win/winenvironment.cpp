#include "deps/win/winenvironment.h"
#include "deps/win/winpersistencelayer.h"

#include "log.h"
#include "game.h"

#include <boost/bind.hpp>
#include <SDL/SDL.h>

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

PersistenceLayer& WinEnvironment::getPersitenceLayer()
{
	return *(PersistenceLayer*)m_persistenceLayer;
}

} // namespace Guy
