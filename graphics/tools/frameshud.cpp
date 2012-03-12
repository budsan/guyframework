#include "frameshud.h"
#include "graphics/screen.h"

#include <sstream>
#include <math.h>

FramesHUD::FramesHUD()
{
	setColor(rgba(1,0,1,1));
	setDisplayTime(1.0f);
	m_isFontLoaded = false;

	reset();
}

bool FramesHUD::loadFont(const char *filename)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	unsigned int h = viewport[3] - viewport[1];

	if (m_font.load(filename, h/32))
	{
		m_isFontLoaded = true;
		m_font.setAlignment(Font::LEFT);
		return true;
	}

	return false;
}

void FramesHUD::setColor(const rgba &color)
{
	m_color = color;
}

void FramesHUD::setDisplayTime(float time)
{
	if (time <= 0 ) time = 1.0f;
	m_displayTime = time;
	m_displayTimeInv = 1.0f/time;
}
void FramesHUD::update(float deltaTime)
{
	m_timeCount += deltaTime;
	if(m_timeCount > m_displayTime)
	{
		float currentFramesToDisplay  = float(m_framesCount) * m_displayTimeInv;
		float timesHappenedDisplayTime = floor(m_timeCount * m_displayTimeInv);
		currentFramesToDisplay /= timesHappenedDisplayTime;
		m_framesToDisplay = currentFramesToDisplay*0.8 + m_framesToDisplay*0.2;

		m_timeCount -= timesHappenedDisplayTime * m_displayTime;
		m_framesCount = 0;
	}
}

void FramesHUD::draw()
{
	m_framesCount++;
	if (m_isFontLoaded)
	{
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		unsigned int w = viewport[0];
		unsigned int h = viewport[1];

		std::stringstream ss;
		ss.unsetf(std::ios::floatfield);
		ss.precision(5);
		ss << m_framesToDisplay;

		m_font.draw2D(ss.str().c_str(), math::vec2f(w, h), m_color);
	}
}

void FramesHUD::reset()
{
	m_timeCount = 0;
	m_framesToDisplay = 0;
	m_framesCount = 0;
}
