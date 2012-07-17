#pragma once

#include "environment.h"
#include "deps/bb10/qnxinput.h"
#include "deps/bb10/qnxscreen.h"
#include "deps/bb10/qnxpersistencelayer.h"

#include <boost/function.hpp>

#include "graphics/screen.h"
#include "audio/emyl.h"

#include <bps/bps.h>

namespace Guy {

class QNXEnvironment : public Environment
{
public:
	QNXEnvironment();
	virtual ~QNXEnvironment();

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

	static void delay (float secs);
	static float time();

private:
	QNXScreen*           m_screen; //graphics
	emyl::manager*       m_audio; //sound
	QNXInput*            m_input; //input
	QNXPersistenceLayer* m_persistenceLayer;

	screen_context_t screen_ctx;

	bool m_exit;
	bool m_pause;

	unsigned short m_framesPerSecond;
	float         m_secsPerFrame;

	//-GAMELOOP-BEHAVIOR-------------------------------------------------//

	void loopVariableFastest();
	void loopVariable();
	void loopStable();
	void loopStableDrop();

	boost::function<void()> m_gameLoop;

	float m_before;
	float m_accumTime;
};

} // namespace Guy
