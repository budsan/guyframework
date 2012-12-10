#pragma once

#include <EGL/egl.h>

#ifdef USING_GL11
#include <GLES/gl.h>
#include <GLES/glext.h>
#elif defined(USING_GL20)
#include <GLES2/gl2.h>
#else
#error bbutil must be compiled with either USING_GL11 or USING_GL20 flags
#endif
