#ifndef _TEXTURE_INCLUDED_
#define _TEXTURE_INCLUDED_

#include <stdexcept>
#include "graphics.h"

/// Texture class
class Texture
{
private:
	friend class TexturePNGLoader;
	friend class TextureQTLoader;
	friend class TextureManager;

	GLuint  id;
	GLsizei width;
	GLsizei height;
public:
	Texture();

	bool Load(std::string filename);
	bool Load(GLenum iformat, int width, int height, GLfloat *pixels);
	bool Load(GLenum iformat, int width, int height, GLubyte *pixels);
	bool Load(const Texture &other); //TODO
	bool LoadNullTexture();

	GLsizei getWidth()  const {return width; }
	GLsizei	getHeight() const {return height;}

	GLsizei w() const {return width; }
	GLsizei	h() const {return height;}

	~Texture();

	/// Set texture filtering for both magnification and minification
	void setFiltering(GLenum filter) const;

	/// Set texture wrap for both coordinates S and T
	void setWrap(GLenum wrap) const;

	/// Bind the texture to the current unit
	       void   Bind() const;
	static void Unbind();

	// El operador igualdad para hacer copias esta PROHIBIDO.
	const Texture& operator=(const Texture& other);

	// La razon de esto es por temas de eficiencia. Si simplemente
	// copiaramos los identificadores, como se crearia un objeto nuevo,
	// este al salir de su scope llamaria al destructor liberando el
	// identificador de textura que usa ambas.
	//
	// Para hacerlo bien se deberia tener un contador de referencias o crear
	// un identificador nuevo de textura y copiar la textura en el nuevo
	// identificador, cosa realmente costosa.
	// Si realmente lo que se quiere es una de estas dos ultimas soluciones
	// se puede usar un smart_ptr o usar el Load que tiene de parametros de
	// entrada otra Texture, concienciando al programador que lo que esta
	// haciendo es costoso en tiempo y en recursos de memoria.
};

#ifdef INCLUDED_FROM_QT
class TextureQTLoader
{
private:
	static bool LoadQT(const char *fname, Texture &t);
	friend class Texture;
};

#else
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
#endif

#endif
