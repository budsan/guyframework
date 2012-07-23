#pragma once

#include "environment.h"

namespace Guy {

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

	virtual const char *name();
	virtual const char *version();
};

} //namespace Guy

