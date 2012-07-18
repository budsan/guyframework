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

	screen_create_context(&screen_ctx, 0);
	if (BPS_SUCCESS != screen_request_events(screen_ctx))
	{
		printLog("screen_request_events failed\n");
		screen_destroy_context(screen_ctx);
		return false;
	}

	m_screen = new QNXScreen(screen_ctx);
	m_audio  = emyl::manager::get_instance();
	m_input  = new QNXInput(screen_ctx);

	setFramesPerSecond(0, false, false);
	m_game->init();

	if (!m_screen->init())
	{
		printLog("ERROR: Couldn't init screen.\n");
		return false;
	}
	//m_screen->setCaption(m_game->getName());

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

	bps_shutdown();
}

void QNXEnvironment::setFramesPerSecond(unsigned short frames, bool stable, bool dropFrames)
{
	m_framesPerSecond = frames;
	if (frames != 0) m_secsPerFrame = 1.0f/(float)m_framesPerSecond;
	else m_secsPerFrame = 0;

	if (m_secsPerFrame == 0) m_gameLoop =  boost::bind(&QNXEnvironment::loopVariableFastest, this);
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
	m_before = time();

	//MAIN LOOP
	while (!m_exit)
	{
		if (m_pause)
		{
			m_audio->sleep();
			while (!m_exit && m_pause) m_input->waitEvent();
			m_audio->unsleep();
			m_before = time();
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

PersistenceLayer& QNXEnvironment::getPersitenceLayer()
{
	return *(PersistenceLayer*)m_persistenceLayer;
}

void QNXEnvironment::loopVariableFastest()
{
	float now = time();
	float fDeltaTime = now - m_before;
	m_before = now;

	m_game->update(fDeltaTime);
	m_game->draw();
}

void QNXEnvironment::loopVariable()
{
	float now = time();
	float fDeltaTime = now - m_before;
	if (fDeltaTime < m_secsPerFrame)
	{
		delay(m_secsPerFrame - fDeltaTime);
		now = time();
		fDeltaTime = now - m_before;
	}
	m_before = now;

	m_game->update(fDeltaTime);
	m_game->draw();
}

void QNXEnvironment::loopStable()
{
	for(;;)
	{
		float now = time();
		float fDeltaTime = now - m_before;
		float currAccumTime = m_accumTime + fDeltaTime;

		if (currAccumTime < m_secsPerFrame) {
			delay(m_secsPerFrame - currAccumTime);
		}
		else {
			m_before = now;
			m_accumTime = currAccumTime - m_secsPerFrame;
			break;
		}
	}

	m_game->update(m_secsPerFrame);
	m_game->draw();
}

void QNXEnvironment::loopStableDrop()
{
	for(;;)
	{
		float now = time();
		float fDeltaTime = now - m_before;
		float currAccumTime = m_accumTime + fDeltaTime;

		if (currAccumTime < m_secsPerFrame) {
			delay(m_secsPerFrame - currAccumTime);
		}
		else {
			m_before = now;
			m_accumTime = currAccumTime - m_secsPerFrame;
			break;
		}
	}

	while(m_accumTime >= m_secsPerFrame)
	{
		m_game->update(m_secsPerFrame);
		m_accumTime -= m_secsPerFrame;
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
