#pragma once

#include "deps/sdl/sdlenvironment.h"
#include "deps/linux/linuxpersistencelayer.h"

#include <boost/function.hpp>

#include "graphics/screen.h"
#include "audio/emyl.h"

#include <SDL/SDL.h>

namespace Guy {

class LinuxEnvironment : public SDLEnvironment
{
public:
	LinuxEnvironment();
	virtual ~LinuxEnvironment();

	virtual bool init(Game *game);
	virtual void destroy();

	virtual PersistenceLayer& getPersitenceLayer();

private:
	LinuxPersistenceLayer* m_persistenceLayer;
};

} // namespace Guy
