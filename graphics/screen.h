#pragma once

#include "graphics.h"
#include "color.h"
#include <map>
#include <string>

class Screen
{
public:
	virtual void flip() = 0;
	virtual void fillWithColor(const rgba &color) = 0;

	virtual float getRatio() = 0;
	virtual void  setRatio(float) = 0;

	typedef struct
	{
		unsigned int w;
		unsigned int h;
		unsigned int bpp;
	} vmode;

	virtual const vmode* getVideoModeList(unsigned int &size) = 0;
	virtual const vmode* getCurrentVideoMode() = 0;
};

