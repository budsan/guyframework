#pragma once

#include "color.h"
#include "math/algebra3.h"

#include <ft2build.h>
#include FT_FREETYPE_H

class Font
{
public:

    Font() : m_alignment(LEFT), initialized(0) {}

	bool load(const char* path, int point_size, int dpi = 96);
	math::vec2f measure(const char* msg);

	enum Alignment { LEFT, RIGHT, CENTER };
	void setAlignment(Alignment h) { m_alignment = h; }

	void draw2D(const char* msg, float x, float y, float r = 1, float g = 1, float b = 1, float a = 1);
	void draw(const char* msg, float x, float y, float r = 1, float g = 1, float b = 1, float a = 1);


	void draw2D(const char* msg, const math::vec2f &pos, const rgba &c) {
		draw2D(msg, pos.x, pos.y, c.r, c.g, c.b, c.a);
	}

	void draw2D(const char* msg, float x, float y, const rgba &c) {
		draw2D(msg, x, y, c.r, c.g, c.b, c.a);
	}

	void draw2D(const char* msg, const math::vec2f &pos, float r = 1, float g = 1, float b = 1, float a = 1) {
		draw2D(msg, pos.x, pos.y, r, g, b, a);
	}

	void draw(const char* msg, const math::vec2f &pos, const rgba &c) {
		draw(msg, pos.x, pos.y, c.r, c.g, c.b, c.a);
	}

	void draw(const char* msg, float x, float y, const rgba &c) {
		draw(msg, x, y, c.r, c.g, c.b, c.a);
	}

	void draw(const char* msg, const math::vec2f &pos, float r = 1, float g = 1, float b = 1, float a = 1) {
		draw(msg, pos.x, pos.y, r, g, b, a);
	}

private:

	inline int nextPowerOfTwo ( int a )
	{
		int rval=1;
		while(rval<a) rval<<=1;
		return rval;
	}

	unsigned short m_alignment;
	unsigned int font_texture;
	float pt;
	float advance[128];
	float width[128];
	float height[128];
	float tex_x1[128];
	float tex_x2[128];
	float tex_y1[128];
	float tex_y2[128];
	float offset_x[128];
	float offset_y[128];
	int initialized;
};
