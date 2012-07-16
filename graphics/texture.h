#pragma once
#define GUY_TEXTURE_INCLUDED

#include "graphics.h"
#include <stdexcept>

namespace Guy {

/// Texture class
class Texture
{
private:
	friend class TexturePNGLoader;
	friend class TextureQTLoader;
	friend class TextureManager;

	GLuint  m_id;
	GLsizei m_width;
	GLsizei m_height;
public:
	Texture();
	~Texture();

	bool load(std::string filename);
	bool load(GLenum iformat, int width, int height, GLfloat *pixels);
	bool load(GLenum iformat, int width, int height, GLubyte *pixels);
	bool load(const Texture &other); //TODO
	bool loadNullTexture();

	GLsizei getWidth()  const {return m_width; }
	GLsizei	getHeight() const {return m_height;}

	GLsizei w() const {return m_width; }
	GLsizei	h() const {return m_height;}

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
};

} // namespace Guy
