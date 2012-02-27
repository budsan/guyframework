#include "texthud.h"
#include "graphics/screen.h"

TextHUD::TextHUD()
{
	setColor(rgba(1,0,1,1));

	m_displayText = "";
	m_isFontLoaded = false;
	m_clampedPos = math::vec2f(1,1);

	reset();
}

bool TextHUD::loadFont(const char *filename)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	unsigned int h = viewport[3] - viewport[1];

	if (m_font.load(filename, h/32))
	{
		m_isFontLoaded = true;
		m_font.setAlignment(Font::TOP, Font::LEFT);
		return true;
	}

	return false;
}

void TextHUD::setColor(const rgba &color)
{
	m_color = color;
}

void TextHUD::update(float deltaTime)
{

}

void TextHUD::draw()
{
	if (m_isFontLoaded)
	{
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		unsigned int w = viewport[2];
		unsigned int h = viewport[3];

		glPushAttrib(GL_CURRENT_BIT);
		glColor(m_color);
		m_font.print2D(math::vec2f(m_clampedPos.x*w, m_clampedPos.y*h), m_displayText.c_str());
		glPopAttrib();
	}
}

void TextHUD::reset()
{

}

