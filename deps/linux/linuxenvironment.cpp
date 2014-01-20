#include "deps/linux/linuxenvironment.h"
#include "deps/linux/linuxpersistencelayer.h"

#include "debug.h"
#include "game.h"

#include <SDL2/SDL.h>

namespace Guy {

LinuxEnvironment::LinuxEnvironment()
{
}

LinuxEnvironment::~LinuxEnvironment()
{
}

bool LinuxEnvironment::init(Game *game)
{
	m_persistenceLayer = new LinuxPersistenceLayer();
	m_persistenceLayer->load("data/game.cfg");

	return SDLEnvironment::init(game);
}

void LinuxEnvironment::destroy()
{
	m_persistenceLayer->persist();
	SDLEnvironment::destroy();
}

PersistenceLayer& LinuxEnvironment::persitenceLayer()
{
	return *(PersistenceLayer*)m_persistenceLayer;
}

} // namespace Guy
