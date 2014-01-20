#include "texture.h"
#include "debug.h"

#include <string>

#include "deps/stb_image/stb_image.h"

namespace Guy {

Texture::Texture() : m_id(0), m_size(0,0)
{

}

Texture::~Texture()
{
	if (m_id != 0) GL_ASSERT(glDeleteTextures(1, &m_id));
}

// Assignment operator for copying instances is FORBIDDEN
const Texture& Texture::operator=(const Texture& other)
{
	GUY_ASSERT(other.m_id == 0);
	return *this;
}

void Texture::setFiltering(GLenum filter) const
{
	bind();
	GL_ASSERT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter));
	GL_ASSERT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter));
}

void Texture::setWrap(GLenum wrap) const
{
	bind();
	GL_ASSERT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap));
	GL_ASSERT(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap));
}

void Texture::bind() const
{
	GL_ASSERT(glBindTexture(GL_TEXTURE_2D, m_id));
}

void Texture::unbind()
{
	GL_ASSERT(glBindTexture(GL_TEXTURE_2D, 0));
}

//---------------------------------------------------------------------------//

bool Texture::create(int width, int height)
{
	// Check if texture parameters are valid before creating it
	if ((width == 0) || (height == 0))
	{
		GUY_ERROR("Failed to create texture, invalid size (%dx%d)\n", width, height);
		return false;
	}

	// All the validity checks passed, we can store the new texture settings
	m_size.x = width;
	m_size.y = height;

	// Create the OpenGL texture if it doesn't exist yet
	if (!m_id)
	{
		GLuint texture;
		GL_ASSERT(glGenTextures(1, &texture));
		m_id = texture;
	}

	// Initialize the texture
	GL_ASSERT(glBindTexture(GL_TEXTURE_2D, m_id));
	GL_ASSERT(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));

	return true;
}

bool Texture::load(std::string filename)
{
	int width, height, channels;
	unsigned char* ptr = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);

	bool result = false;
	if (ptr && width && height) result = load(ptr, width, height);
	else printLog("Failed to load image \"%s\". Reason : %s\n", filename.c_str(), stbi_failure_reason());
	stbi_image_free(ptr);

	return result;
}

bool Texture::load(const void* memFile, std::size_t size)
{
	int width, height, channels;
	const unsigned char* buffer = static_cast<const unsigned char*>(memFile);
	unsigned char* ptr = stbi_load_from_memory(buffer, static_cast<int>(size), &width, &height, &channels, STBI_rgb_alpha);

	bool result = false;
	if (ptr && width && height) result = load(ptr, width, height);
	else printLog("Failed to load image from memory. Reason : %s\n", stbi_failure_reason());
	stbi_image_free(ptr);

	return result;
}

bool Texture::load(const unsigned char* rgba8Raw, int width, int height)
{
	if (!this->create(width, height)) return false;

	const unsigned char* pixels = rgba8Raw;
	GL_ASSERT(glBindTexture(GL_TEXTURE_2D, m_id));
	int line = height;
	while(line--) {
		GL_ASSERT(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, line, width, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixels));
		pixels += 4 * width;
	}

	return true;
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

	GL_ASSERT(glBindTexture (GL_TEXTURE_2D, m_id));
	GL_ASSERT(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, texels));

	delete[] texels;

	m_size.x  = size;
	m_size.y = size;

	return true;
}

} //namespace Guy
