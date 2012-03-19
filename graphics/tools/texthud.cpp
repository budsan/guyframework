#include "texthud.h"
#include "graphics/screen.h"

TextHUD::TextHUD() : m_font()
{
	setColor(rgba(1,0,1,1));

	m_displayText = "";
	m_clampedPos = math::vec2f(1,1);

	reset();
}

bool TextHUD::loadFont(const char *filename)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	unsigned int h = viewport[3] - viewport[1];

    m_font = boost::shared_ptr<Font>(new Font());
    if (m_font->load(filename, h/32))
	{
        m_font->setAlignment(Font::LEFT);
		return true;
	}
    else m_font.reset();

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
    if (m_font != NULL)
	{
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		unsigned int w = viewport[2];
		unsigned int h = viewport[3];

		m_font->draw2D(
			m_displayText.c_str(),
			math::vec2f(m_clampedPos.x * (float) w, m_clampedPos.y * (float) h),
			m_color);
	}
}

void TextHUD::reset()
{

}

