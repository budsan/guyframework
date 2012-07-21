#pragma once

#include "deps/sdl/sdlenvironment.h"
#include "deps/win/winpersistenceLayer.h"

#include <boost/function.hpp>

#include "graphics/screen.h"
#include "audio/emyl.h"

#include <SDL/SDL.h>

namespace Guy {

class WinEnvironment : public SDLEnvironment
{
public:
    WinEnvironment();
    virtual ~WinEnvironment();

	virtual bool init(Game *game);
	virtual void destroy();

	virtual PersistenceLayer& getPersitenceLayer();

private:
    WinPersistenceLayer* m_persistenceLayer;
};

} // namespace Guy
