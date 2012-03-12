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

Game::Game() : m_State(NULL), m_NextState(NULL), m_FramesPerSecond(0), myGameLoop(NULL)
{
	SDL_Init(0);

	m_screen   = Screen::ptrInstance();
	m_audio    = emyl::manager::get_instance();
	m_input    = Input::ptrInstance();
	m_settings = Settings::pInstance();

	setFramesPerSecond(0);
	setStableDeltaTime(false, false);

#ifndef _WINDOWS
	srand(time(NULL));
#else
	srand(unsigned(time(0)));
#endif
	m_Exit = false;
}

Game::~Game()
{
	unload();

	delete m_screen;
	delete m_audio;

	if(m_State != NULL)
	{
		delete m_State;
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

	if (m_NextState != NULL)
	{
		m_State = m_NextState;
		m_NextState = NULL;

		m_State->setGame(this);
		m_State->load();
	}

	load();
	return true;
}


// Se pone la tasa de updates a la que se quiere ir.0 = variable.
void Game::setFramesPerSecond(unsigned short frames)
{
	m_FramesPerSecond = frames;
	if(frames != 0)
	{
		m_SecondsPerFrame = (1000.0f/m_FramesPerSecond)*0.001f;
	}
	else
	{
#ifdef TICKS_PER_SECOND
		//La unidad de tiempo mas pequenya, menos ya seria gametime = 0
		m_SecondsPerFrame = (1000.0f/float(TICKS_PER_SECOND))*0.001f;
#else
		mySecondsPerFrame = 0.0f;
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
	if (enable && m_SecondsPerFrame > 0)
	{
		if (autoframeskip) changeGameLoop<StableSkipLoop>();
		else               changeGameLoop<StableLoop>();
	}
	else changeGameLoop<VariableLoop>();
}

void Game::update(float deltaTime) {}
void Game::draw() {}
void Game::load() {}
void Game::unload() {}

void Game::changeState(GameState* next)
{
	if (m_NextState != NULL) delete m_NextState;
	m_NextState = next;
	m_NextState->setGame(this);
}

void Game::exit()
{
	m_Exit = true;
}

void Game::run()
{
	//MAIN LOOP
	while (!m_Exit)
	{
		m_input->update();

		//EXIT CASE
		if(m_input->exit())
		{
			m_State->unload();
			delete m_State;
			m_State = NULL;
			m_Exit = true;
			continue;
		}

		//UPDATES AND DRAWS
		myGameLoop->loopIteration();

		//SHOW CHANGES
		m_screen->flip();
		emyl::stream::updateAll();

		//STATE CHANGING
		if (m_NextState != NULL)
		{
			m_State->unload();
			delete m_State;
			m_State = m_NextState;
			m_NextState = NULL;
			m_State->load();
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
	if (deltaTime < m_SecondsPerFrame)
	{
		int toWait = floor(1000*(m_SecondsPerFrame - deltaTime));
		SDL_Delay(toWait);

		now = m_input->getTimeRaw();
		deltaTime = now - before;
	}

	//UPDATE&DRAW
	update(deltaTime);
	if(m_State != NULL)
	{
		m_State->update(deltaTime);
		m_State->draw();
	}
	draw();
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

		if (accumTime < m_SecondsPerFrame) SDL_Delay(1);
		else break;
	}
	accumTime -= m_SecondsPerFrame;

	//UPDATE&DRAW
	update(m_SecondsPerFrame);
	if(m_State != NULL)
	{
		m_State->update(m_SecondsPerFrame);
		m_State->draw();
	}
	draw();
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

		if (accumTime < m_SecondsPerFrame) SDL_Delay(1);
		else break;
	}

	//UPDATE&DRAW
	while(accumTime >= m_SecondsPerFrame)
	{
		update(m_SecondsPerFrame);
		m_State->update(m_SecondsPerFrame);
		accumTime -= m_SecondsPerFrame;
	}

	if(m_State != NULL) m_State->draw();
	draw();
}

Game::GameLoop::GameLoop(Game* parent) : parent(parent){}

Game::VariableLoop::VariableLoop(Game* parent) : GameLoop(parent), myNow(-1) {}
void Game::VariableLoop::loopIteration() {parent->loopVariable(myNow);}

Game::StableLoop::StableLoop(Game* parent) : GameLoop(parent), myNow(-1) {}
void Game::StableLoop::loopIteration() {parent->loopStable(myNow, myAccumTime);}

Game::StableSkipLoop::StableSkipLoop(Game* parent) : GameLoop(parent), myNow(-1) {}
void Game::StableSkipLoop::loopIteration() {parent->loopStableSkip(myNow, myAccumTime);}

void emylErrorCallback(const std::string &error)
{
	Log() << "emyl error: " << error << std::endl;
}
