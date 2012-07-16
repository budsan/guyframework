#include "texturepngloader.h"

#include "graphics/texture.h"
#include <stdlib.h>

namespace Guy {

void TexturePNGLoader::getPNGTextureInfo (int color_type, gl_texture_t *texinfo)
{
	switch (color_type)
	{
	case PNG_COLOR_TYPE_GRAY:
		texinfo->format = GL_LUMINANCE;
		texinfo->internalFormat = 1;
		break;

	case PNG_COLOR_TYPE_GRAY_ALPHA:
		texinfo->format = GL_LUMINANCE_ALPHA;
		texinfo->internalFormat = 2;
		break;

	case PNG_COLOR_TYPE_RGB:
		texinfo->format = GL_RGB;
		texinfo->internalFormat = 3;
		break;

	case PNG_COLOR_TYPE_RGB_ALPHA:
		texinfo->format = GL_RGBA;
		texinfo->internalFormat = 4;
		break;

	default:
		/* Badness */
		break;
	}
}

//---------------------------------------------------------------------------//

//BUGFIX for libpng 1.4 Thanks to Albert Vaca
#if PNG_LIBPNG_VER_MINOR >= 4
#define png_set_gray_1_2_4_to_8 png_set_expand_gray_1_2_4_to_8
#endif

TexturePNGLoader::gl_texture_t *TexturePNGLoader::ReadPNGFromFile (const char *fname)
{
	png_bytep *row_pointers = NULL;
	int i;

	/* open image file */
	FILE *fp = fopen (fname, "rb");
	if (!fp)
	{
		return NULL;
	}

	/* read magic number */
	png_byte magic[8];
	fread (magic, 1, sizeof (magic), fp);

	/* check for valid magic number */
	if (!png_check_sig (magic, sizeof(magic)))
	{
		fclose (fp);
		return NULL;
	}

	/* create a png read struct */
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
		fclose (fp);
		return NULL;
	}

	/* create a png info struct */
	png_infop info_ptr = png_create_info_struct (png_ptr);
	if (!info_ptr)
	{
		fclose (fp);
		png_destroy_read_struct (&png_ptr, NULL, NULL);
		return NULL;
	}

	/* create our OpenGL texture object */
	gl_texture_t *texinfo = (gl_texture_t *)malloc (sizeof (gl_texture_t));

	/* initialize the setjmp for returning properly after a libpng error occured */
	if (setjmp (png_jmpbuf (png_ptr)))
	{
		fclose (fp);
		png_destroy_read_struct (&png_ptr, &info_ptr, NULL);

		if(row_pointers)
			free(row_pointers);

		if (texinfo)
		{
			if (texinfo->texels) free(texinfo->texels);
			free (texinfo);
		}
		return NULL;
	}

	/* setup libpng for using standard C fread() function with our FILE pointer */
	png_init_io (png_ptr, fp);

	/* tell libpng that we have already read the magic number */
	png_set_sig_bytes (png_ptr, sizeof (magic));

	/* read png info */
	png_read_info (png_ptr, info_ptr);

	/* get some usefull information from header */
	int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	int color_type = png_get_color_type(png_ptr, info_ptr);

	/* convert index color images to RGB images */
	if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb (png_ptr);

	/* convert 1-2-4 bits grayscale images to 8 bits grayscale. */
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_gray_1_2_4_to_8 (png_ptr);

	if (png_get_valid (png_ptr, info_ptr, PNG_INFO_tRNS)) png_set_tRNS_to_alpha (png_ptr);

	if (bit_depth == 16) png_set_strip_16 (png_ptr);
	else if (bit_depth < 8) png_set_packing (png_ptr);

	/* update info structure to apply transformations */
	png_read_update_info (png_ptr, info_ptr);

	/* retrieve updated information */
	png_get_IHDR (png_ptr, info_ptr, (png_uint_32*)(&texinfo->width), (png_uint_32*)(&texinfo->height), &bit_depth, &color_type, NULL, NULL, NULL);

	/* get image format and components per pixel */
	getPNGTextureInfo (color_type, texinfo);

	/* we can now allocate memory for storing pixel data */
	texinfo->texels = (GLubyte *)malloc (sizeof (GLubyte) * texinfo->width * texinfo->height * texinfo->internalFormat);

	/* setup a pointer array.  Each one points at the begening of a row. */
	row_pointers = (png_bytep *)malloc (sizeof (png_bytep) * texinfo->height);

	for (i = 0; i < texinfo->height; ++i)
	{
		row_pointers[i] = (png_bytep)(texinfo->texels + ((texinfo->height - (i + 1)) * texinfo->width * texinfo->internalFormat));
	}

	/* read pixel data using row pointers */
	png_read_image (png_ptr, row_pointers);

	/* finish decompression and release memory */
	png_read_end (png_ptr, NULL);
	png_destroy_read_struct (&png_ptr, &info_ptr, NULL);

	/* we don't need row pointers anymore */
	free(row_pointers);

	fclose (fp);
	return texinfo;
}

//---------------------------------------------------------------------------//

bool TexturePNGLoader::LoadPNG(const char *fname, Texture &t)
{
	gl_texture_t *png_tex = NULL;
	t.m_id = 0;

	png_tex = ReadPNGFromFile(fname);

	//GLsizei width = png_tex->width;
	//GLsizei height = png_tex->height;

	if (png_tex && png_tex->texels)
	{
		/* generate texture */
		glGenTextures (1, &png_tex->id);
		glBindTexture (GL_TEXTURE_2D, png_tex->id);

		/* setup some parameters for texture filters and mipmapping */
		//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

		gluBuild2DMipmaps (GL_TEXTURE_2D, png_tex->internalFormat,
				   png_tex->width, png_tex->height,
				   png_tex->format, GL_UNSIGNED_BYTE, png_tex->texels);

		t.m_id     = png_tex->id;
		t.m_width  = png_tex->width;
		t.m_height = png_tex->height;

		/* OpenGL has its own copy of texture data */
		free (png_tex->texels);
		free (png_tex);

		return true;
	}

	return false;
}

} // namespace Guy
