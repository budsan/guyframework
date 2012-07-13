#pragma once

#include "environment.h"
#include "linuxpersistencelayer.h"
#include "linuxinput.h"
#include "linuxscreen.h"

#include <boost/function.hpp>

#include "graphics/screen.h"
#include "audio/emyl.h"

#include <SDL/SDL.h>

namespace Guy {

class LinuxEnvironment : public Environment
{
public:
	LinuxEnvironment();
	virtual ~LinuxEnvironment();

	virtual bool init(Game *game);
	virtual void destroy();

	virtual void run();
	virtual void pause();
	virtual void resume();
	virtual void exit();

	virtual Screen&           getScreenManager();
	virtual emyl::manager&    getAudioManager();
	virtual Input&            getInputManager();
	virtual PersistenceLayer& getPersitenceLayer();

	void setFramesPerSecond(unsigned short frames = 0, bool stable = false, bool dropFrames = false);

private:
	LinuxScreen*           m_screen; //graphics
	emyl::manager*         m_audio; //sound
	LinuxInput*            m_input; //input
	LinuxPersistenceLayer* m_persistenceLayer;

	bool  m_exit;

	unsigned short m_framesPerSecond;
	Uint32         m_ticksPerFrame;

	bool m_pause;

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
