#include "qnxenvironment.h"
#include "qnxinput.h"
#include "qnxscreen.h"

#include "log.h"
#include "game.h"

#include <boost/bind.hpp>

#include <time.h>
#include <sys/time.h>

namespace Guy {

void emylErrorCallback(const std::string &error)
{
	printLog("emyl error: %s\n", error.c_str());
}

QNXEnvironment::QNXEnvironment()
	: m_exit(false), m_framesPerSecond(0), m_pause(false)
{
}

QNXEnvironment::~QNXEnvironment()
{
}

bool QNXEnvironment::init(Game *game)
{
	Environment::init(game);

	openLogFile();
	printLog("--------------------------------------------------------\n");
	printLog(" %s - %s\n", m_game->getName(), m_game->getVersion());
	printLog("--------------------------------------------------------\n");

	bps_initialize();

	m_screen   = new SDLScreen();
	if (!m_screen->preinit())
	{
		printLog("ERROR: Couldn't init screen.\n");
		return false;
	}

	m_audio    = emyl::manager::get_instance();
	m_input    = new SDLInput();

	setFramesPerSecond(0, false, false);
	m_game->init();

	if (!m_screen->init())
	{
		printLog("ERROR: Couldn't init screen.\n");
		return false;
	}
	m_screen->setCaption(m_game->getName());

	if (!m_audio-> init())
	{
		printLog("ERROR: Couldn't run sound.\n");
	}
	emyl::setErrorCallback(emylErrorCallback);

	m_game->load();
	return true;
}

void QNXEnvironment::destroy()
{
	m_game = NULL;

	delete m_screen;
	delete m_audio;

	SDL_Quit();
}

void QNXEnvironment::setFramesPerSecond(unsigned short frames, bool stable, bool dropFrames)
{
	m_framesPerSecond = frames;
	if (frames != 0) m_ticksPerFrame = 1000/(Uint32)m_framesPerSecond;
	else m_ticksPerFrame = 0;

	if (m_ticksPerFrame == 0) m_gameLoop =  boost::bind(&QNXEnvironment::loopVariableFastest, this);
	else {
		if (stable) {
			if (dropFrames) m_gameLoop = boost::bind(&QNXEnvironment::loopStableDrop, this);
			else m_gameLoop = boost::bind(&QNXEnvironment::loopStable, this);
		}
		else m_gameLoop =  boost::bind(&QNXEnvironment::loopVariable, this);
	}

	m_accumTime = 0;
}

void QNXEnvironment::run()
{
	if (m_game == NULL) return;
	m_before = SDL_GetTicks();

	//MAIN LOOP
	while (!m_exit)
	{
		if (m_pause)
		{
			m_audio->sleep();
			while (!m_exit && m_pause) m_input->waitEvent();
			m_audio->unsleep();
			m_before = SDL_GetTicks();
		}

		m_input->pollEvents();

		//EXIT CASE
		if(m_exit) continue;

		//UPDATES AND DRAWS
		m_gameLoop();

		//SHOW CHANGES
		m_screen->flip();
		emyl::stream::updateAll();
	}

	m_game->unload();
	m_game = NULL;
}

void QNXEnvironment::pause()
{
	m_pause = true;
}

void QNXEnvironment::resume()
{
	m_pause = false;
}

void QNXEnvironment::exit()
{
	m_exit = true;
}

Screen& QNXEnvironment::getScreenManager()
{
	return *(Screen*)m_screen;
}

emyl::manager& QNXEnvironment::getAudioManager()
{
	return *m_audio;
}

Input& QNXEnvironment::getInputManager()
{
	return *(Input*)m_input;
}

void QNXEnvironment::loopVariableFastest()
{
	Uint32 now = SDL_GetTicks();
	Uint32 uiDeltaTime = now - m_before;
	m_before = now;

	m_game->update(uiDeltaTime/1000.f);
	m_game->draw();
}

void QNXEnvironment::loopVariable()
{
	Uint32 now = SDL_GetTicks();
	Uint32 uiDeltaTime = now - m_before;
	if (uiDeltaTime < m_ticksPerFrame)
	{
		SDL_Delay(m_ticksPerFrame - uiDeltaTime);
		now = SDL_GetTicks();
		uiDeltaTime = now - m_before;
	}
	m_before = now;

	m_game->update(uiDeltaTime/1000.f);
	m_game->draw();
}

void QNXEnvironment::loopStable()
{
	for(;;)
	{
		Uint32 now = SDL_GetTicks();
		Uint32 uiDeltaTime = now - m_before;
		Uint32 currAccumTime = m_accumTime + uiDeltaTime;

		if (currAccumTime < m_ticksPerFrame) {
			SDL_Delay(m_ticksPerFrame - currAccumTime);
		}
		else {
			m_before = now;
			m_accumTime = currAccumTime - m_ticksPerFrame;
			break;
		}
	}

	m_game->update(m_ticksPerFrame/1000.f);
	m_game->draw();
}

void QNXEnvironment::loopStableDrop()
{
	for(;;)
	{
		Uint32 now = SDL_GetTicks();
		Uint32 uiDeltaTime = now - m_before;
		Uint32 currAccumTime = m_accumTime + uiDeltaTime;

		if (currAccumTime < m_ticksPerFrame) {
			SDL_Delay(m_ticksPerFrame - currAccumTime);
		}
		else {
			m_before = now;
			m_accumTime = currAccumTime - m_ticksPerFrame;
			break;
		}
	}

	while(m_accumTime >= m_ticksPerFrame)
	{
		m_game->update(m_ticksPerFrame);
		m_accumTime -= m_ticksPerFrame;
	}

	m_game->draw();
}

void QNXEnvironment::delay (float secs)
{
	struct timeval tv;
	int microsecs = secs * 1000000;
	tv.tv_sec  = microsecs / 1000000;
	tv.tv_usec = microsecs % 1000000;
	select (0, NULL, NULL, NULL, &tv);
}

float QNXEnvironment::time()
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (double) ts.tv_sec + ts.tv_nsec / 1000000000.0;
}

} // namespace Guy
