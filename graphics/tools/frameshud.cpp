#include "frameshud.h"
#include "graphics/screen.h"

FramesHUD::FramesHUD()
{
	setColor(rgba(1,0,1,1));
	setDisplayTime(1.0f);
	myIsFontLoaded = false;

	Reset();
}

bool FramesHUD::LoadFont(const char *filename)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	unsigned int h = viewport[3] - viewport[1];

	if (myFont.Load(filename, h/32))
	{
		myIsFontLoaded = true;
		myFont.setAlignment(Font::TOP, Font::LEFT);
		return true;
	}

	return false;
}

void FramesHUD::setColor(const rgba &color)
{
	myColor = color;
}

void FramesHUD::setDisplayTime(float time)
{
	if (time <= 0 ) time = 1.0f;
	myDisplayTime = time;
	myDisplayTimeInv = 1.0f/time;
}
void FramesHUD::Update(float GameTime)
{
	myTimeCount += GameTime;
	if(myTimeCount > myDisplayTime)
	{
		float currentFramesToDisplay  = float(myNumOfFrames) * myDisplayTimeInv;
		float timesHappenedDisplayTime = floor(myTimeCount * myDisplayTimeInv);
		currentFramesToDisplay /= timesHappenedDisplayTime;
		myFramesToDisplay = currentFramesToDisplay*0.8 + myFramesToDisplay*0.2;

		myTimeCount -= timesHappenedDisplayTime * myDisplayTime;
		myNumOfFrames = 0;
	}
}

void FramesHUD::Draw()
{
	myNumOfFrames++;
	if (myIsFontLoaded)
	{
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		unsigned int w = viewport[0];
		unsigned int h = viewport[1];

		glPushAttrib(GL_CURRENT_BIT);
		glColor(myColor);
		myFont.Print2D(w, h, "%4.2f", myFramesToDisplay);
		glPopAttrib();
	}
}

void FramesHUD::Reset()
{
	myTimeCount = 0;
	myFramesToDisplay = 0;
	myNumOfFrames = 0;
}
