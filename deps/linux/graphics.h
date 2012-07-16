#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>


#if defined(GUY_TEXTURE_INCLUDED)
	#if defined(GUY_USE_QT_DEPS)
		#include "deps/qt/textureqtloader.h"
	#else
		#include "deps/png/texturepngloader.h"
	#endif
#endif
