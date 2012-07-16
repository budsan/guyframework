#pragma once
#define GUY_TEXTUREQTLOADER_INCLUDED

#include <QString>
#include <QImage>
#include <QGLWidget>

namespace Guy {

class TextureQTLoader
{
private:
	static bool LoadQT(const char *fname, Texture &t);
	friend class Texture;
};

} // namespace Guy
