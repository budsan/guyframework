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
	
	bool Load(const char *filename, unsigned int height);
	void setAlignment(VertAlignment v, HorzAlignment h);
	void Print(float x, float y, const char *fmt, ...);
	void Print2D(float x, float y, const char *fmt, ...);
#ifdef _MATH_VEC2_DEFINED_
	void Print(math::vec2f v, const char *fmt, ...);
	void Print2D(math::vec2f v, const char *fmt, ...);
#endif

	void Clean();
private:

	bool RenderFaceToTexture(FT_Face face, char ch);
	void CalcAlignment(std::vector<std::string> &lines, std::vector<int>& align);
	void PrintGL(float x, float y, char *text);
	inline int NextPowerOfTwo ( int a )
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

	GLuint        *myTextures;
	glyph_metrics *myMetrics;
	GLuint         myListFirst;
	unsigned short myAlignment;
	unsigned short myHeight;
};
