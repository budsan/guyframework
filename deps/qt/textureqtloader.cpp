#include "textureqtloader.h"
#include "graphics/texture.h"

namespace Guy {

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

} // namespace Guy {
