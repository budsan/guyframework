#include "deps/linux/linuxenvironment.h"
#include "deps/linux/linuxpersistencelayer.h"

#include "log.h"
#include "game.h"

#include <boost/bind.hpp>
#include <SDL/SDL.h>

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

PersistenceLayer& LinuxEnvironment::getPersitenceLayer()
{
	return *(PersistenceLayer*)m_persistenceLayer;
}

} // namespace Guy
