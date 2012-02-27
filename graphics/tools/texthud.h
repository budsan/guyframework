#pragma once

#include <string>

#include "graphics/font.h"
#include "graphics/color.h"
#include "math/vec2.h"

class TextHUD
{
 public:
	TextHUD();
	bool loadFont(const char *filename);
	void setColor(const rgba &color);

	void update(float deltaTime);
	void draw();
	void reset();

	Font &font() { return m_font;}
	std::string &displayText() {return m_displayText;}
	math::vec2f &clampedPos() {return m_clampedPos;}

protected:
	Font m_font;
	rgba m_color;

	std::string m_displayText;
	math::vec2f m_clampedPos;
	bool m_isFontLoaded;
};
