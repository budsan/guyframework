#pragma once

#include "graphics.h"
#include "math/algebra3.h"

#include <string>

namespace Guy {

/// Texture class
class Texture
{
public:
	Texture();
	~Texture();

	bool create(int width, int height);
	bool load(std::string filename);
	bool load(const void* memFile, std::size_t size);
	bool load(const unsigned char *rgba8Raw, int width, int height);

	bool loadNullTexture();

	int getWidth()  const {return m_size.x;}
	int getHeight() const {return m_size.y;}
	const math::vec2i &getSize() const {return m_size;}

	GLuint id() const {return m_id;}

	/// Set texture filtering for both magnification and minification
	void setFiltering(GLenum filter) const;

	/// Set texture wrap for both coordinates S and T
	void setWrap(GLenum wrap) const;

	/// Bind the texture to the current unit
	       void   bind() const;
	static void unbind();

	// Assignment operator for copying instances is FORBIDDEN
	const Texture& operator=(const Texture& other);

	// We doing that for efficiency issues. If we just simply copy
	// opengl texture id throught every instance, it will be freed
	// when first instance is destroyed, invalidating all copies.
private:
	friend class TextureManager;

	GLuint m_id;
	math::vec2i m_size;
};

} // namespace Guy
