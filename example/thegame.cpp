#include "thegame.h"
#include <iostream>

TheGame::TheGame()
{

}

void TheGame::Configure()
{
	//Asi haremos que el juego sea 16:9, si ponemos 0 se
	//pondra el ratio automaticamente al de la resolucion escogida
	myScreen->setRatio(16.0/9.0);
	//Y asi se pondra el ratio en funcion de la resolucion
	//myScreen->setRatio(0);

	//Para que el juego vaya a 60 frames por segundo
	setFramesPerSecond(50);
	//Para que vaya a un framerate variable
	//setFramesPerSecond(0);

	//Para que GameTime sea estable o no
	setStableGameTime(false);

	//Decimos cual sera nuestro primer estado de juego
	ChangeState(new SplashState());

	//Tengo creado un FramesHUD en mi TheGame
	if(!frames.LoadFont("data/font/nibby.ttf"))
	{
		std::cerr << "Cannot load data/font/nibby.ttf" << std::endl;
	}
	frames.setDisplayTime(0.1f);
}

void TheGame::Update(float GameTime)
{
	frames.Update(GameTime);
}

void TheGame::Draw()
{
	frames.Draw();
}


