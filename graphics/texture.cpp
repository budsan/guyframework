#include "texture.h"
#include "log.h"

#include <string>
#include <assert.h>

#include "deps/stb_image/stb_image.h"

namespace Guy {

Texture::Texture() : m_id(0), m_size(0,0), m_actualSize(0,0)
{

}

Texture::~Texture()
{
	if (m_id != 0) glDeleteTextures(1, &m_id);
}

// Assignment operator for copying instances is FORBIDDEN
const Texture& Texture::operator=(const Texture& other)
{
	assert(other.m_id == 0);
	return *this;
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

int Texture::getValidSize(int size)
{
	if (GLEW_ARB_texture_non_power_of_two)
	{
		// If hardware supports NPOT textures, then just return the unmodified size
		return size;
	}
	else
	{
		// If hardware doesn't support NPOT textures, we calculate the nearest power of two
		int powerOfTwo = 1;
		while (powerOfTwo < size) powerOfTwo *= 2;
		return powerOfTwo;
	}
}

int Texture::getMaximumSize()
{
	GLint size;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size);

	return static_cast<int>(size);
}

//---------------------------------------------------------------------------//

bool Texture::create(int width, int height)
{
	// Check if texture parameters are valid before creating it
	if ((width == 0) || (height == 0))
	{
		dbgPrintLog("Failed to create texture, invalid size (%dx%d)\n", width, height);
		return false;
	}

	// Compute the internal texture dimensions depending on NPOT textures support
	math::vec2<GLint> actualSize(getValidSize(width), getValidSize(height));

	// Check the maximum texture size
	int maxSize = getMaximumSize();
	if ((actualSize.x > maxSize) || (actualSize.y > maxSize))
	{
		dbgPrintLog("Failed to create texture, its internal size is too high (%dx%d, maximum is %dx%d)\n",
			    actualSize.x, actualSize.y, maxSize, maxSize);
		return false;
	}

	// All the validity checks passed, we can store the new texture settings
	m_size.x = width;
	m_size.y = height;
	m_actualSize = actualSize;

	// Create the OpenGL texture if it doesn't exist yet
	if (!m_id)
	{
		GLuint texture;
		glGenTextures(1, &texture);
		m_id = texture;
	}

	// Initialize the texture
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_actualSize.x, m_actualSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	return true;
}

bool Texture::load(std::string filename)
{
	// Load the image and get a pointer to the pixels in memory
	int width, height, channels;
	unsigned char* ptr = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);

	if (ptr && width && height)
	{
		if (!this->create(width, height))
		{
			stbi_image_free(ptr);
			return false;
		}

		unsigned char* pixels = ptr;
		glBindTexture(GL_TEXTURE_2D, m_id);
		for (int i = 0; i < height; ++i)
		{
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, i, width, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
			pixels += 4 * width;
		}

		// Free the loaded pixels
		stbi_image_free(ptr);

		return true;
	}
	else
	{
		printLog("Failed to load image \"%s\". Reason : %s\n", filename.c_str(), stbi_failure_reason());
		return false;
	}
}

bool Texture::load(const void* data, std::size_t size)
{

}

bool Texture::loadNullTexture()
{
	m_id = 0;
	const int size = 64;
	if (!this->create(size, size)) return false;


	unsigned char *texels = new unsigned char[4*size*size];
	unsigned char *p = texels;
	for (int y = 0; y < size; y++)
	{
		unsigned char fucsia_y = y < (size/2) ? ~0:0;
		for (int x = 0; x < size; x++)
		{
			unsigned char fucsia_x = x < (size/2) ? ~0:0;
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

	glBindTexture (GL_TEXTURE_2D, m_id);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, texels);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
	delete[] texels;

	m_size.x  = size;
	m_size.y = size;

	return true;
}

} //namespace Guy
