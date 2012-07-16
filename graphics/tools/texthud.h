#pragma once

#include <string>
#include <boost/smart_ptr.hpp>

#include "graphics/font.h"
#include "graphics/color.h"
#include "math/algebra3.h"

namespace Guy {

class TextHUD
{
 public:
	TextHUD();
	bool loadFont(const char *filename);
	bool loadFont(const char *filename, unsigned int h);
	void setColor(const rgba &color);

	void update(float deltaTime);
	void draw();
	void reset();

    boost::shared_ptr<Font> &font() { return m_font;}
	std::string &displayText() {return m_displayText;}
	math::vec2f &clampedPos() {return m_clampedPos;}

protected:
    boost::shared_ptr<Font> m_font;
	rgba m_color;

	std::string m_displayText;
	math::vec2f m_clampedPos;
};

} // namespace Guy
