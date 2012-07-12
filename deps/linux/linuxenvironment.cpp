#include "linuxenvironment.h"
#include "linuxpersistencelayer.h"
#include "linuxinput.h"
#include "linuxscreen.h"

#include "log.h"
#include "game.h"

#include <boost/bind.hpp>
#include <SDL/SDL.h>

void emylErrorCallback(const std::string &error)
{
	printLog("emyl error: %s\n", error.c_str());
}

LinuxEnvironment::LinuxEnvironment()
	: m_exit(false), m_framesPerSecond(0), m_pause(false)
{
}

LinuxEnvironment::~LinuxEnvironment()
{
}

bool LinuxEnvironment::init(Game *game)
{
	Environment::init(game);

	openLogFile();
	printLog("--------------------------------------------------------------------------------\n");
	printLog("%s - %s\n", m_game->getName(), m_game->getVersion());
	printLog("--------------------------------------------------------------------------------\n");

	SDL_Init(0);

	m_screen   = new LinuxScreen();
	if (!m_screen->preinit())
	{
		printLog("ERROR: Couldn't init screen.\n");
		return false;
	}

	m_audio    = emyl::manager::get_instance();
	m_input    = new LinuxInput();
	m_persistenceLayer = new LinuxPersistenceLayer();

	setFramesPerSecond(0, false, false);
	m_persistenceLayer->load("data/game.cfg");
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

void LinuxEnvironment::destroy()
{
	m_game = NULL;

	delete m_screen;
	delete m_audio;

	m_persistenceLayer->persist();

	SDL_Quit();
}

void LinuxEnvironment::setFramesPerSecond(unsigned short frames, bool stable, bool dropFrames)
{
	m_framesPerSecond = frames;
	if (frames != 0) m_ticksPerFrame = 1000/(Uint32)m_framesPerSecond;
	else m_ticksPerFrame = 0;

	if (m_ticksPerFrame == 0) m_gameLoop =  boost::bind(&LinuxEnvironment::loopVariableFastest, this);
	else {
		if (stable) {
			if (dropFrames) m_gameLoop = boost::bind(&LinuxEnvironment::loopStableDrop, this);
			else m_gameLoop = boost::bind(&LinuxEnvironment::loopStable, this);
		}
		else m_gameLoop =  boost::bind(&LinuxEnvironment::loopVariable, this);
	}

	m_accumTime = 0;
}

void LinuxEnvironment::run()
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

void LinuxEnvironment::pause()
{
	m_pause = true;
}

void LinuxEnvironment::resume()
{
	m_pause = false;
}

void LinuxEnvironment::exit()
{
	m_exit = true;
}

Screen& LinuxEnvironment::getScreenManager()
{
	return *(Screen*)m_screen;
}

emyl::manager& LinuxEnvironment::getAudioManager()
{
	return *m_audio;
}

Input& LinuxEnvironment::getInputManager()
{
	return *(Input*)m_input;
}

PersistenceLayer& LinuxEnvironment::getPersitenceLayer()
{
	return *(PersistenceLayer*)m_persistenceLayer;
}

void LinuxEnvironment::loopVariableFastest()
{
	Uint32 now = SDL_GetTicks();
	Uint32 uiDeltaTime = now - m_before;
	m_before = now;

	m_game->update(uiDeltaTime/1000.f);
	m_game->draw();
}

void LinuxEnvironment::loopVariable()
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

void LinuxEnvironment::loopStable()
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

void LinuxEnvironment::loopStableDrop()
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
