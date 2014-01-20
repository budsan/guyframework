#pragma once

#include "graphics/font.h"
#include "graphics/color.h"

namespace Guy {

class FramesHUD
{
 public:
	FramesHUD();
	bool loadFont(const char *filename);
	void setColor(const rgba &color);
	void setDisplayTime(double time);

	void update(double deltaTime);
	void draw();
	void reset();

protected:
	Font m_font;
	rgba m_color;
	double m_displayTime, m_displayTimeInv;
	double m_timeCount;
	double m_framesToDisplay;
	unsigned short m_framesCount;
	bool m_isFontLoaded;
};

} // namespace Guy
