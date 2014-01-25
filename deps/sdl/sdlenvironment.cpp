#include "sdlenvironment.h"
#include "sdlinput.h"
#include "sdlscreen.h"

#include "debug.h"
#include "game.h"

#include <functional>
#include <SDL2/SDL.h>

namespace Guy {

SDLEnvironment::SDLEnvironment()
	: m_exit(false), m_pause(false), m_framesPerSecond(0)
{
}

SDLEnvironment::~SDLEnvironment()
{
}

bool SDLEnvironment::init(Game *_game)
{
	Environment::init(_game);

	openLogFile();
	printLog("--------------------------------------------------------\n");
	printLog(" %s - %s\n", m_game->name(), m_game->version());
	printLog("--------------------------------------------------------\n");

	SDL_Init(0);

	m_screen   = new SDLScreen();
	m_screen->setCaption(m_game->name());

	if (!m_screen->preinit())
	{
		printLog("ERROR: Couldn't init screen.\n");
		return false;
	}

	m_audio    = emyl::manager::get_instance();
	m_input    = new SDLInput();

	setFrameRate();
	m_game->init();

	if (!m_screen->init())
	{
		printLog("ERROR: Couldn't init screen.\n");
		return false;
	}

	if (!m_audio-> init())
	{
		printLog("ERROR: Couldn't run sound.\n");
	}

	emyl::setErrorCallback([](const std::string &error){
		printLog("emyl error: %s\n", error.c_str());
	});

	m_game->load();
	return true;
}

void SDLEnvironment::destroy()
{
	m_game = NULL;

	delete m_screen;
	delete m_audio;

	SDL_Quit();
}

void SDLEnvironment::setFrameRate(unsigned short frames, DeltaTimeType type)
{
	m_framesPerSecond = frames;
	if (frames != 0)
		m_ticksPerFrame = 1000/(Uint32)m_framesPerSecond;
	else
		m_ticksPerFrame = 0;

	if (m_ticksPerFrame == 0)
		m_gameLoop =  std::bind(&SDLEnvironment::loopVariableFastest, this);
	else
	{
		switch(type)
		{
		case VariableDeltaTime:
			m_gameLoop = std::bind(&SDLEnvironment::loopVariable, this);
			break;
		case StableDeltaTime:
			m_gameLoop = std::bind(&SDLEnvironment::loopStable, this);
			break;
		case StableWithFrameSkipDeltaTime:
			m_gameLoop = std::bind(&SDLEnvironment::loopStableDrop, this);
			break;
		default:
			m_gameLoop = std::bind(&SDLEnvironment::loopVariableFastest, this);
			break;
		}
	}

	m_accumTime = 0;
}

void SDLEnvironment::run()
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
		if (m_exit) continue;

		//UPDATES AND DRAWS
		m_gameLoop();

		//SHOW CHANGES
		m_screen->flip();
		emyl::stream::updateAll();
	}

	m_game->unload();
	m_game = NULL;
}

void SDLEnvironment::pause()
{
	m_pause = true;
}

void SDLEnvironment::resume()
{
	m_pause = false;
}

void SDLEnvironment::exit()
{
	m_exit = true;
}

Screen& SDLEnvironment::screen()
{
	return *(Screen*)m_screen;
}

emyl::manager& SDLEnvironment::audio()
{
	return *m_audio;
}

Input& SDLEnvironment::input()
{
	return *(Input*)m_input;
}

void SDLEnvironment::loopVariableFastest()
{
	Uint32 now = SDL_GetTicks();
	Uint32 uiDeltaTime = now - m_before;
	m_before = now;

	m_game->update(uiDeltaTime/1000.0);
	m_game->draw();
}

void SDLEnvironment::loopVariable()
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

	m_game->update(uiDeltaTime/1000.0);
	m_game->draw();
}

void SDLEnvironment::loopStable()
{
	for (;;)
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

	m_game->update(m_ticksPerFrame/1000.0);
	m_game->draw();
}

void SDLEnvironment::loopStableDrop()
{
	for (;;)
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

	while (m_accumTime >= m_ticksPerFrame)
	{
		m_game->update(m_ticksPerFrame/1000.0);
		m_accumTime -= m_ticksPerFrame;
	}

	m_game->draw();
}

} // namespace Guy
