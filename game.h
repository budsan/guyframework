#pragma once

#include "environment.h"

class Game
{
public:
	Game();
	~Game();

	virtual void init() {}
	virtual void load() {}

	virtual void update(float deltaTime) {deltaTime = 0;}
	virtual void draw() {}

	virtual void unload() {}

	virtual const char *getName() {return "Game";}
	virtual const char *getVersion() {return "Undefined";}
};

