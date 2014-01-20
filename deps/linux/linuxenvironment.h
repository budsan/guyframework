#pragma once

#include "deps/sdl/sdlenvironment.h"
#include "deps/linux/linuxpersistencelayer.h"

#include "graphics/screen.h"
#include "audio/emyl.h"

#include <SDL2/SDL.h>

namespace Guy {

class LinuxEnvironment : public SDLEnvironment
{
public:
	LinuxEnvironment();
	virtual ~LinuxEnvironment();

	virtual bool init(Game *game);
	virtual void destroy();

	virtual PersistenceLayer& persitenceLayer();

private:
	LinuxPersistenceLayer* m_persistenceLayer;
};

} // namespace Guy
