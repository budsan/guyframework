#pragma once

#include "environment.h"

class Game
{
public:
	Game();
	~Game();

	virtual void init();
	virtual void load();

	virtual void update(float deltaTime) = 0;
	virtual void draw() = 0;

	virtual void unload();

	virtual const char *getName();
	virtual const char *getVersion();
};

