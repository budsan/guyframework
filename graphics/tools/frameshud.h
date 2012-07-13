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
	void setDisplayTime(float time);

	void update(float deltaTime);
	void draw();
	void reset();

protected:
	Font m_font;
	rgba m_color;
	float m_displayTime, m_displayTimeInv;
	float m_timeCount;
	float m_framesToDisplay;
	unsigned short m_framesCount;
	bool m_isFontLoaded;
};

} // namespace Guy
