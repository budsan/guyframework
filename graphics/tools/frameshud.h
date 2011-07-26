#pragma once

#include "graphics/font.h"
#include "graphics/color.h"

class FramesHUD
{
 public:
        FramesHUD();
        bool LoadFont(const char *filename);
	void setColor(const rgba &color);
	void setDisplayTime(float time);

        void Update(float GameTime);
	void Draw();
	void Reset();

protected:
	Font myFont;
	rgba myColor;
	float myDisplayTime, myDisplayTimeInv;
	float myTimeCount;
	float myFramesToDisplay;
	unsigned short myNumOfFrames;
	bool myIsFontLoaded;
};
