#include "texture.h"

#include <string>
#include <stdexcept>

namespace Guy {

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
	bool success;

#ifdef GUY_TEXTUREQTLOADER_INCLUDED
	success = TextureQTLoader::LoadQT(filename.c_str(), *this);
	if (success) return true;
#endif

#ifdef GUY_TEXTUREPNGLOADER_INCLUDED
	success =  TexturePNGLoader::LoadPNG(filename.c_str(), *this);
	if (success) return true;
#endif

	return false;
}

// Assignment operator for copying instances is FORBIDDEN
const Texture& Texture::operator=(const Texture& other)
{
	if (other.m_id != 0)
	{
		throw std::runtime_error("TEXTURE ASSIGNMENT OPERATOR FOR COPYING IS FORBIDDEN");
	}

	return *this;
}


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

} //namespace Guy
