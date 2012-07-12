#include "texture.h"

#include <string>
#include <stdlib.h>
#include <stdexcept>

#ifdef INCLUDED_FROM_QT
#include <QString>
#include <QImage>
#include <QGLWidget>
#else
#include <png.h>
#endif

Texture::Texture() : m_id(0), m_width(0), m_height(0)
{
	//glGenTextures(1, &id);
}

Texture::~Texture()
{
	if (m_id != 0) glDeleteTextures(1, &m_id);
}


bool Texture::load(std::string filename)
{
#ifdef INCLUDED_FROM_QT
	return TextureQTLoader::LoadQT(filename.c_str(), *this);
#else
	return TexturePNGLoader::LoadPNG(filename.c_str(), *this);
#endif
}

// El operador igualdad para hacer copias esta PROHIBIDO.
const ::Texture& Texture::operator=(const Texture& other)
{
	if (other.m_id != 0)
	{
		throw std::runtime_error("TEXTURE EQUAL OPERATOR FOR COPY IS FORBIDDEN");
	}

	return *this;
}

#ifndef INCLUDED_FROM_QT
bool Texture::load(GLenum iformat, int width, int height, GLfloat *pixels)
{
	glGenTextures(1, &m_id);
	bind();

	GLenum format;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	if (iformat == GL_RGB16F_ARB || iformat == GL_RGB32F_ARB) {
		format = GL_RGB;
	}
	else if (iformat == GL_RGBA16F_ARB || iformat == GL_RGB32F_ARB) {
		format = GL_RGBA;
	}
	else if (iformat == GL_RGBA8 || iformat == GL_RGBA || iformat == 4) {
		format = GL_RGBA;
	}
	else if (iformat == GL_RGB8 || iformat == GL_RGB || iformat == 3) {
		format = GL_RGB;
	}
	else if (iformat == GL_LUMINANCE8_ALPHA8 || iformat == GL_LUMINANCE_ALPHA || iformat == 2) {
		format = GL_LUMINANCE_ALPHA;
	}
	else if (iformat == GL_LUMINANCE8 || iformat == GL_LUMINANCE || iformat == 1) {
		format = GL_LUMINANCE;
	}
	else if (iformat == GL_DEPTH_COMPONENT || iformat == GL_DEPTH_COMPONENT16_ARB ||
		 iformat == GL_DEPTH_COMPONENT24_ARB || iformat == GL_DEPTH_COMPONENT32_ARB) {
		format = GL_DEPTH_COMPONENT;
	}
	else {
		return false;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, iformat, width, height, 0, format, GL_FLOAT, pixels);
	this->m_width  = width;
	this->m_height = height;

	return true;
}
#endif


bool Texture::load(GLenum iformat, int width, int height, GLubyte *pixels)
{
	glGenTextures(1, &m_id);
	bind();

	GLenum format;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	if (iformat == GL_RGB8) {
		format = GL_RGB;
	}
	else if (iformat == GL_RGBA8) {
		format = GL_RGBA;
	}
	else {
		return false;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, iformat, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);
	return true;
}

bool Texture::load(const Texture &other)
{
	return false;
}


void Texture::setFiltering(GLenum filter) const
{
	bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
}

void Texture::setWrap(GLenum wrap) const
{
	bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
}

void Texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

//---------------------------------------------------------------------------//

bool Texture::loadNullTexture()
{
	m_id = 0;
	const unsigned int he = 64, wi = 64;
	unsigned char *texels = new unsigned char[4*he*wi];
	unsigned char *p = texels;
	for (unsigned int y = 0; y < he; y++)
	{
		unsigned char fucsia_y = y < (he/2) ? ~0:0;
		for (unsigned int x = 0; x < wi; x++)
		{
			unsigned char fucsia_x = x < (wi/2) ? ~0:0;
			if (fucsia_y^fucsia_x)
			{
				*p++ = ~0; //R
				*p++ =  0; //G
				*p++ = ~0; //B
				*p++ = ~0; //A
			}
			else
			{
				*p++ =  0; //R
				*p++ =  0; //G
				*p++ =  0; //B
				*p++ = ~0; //A
			}
		}
	}

	glGenTextures (1, &m_id);
	if (m_id == 0) return false;
	glBindTexture (GL_TEXTURE_2D, m_id);
	gluBuild2DMipmaps (GL_TEXTURE_2D, GL_RGBA, wi, he, GL_RGBA, GL_UNSIGNED_BYTE, texels);

	m_width  = wi;
	m_height = he;
	delete[] texels;
	return true;
}

//---------------------------------------------------------------------------//

#ifdef INCLUDED_FROM_QT

int NextPowerOfTwo ( int a )
{
	int rval=1;
	while(rval<a) rval<<=1;
	return rval;
}

bool TextureQTLoader::LoadQT(const char *fname, Texture &t)
{
	QString Path = QString(fname);
	QImage bitmap;
	if (!bitmap.load(Path)) return false;

	QImage img;
	bool extension_T_NOPOW2 = false;
	if (extension_T_NOPOW2)
	{
		//Si el tamany de la imatge no es potencia de dos
		int newSizeX = NextPowerOfTwo(bitmap.width()),
				newSizeY = NextPowerOfTwo(bitmap.height());
		QImage newBitmap = bitmap = bitmap.scaled(newSizeX,newSizeY,
							  Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

		img = QGLWidget::convertToGLFormat(newBitmap);
	}
	else img = QGLWidget::convertToGLFormat(bitmap);

	GLuint id;
	glGenTextures (1, &id);
	glBindTexture (GL_TEXTURE_2D, id);
	gluBuild2DMipmaps (GL_TEXTURE_2D, GL_RGBA,
			   img.width(), img.height(),
			   GL_RGBA, GL_UNSIGNED_BYTE, img.bits());

	t.m_id     = id;
	t.m_width  = img.width();
	t.m_height = img.height();

	return true;
}

#else

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

#endif
