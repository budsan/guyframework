#pragma once

#include "environment.h"
#include "deps/sdl/sdlinput.h"
#include "deps/sdl/sdlscreen.h"

#include <boost/function.hpp>

#include "graphics/screen.h"
#include "audio/emyl.h"

#include <SDL/SDL.h>

namespace Guy {

class SDLEnvironment : public Environment
{
public:
	SDLEnvironment();
	virtual ~SDLEnvironment();

	virtual bool init(Game *game);
	virtual void destroy();

	virtual void run();
	virtual void pause();
	virtual void resume();
	virtual void exit();

	virtual Screen&           screen();
	virtual emyl::manager&    audio();
	virtual Input&            input();

	void setFrameRate(unsigned short frames = 0,
					   DeltaTimeType type = VariableDeltaTime);

private:
	SDLScreen*      m_screen; //graphics
	emyl::manager*  m_audio; //sound
	SDLInput*       m_input; //input

	bool m_exit;
	bool m_pause;

	unsigned short m_framesPerSecond;
	Uint32         m_ticksPerFrame;

	//-GAMELOOP-BEHAVIOR-------------------------------------------------//

	void loopVariableFastest();
	void loopVariable();
	void loopStable();
	void loopStableDrop();

	boost::function<void()> m_gameLoop;

	Uint32 m_before;
	Uint32 m_accumTime;
};

} // namespace Guy
