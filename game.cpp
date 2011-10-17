#include "game.h"

#include <iostream>
#include <cmath>

#ifdef _WINDOWS
#include <time.h>
#endif

#include "defines.h"
#include "log.h"
#include "input/input.h"
#include "graphics/screen.h"
#include "audio/emyl.h"
#include "gamestate.h"

void emylErrorCallback(const std::string &error);

Game::Game() : myState(NULL), myNextState(NULL), myFramesPerSecond(0), myGameLoop(NULL)
{
	SDL_Init(0);

	myScreen   = Screen::pInstance();
	myAudio    = emyl::manager::get_instance();
	myInput    = Input::pInstance();
	mySettings = Settings::pInstance();

	setFramesPerSecond(0);
	setStableGameTime(false, false);

#ifndef _WINDOWS
	srand(time(NULL));
#else
	srand(unsigned(time(0)));
#endif
	myExit = false;
}

Game::~Game()
{
	Unload();

	delete myScreen;
	delete myAudio;

	if(myState != NULL)
	{
		delete myState;
	}

	mySettings->Save("data/game.cfg");

	SDL_Quit();
}	

bool Game::Init()
{
	Log() << "--------------------------------------------------------------------------------" << std::endl;
	Log() << getName() << " - " << getVersion() << std::endl;
	Log() << "--------------------------------------------------------------------------------" << std::endl;

	Configure();
	mySettings->Load("data/game.cfg");
	myInput->getKeyFromSettings();

	if (!myScreen->Init())
	{
		LOG << "ERROR: Couldn't init screen." << std::endl;
		return false;
	}
	myScreen->setCaption(getName());

	if (!myAudio-> init())
	{
		LOG << "ERROR: Couldn't run sound." << std::endl;
	}
	emyl::setErrorCallback(emylErrorCallback);

	if (myNextState != NULL)
	{
		myState = myNextState;
		myNextState = NULL;

		myState->setGame(this);
		myState->Load();
	}

	Load();
	return true;
}


// Se pone la tasa de updates a la que se quiere ir.0 = variable.
void Game::setFramesPerSecond(unsigned short frames)
{
	myFramesPerSecond = frames;
	if(frames != 0)
	{
		mySecondsPerFrame = (1000.0f/myFramesPerSecond)*0.001f;
	}
	else
	{
#ifdef TICKS_PER_SECOND
		//La unidad de tiempo mas pequenya, menos ya seria gametime = 0
		mySecondsPerFrame = (1000.0f/float(TICKS_PER_SECOND))*0.001f;
#else
		mySecondsPerFrame = 0.0f;
#endif

		//Con tasa de frames automatica solo puede funcionar en
		//modo tiempo de update variable.
		ChangeGameLoop<VariableLoop>();
	}
}

// Activa updates con un GameTime fijo.
// PROS:
// 1. Si se trabaja con componentes logicos del juego que solo funcionan
// con tasas de updates fijas que no miran el GameTime, activar esta opcion
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

void Game::setStableGameTime(bool enable, bool autoframeskip)
{
	if (enable && mySecondsPerFrame > 0)
	{
		if (autoframeskip) ChangeGameLoop<StableSkipLoop>();
		else               ChangeGameLoop<StableLoop>();
	}
	else ChangeGameLoop<VariableLoop>();
}

void Game::Update(float GameTime) {}
void Game::Draw() {}
void Game::Load() {}
void Game::Unload() {}

void Game::ChangeState(GameState* next)
{
	if (myNextState != NULL) delete myNextState;
	myNextState = next;
	myNextState->setGame(this);
}

void Game::Exit()
{
	myExit = true;
}

void Game::Run()
{
	//MAIN LOOP
	while (!myExit)
	{
		myInput->Update();

		//EXIT CASE
		if(myInput->Exit())
		{
			myState->Unload();
			delete myState;
			myState = NULL;
			myExit = true;
			continue;
		}

		//UPDATES AND DRAWS
		myGameLoop->LoopIteration();

		//SHOW CHANGES
		myScreen->Flip();
		emyl::stream::updateAll();

		//STATE CHANGING
		if (myNextState != NULL)
		{
			myState->Unload();
			delete myState;
			myState = myNextState;
			myNextState = NULL;
			myState->Load();
		}
	}
}

void Game::LoopVariable(float &Now)
{
	if(Now < 0) Now = myInput->getGlobalTime();

	//GAMETIME CALCULATION
	float Before = Now;
	Now = myInput->getGlobalTime();
	float interval = Now - Before;
	if (interval < mySecondsPerFrame)
	{
		int toWait = floor(1000*(mySecondsPerFrame - interval));
		SDL_Delay(toWait);

		Now = myInput->getGlobalTimeRaw();
		interval = Now - Before;
	}

	//UPDATE&DRAW
	Update(interval);
	if(myState != NULL)
	{
		myState->Update(interval);
		myState->Draw();
	}
	Draw();
}

void Game::LoopStable(float &Now, float &AccumTime)
{
	if(Now < 0)
	{
		Now = myInput->getGlobalTime();
		AccumTime = 0;
	}

	//GAMETIME CALCULATION
	for(;;)
	{
		float Before = Now;
		Now = myInput->getGlobalTimeRaw();
		float interval = Now - Before;
		AccumTime += interval;

		if (AccumTime < mySecondsPerFrame) SDL_Delay(1);
		else break;
	}
	AccumTime -= mySecondsPerFrame;

	//UPDATE&DRAW
	Update(mySecondsPerFrame);
	if(myState != NULL)
	{
		myState->Update(mySecondsPerFrame);
		myState->Draw();
	}
	Draw();
}

void Game::LoopStableSkip(float &Now, float &AccumTime)
{
	if(Now < 0)
	{
		Now = myInput->getGlobalTime();
		AccumTime = 0;
	}

	//GAMETIME CALCULATION
	for(;;)
	{
		float Before = Now;
		Now = myInput->getGlobalTimeRaw();
		float interval = Now - Before;
		AccumTime += interval;

		if (AccumTime < mySecondsPerFrame) SDL_Delay(1);
		else break;
	}

	//UPDATE&DRAW
	while(AccumTime >= mySecondsPerFrame)
	{
		Update(mySecondsPerFrame);
		myState->Update(mySecondsPerFrame);
		AccumTime -= mySecondsPerFrame;
	}

	if(myState != NULL) myState->Draw();
	Draw();
}

Game::GameLoop::GameLoop(Game* parent) : parent(parent){}

Game::VariableLoop::VariableLoop(Game* parent) : GameLoop(parent), myNow(-1) {}
void Game::VariableLoop::LoopIteration() {parent->LoopVariable(myNow);}

Game::StableLoop::StableLoop(Game* parent) : GameLoop(parent), myNow(-1) {}
void Game::StableLoop::LoopIteration() {parent->LoopStable(myNow, myAccumTime);}

Game::StableSkipLoop::StableSkipLoop(Game* parent) : GameLoop(parent), myNow(-1) {}
void Game::StableSkipLoop::LoopIteration() {parent->LoopStableSkip(myNow, myAccumTime);}

void emylErrorCallback(const std::string &error)
{
	Log() << "emyl error: " << error << std::endl;
}
