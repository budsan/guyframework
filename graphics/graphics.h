#pragma once

#if defined(GUY_USE_LINUX)
#include "deps/linux/graphics.h"
#elif defined(GUY_USE_WINDOWS)
#include "deps/win/graphics.h"
#elif defined(GUY_USE_BB10)
#include "deps/bb10/graphics.h"
#endif

#include "log.h"

#ifdef NDEBUG
#define GL_ASSERT( gl_code ) gl_code
#else
#define GL_ASSERT( gl_code ) do \
    { \
	gl_code; \
	__gl_error_code = glGetError(); \
	GUY_ASSERT(__gl_error_code == GL_NO_ERROR); \
    } while(0)
#endif

#define GL_CHECK( gl_code ) do \
    { \
	while (glGetError() != GL_NO_ERROR) ; \
	gl_code; \
	__gl_error_code = glGetError(); \
	if (__gl_error_code != GL_NO_ERROR) \
	{ \
	    GUY_ERROR(#gl_code ": %d", (int)__gl_error_code); \
	} \
    } while(0)

/** Global variable to hold GL errors
 * @script{ignore} */
extern GLenum __gl_error_code;

/**
 * Accesses the most recently set global GL error.
 */
#define GL_LAST_ERROR() __gl_error_code
