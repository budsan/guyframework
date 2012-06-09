#include "game.h"

#include <iostream>
#include <cmath>

#ifdef _WINDOWS
#include <time.h>
#endif

#include "log.h"
#include "input/input.h"
#include "graphics/screen.h"
#include "audio/emyl.h"
#include "gamestate.h"

void emylErrorCallback(const std::string &error);

Game::Game() : m_state(NULL), m_nextState(NULL), m_framesPerSecond(0), m_gameLoop(NULL)
{
	SDL_Init(0);

	m_screen   = Screen::ptrInstance();
	m_audio    = emyl::manager::get_instance();
	m_input    = Input::ptrInstance();
	m_settings = Settings::pInstance();

	setFramesPerSecond(0);
	setStableDeltaTime(false, false);

#ifndef _WINDOWS
	srand(unsigned(time(NULL)));
#else
	srand(unsigned(time(0)));
#endif
	m_exit = false;
	m_pause = false;
	m_ignoreNextDeltaTime = false;
}

Game::~Game()
{
	unload();

	delete m_screen;
	delete m_audio;

	if(m_state != NULL)
	{
		delete m_state;
	}

	m_settings->Save("data/game.cfg");

	SDL_Quit();
}	

bool Game::init()
{
	Log() << "--------------------------------------------------------------------------------" << std::endl;
	Log() << getName() << " - " << getVersion() << std::endl;
	Log() << "--------------------------------------------------------------------------------" << std::endl;

	configure();
	m_settings->load("data/game.cfg");
	m_input->getKeyFromSettings();
	m_input->setFocusObserver(this);

	if (!m_screen->init())
	{
		LOG << "ERROR: Couldn't init screen." << std::endl;
		return false;
	}
	m_screen->setCaption(getName());

	if (!m_audio-> init())
	{
		LOG << "ERROR: Couldn't run sound." << std::endl;
	}
	emyl::setErrorCallback(emylErrorCallback);

	if (m_nextState != NULL)
	{
		m_state = m_nextState;
		m_nextState = NULL;

		m_state->setGame(this);
		m_state->load();
	}

	load();
	return true;
}


// Se pone la tasa de updates a la que se quiere ir.0 = variable.
void Game::setFramesPerSecond(unsigned short frames)
{
	m_framesPerSecond = frames;
	if(frames != 0)
	{
		m_secondsPerFrame = (1000.0f/m_framesPerSecond)*0.001f;
	}
	else
	{
#ifdef TICKS_PER_SECOND
		//La unidad de tiempo mas pequenya, menos ya seria gametime = 0
		m_secondsPerFrame = (1000.0f/float(TICKS_PER_SECOND))*0.001f;
#else
		m_SecondsPerFrame = 0.0f;
#endif

		//Con tasa de frames automatica solo puede funcionar en
		//modo tiempo de update variable.
		changeGameLoop<VariableLoop>();
	}
}

// Activa updates con un deltaTime fijo.
// PROS:
// 1. Si se trabaja con componentes logicos del juego que solo funcionan
// con tasas de updates fijas que no miran el deltaTime, activar esta opcion
// hara que los componentes logicos con tasas de updates variables vayan
// perfectamente sincronizados con los componentes de tasas de updates fijas.
//
// CONSTRAS:
// 1. Si en alguna ocasion una iteracion del gameloop tarda mas que la
// tasa de update, el juego sufrira relentizaciones. En estos casos se puede
// activar el autoframeskip. Si se activa se ignoraran algunos frames con tal
// de reducir el tiempo de iteracion del gameloop y conseguir que el juego vaya
// a tiempo real, pero la experiencia de juego empeorara.
// 2. Trabajar con tasas de updates fijas hace que los componentes logicos con
// tasas de updates variables pierdan todas sus ventajas.

void Game::setStableDeltaTime(bool enable, bool autoframeskip)
{
	if (enable && m_secondsPerFrame > 0)
	{
		if (autoframeskip) changeGameLoop<StableSkipLoop>();
		else               changeGameLoop<StableLoop>();
	}
	else changeGameLoop<VariableLoop>();
}

void Game::preUpdate(float deltaTime) {}
void Game::postUpdate(float deltaTime) {}

void Game::preDraw() {}
void Game::postDraw() {}

void Game::load() {}
void Game::unload() {}

void Game::Pause()
{
	m_pause = true;
}

void Game::Resume()
{
	m_pause = false;
}

void Game::changeState(GameState* next)
{
	if (m_nextState != NULL) delete m_nextState;
	m_nextState = next;
	m_nextState->setGame(this);
}

void Game::exit()
{
	m_exit = true;
}

void Game::run()
{
	//MAIN LOOP
	while (!m_exit)
	{
		if (m_pause)
		{
			m_audio->sleep();
			while (!m_input->exit() && m_pause) m_input->waitEvent();
			m_audio->unsleep();

			m_ignoreNextDeltaTime = true;
		}

		m_input->update();

		//EXIT CASE
		if(m_input->exit())
		{
			m_state->unload();
			delete m_state;
			m_state = NULL;
			m_exit = true;
			continue;
		}

		//UPDATES AND DRAWS
		m_gameLoop->loopIteration();

		//SHOW CHANGES
		m_screen->flip();
		emyl::stream::updateAll();

		//STATE CHANGING
		if (m_nextState != NULL)
		{
			m_state->unload();
			delete m_state;
			m_state = m_nextState;
			m_nextState = NULL;
			m_state->load();
		}
	}
}

void Game::loopVariable(float &now)
{
	if(now < 0) now = m_input->getTime();

	//GAMETIME CALCULATION
	float before = now;
	now = m_input->getTime();
	float deltaTime = now - before;
	if (deltaTime < m_secondsPerFrame)
	{
		unsigned int toWait = (unsigned int)floor(1000*(m_secondsPerFrame - deltaTime));
		m_input->delay(toWait);

		now = m_input->getTimeRaw();
		deltaTime = now - before;
	}

	//UPDATE&DRAW
	updateInternal(deltaTime);
	drawInternal();
}

void Game::loopStable(float &now, float &accumTime)
{
	if(now < 0)
	{
		now = m_input->getTime();
		accumTime = 0;
	}

	//GAMETIME CALCULATION
	for(;;)
	{
		float before = now;
		now = m_input->getTimeRaw();
		float deltaTime = now - before;
		accumTime += deltaTime;

		if (accumTime < m_secondsPerFrame) m_input->delay(1);
		else break;
	}
	accumTime -= m_secondsPerFrame;

	//UPDATE&DRAW
	updateInternal(m_secondsPerFrame);
	drawInternal();
}

void Game::loopStableSkip(float &now, float &accumTime)
{
	if(now < 0)
	{
		now = m_input->getTime();
		accumTime = 0;
	}

	//GAMETIME CALCULATION
	for(;;)
	{
		float before = now;
		now = m_input->getTimeRaw();
		float deltaTime = now - before;
		accumTime += deltaTime;

		if (accumTime < m_secondsPerFrame) m_input->delay(1);
		else break;
	}

	//UPDATE&DRAW
	while(accumTime >= m_secondsPerFrame)
	{
		updateInternal(m_secondsPerFrame);
		accumTime -= m_secondsPerFrame;
	}

	drawInternal();
}

void Game::updateInternal(float deltaTime)
{
	if (m_ignoreNextDeltaTime) m_ignoreNextDeltaTime = false;
	else {
		preUpdate(deltaTime);
		if(m_state != NULL) m_state->update(deltaTime);
		postUpdate(deltaTime);
	}
}

void Game::drawInternal()
{
	preDraw();
	if(m_state != NULL) m_state->draw();
	postDraw();
}

Game::GameLoop::GameLoop(Game* parent) : m_parent(parent){}

Game::VariableLoop::VariableLoop(Game* parent) : GameLoop(parent), m_now(-1) {}
void Game::VariableLoop::loopIteration() {m_parent->loopVariable(m_now);}

Game::StableLoop::StableLoop(Game* parent) : GameLoop(parent), m_now(-1) {}
void Game::StableLoop::loopIteration() {m_parent->loopStable(m_now, m_accumTime);}

Game::StableSkipLoop::StableSkipLoop(Game* parent) : GameLoop(parent), m_now(-1) {}
void Game::StableSkipLoop::loopIteration() {m_parent->loopStableSkip(m_now, m_accumTime);}

void emylErrorCallback(const std::string &error)
{
	Log() << "emyl error: " << error << std::endl;
}
