#pragma once
#define GUY_TEXTUREPNGLOADER_INCLUDED

#include "graphics/graphics.h"

#include <png.h>

namespace Guy {

class Texture;
class TexturePNGLoader
{
private:
	struct gl_texture_t
	{
		GLsizei width;
		GLsizei height;

		GLenum format;
		GLint internalFormat;
		GLuint id;

		GLubyte *texels;

	};

	static void getPNGTextureInfo (int color_type, gl_texture_t *texinfo);
	static gl_texture_t *ReadPNGFromFile (const char *fname);
	static bool LoadPNG(const char *fname, Texture &t);

	friend class Texture;
};

} // namespace Guy
