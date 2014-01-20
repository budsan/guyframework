#pragma once

#include "deps/sdl/sdlenvironment.h"
#include "deps/win/winpersistencelayer.h"

#include "graphics/screen.h"
#include "audio/emyl.h"

#include <SDL2/SDL.h>

namespace Guy {

class WinEnvironment : public SDLEnvironment
{
public:
    WinEnvironment();
    virtual ~WinEnvironment();

	virtual bool init(Game *game);
	virtual void destroy();

	virtual PersistenceLayer& persitenceLayer();

private:
    WinPersistenceLayer* m_persistenceLayer;
};

} // namespace Guy
