#pragma once

#ifdef _WINDOWS
#include <windows.h>
#endif

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

#include "graphics.h"

#include <vector>
#include <string>

#include <math/vec2.h>

class Font
{
public:
	Font();
	
	enum VertAlignment { TOP , BOTTOM, BASELINE };
	enum HorzAlignment { LEFT, RIGHT , CENTER   };
	
	bool load(const char *filename, unsigned int height);
	void setAlignment(VertAlignment v, HorzAlignment h);
	void print(float x, float y, const char *fmt, ...);
	void print2D(float x, float y, const char *fmt, ...);
#ifdef _MATH_ALGEBRA3_DEFINED_
	void print(math::vec2f v, const char *fmt, ...);
	void print2D(math::vec2f v, const char *fmt, ...);
#endif

	void clean();
private:

	bool renderFaceToTexture(FT_Face face, char ch);
	void calcAlignment(std::vector<std::string> &lines, std::vector<int>& align);
	void printGL(float x, float y, char *text);
	inline int nextPowerOfTwo ( int a )
	{
		int rval=1;
		while(rval<a) rval<<=1;
		return rval;
	}

	struct glyph_metrics {
		long advance;
		long width, rows;
		long top, left;
		float texwidth;
		float texheight;
	};

	GLuint        *m_textures;
	glyph_metrics *m_metrics;
	GLuint         m_listFirst;
	unsigned short m_alignment;
	unsigned short m_height;
};
