#include "deps/bb10/qnxscreen.h"
#include "graphics/texturemanager.h"

#include "graphics.h"

#include "math/algebra3.h"
#include "math/bbox.h"
#include "graphics/primitives.h"

#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>

#include <algorithm>

#include "log.h"

namespace Guy {

static void qnxscreen_eglErrorPrint(const char *msg)
{
	static const char *errmsg[] = {
			"function succeeded",
			"EGL is not initialized, or could not be initialized, for the specified display",
			"cannot access a requested resource",
			"failed to allocate resources for the requested operation",
			"an unrecognized attribute or attribute value was passed in an attribute list",
			"an EGLConfig argument does not name a valid EGLConfig",
			"an EGLContext argument does not name a valid EGLContext",
			"the current surface of the calling thread is no longer valid",
			"an EGLDisplay argument does not name a valid EGLDisplay",
			"arguments are inconsistent",
			"an EGLNativePixmapType argument does not refer to a valid native pixmap",
			"an EGLNativeWindowType argument does not refer to a valid native window",
			"one or more argument values are invalid",
			"an EGLSurface argument does not name a valid surface configured for rendering",
			"a power management event has occurred",
			"unknown error code"
	};

	int message_index = eglGetError() - EGL_SUCCESS;

	if (message_index < 0 || message_index > 14)
		message_index = 15;

	printLog("%s: %s\n", msg, errmsg[message_index]);
}

//---------------------------------------------------------------------------//

QNXScreen::QNXScreen(screen_context_t screen_ctx)
: screen_ctx(screen_ctx)
{

}

//---------------------------------------------------------------------------//

QNXScreen::~QNXScreen()
{
	unloadContent();

	//Typical EGL cleanup
	if (egl_disp != EGL_NO_DISPLAY)
	{
		eglMakeCurrent(egl_disp, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (egl_surf != EGL_NO_SURFACE)
		{
			eglDestroySurface(egl_disp, egl_surf);
			egl_surf = EGL_NO_SURFACE;
		}

		if (egl_ctx != EGL_NO_CONTEXT)
		{
			eglDestroyContext(egl_disp, egl_ctx);
			egl_ctx = EGL_NO_CONTEXT;
		}

		if (screen_win != NULL)
		{
			screen_destroy_window(screen_win);
			screen_win = NULL;
		}

		eglTerminate(egl_disp);
		egl_disp = EGL_NO_DISPLAY;
	}
	eglReleaseThread();

	initialized = 0;
}

//---------------------------------------------------------------------------//

bool QNXScreen::init()
{
	int usage;
	int format = SCREEN_FORMAT_RGBX8888;
	EGLint interval = 1;
	int rc, num_configs;

	EGLint attrib_list[]= { EGL_RED_SIZE,        8,
			EGL_GREEN_SIZE,      8,
			EGL_BLUE_SIZE,       8,
			EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
			EGL_RENDERABLE_TYPE, 0,
			EGL_NONE};

#ifdef USING_GL11
	usage = SCREEN_USAGE_OPENGL_ES1 | SCREEN_USAGE_ROTATION;
	attrib_list[9] = EGL_OPENGL_ES_BIT;
#elif defined(USING_GL20)
	usage = SCREEN_USAGE_OPENGL_ES2 | SCREEN_USAGE_ROTATION;
	attrib_list[9] = EGL_OPENGL_ES2_BIT;
	EGLint attributes[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
#else
	#error "program should be compiled with either USING_GL11 or USING_GL20 -D flags\n"
	return false;
#endif

	//Simple egl initialization
	egl_disp = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (egl_disp == EGL_NO_DISPLAY)
	{
		qnxscreen_eglErrorPrint("eglGetDisplay");
		return false;
	}

	rc = eglInitialize(egl_disp, NULL, NULL);
	if (rc != EGL_TRUE)
	{
		qnxscreen_eglErrorPrint("eglInitialize");
		return false;
	}

	rc = eglBindAPI(EGL_OPENGL_ES_API);

	if (rc != EGL_TRUE)
	{
		qnxscreen_eglErrorPrint("eglBindApi");
		return false;
	}

	if(!eglChooseConfig(egl_disp, attrib_list, &egl_conf, 1, &num_configs))
	{
		return false;
	}

#ifdef USING_GL20
	egl_ctx = eglCreateContext(egl_disp, egl_conf, EGL_NO_CONTEXT, attributes);
#elif defined(USING_GL11)
	egl_ctx = eglCreateContext(egl_disp, egl_conf, EGL_NO_CONTEXT, NULL);
#endif

	if (egl_ctx == EGL_NO_CONTEXT)
	{
		qnxscreen_eglErrorPrint("eglCreateContext");
		return false;
	}

	rc = screen_create_window(&screen_win, screen_ctx);
	if (rc)
	{
		printLog("screen_create_window\n");
		return false;
	}

	rc = screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_FORMAT, &format);
	if (rc)
	{
		printLog("screen_set_window_property_iv(SCREEN_PROPERTY_FORMAT)\n");
		return false;
	}

	rc = screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_USAGE, &usage);
	if (rc)
	{
		printLog("screen_set_window_property_iv(SCREEN_PROPERTY_USAGE)\n");
		return false;
	}

	rc = screen_get_window_property_pv(screen_win, SCREEN_PROPERTY_DISPLAY, (void **)&screen_disp);
	if (rc)
	{
		printLog("screen_get_window_property_pv\n");
		return false;
	}

	int screen_resolution[2];

	rc = screen_get_display_property_iv(screen_disp, SCREEN_PROPERTY_SIZE, screen_resolution);
	if (rc)
	{
		printLog("screen_get_display_property_iv\n");
		return false;
	}

	int angle = atoi(getenv("ORIENTATION"));

	screen_display_mode_t screen_mode;
	rc = screen_get_display_property_pv(screen_disp, SCREEN_PROPERTY_MODE, (void**)&screen_mode);
	if (rc)
	{
		printLog("screen_get_display_property_pv\n");
		return false;
	}

	int size[2];
	rc = screen_get_window_property_iv(screen_win, SCREEN_PROPERTY_BUFFER_SIZE, size);
	if (rc)
	{
		printLog("screen_get_window_property_iv\n");
		return false;
	}

	int buffer_size[2] = {size[0], size[1]};

	if ((angle == 0) || (angle == 180))
	{
		if (((screen_mode.width > screen_mode.height) && (size[0] < size[1])) ||
		    ((screen_mode.width < screen_mode.height) && (size[0] > size[1])))
		{
			buffer_size[1] = size[0];
			buffer_size[0] = size[1];
		}
	}
	else if ((angle == 90) || (angle == 270))
	{
		if (((screen_mode.width > screen_mode.height) && (size[0] > size[1])) ||
		    ((screen_mode.width < screen_mode.height && size[0] < size[1])))
		{
			buffer_size[1] = size[0];
			buffer_size[0] = size[1];
		}
	}
	else
	{
		printLog("Navigator returned an unexpected orientation angle.\n");

		return false;
	}

	rc = screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_BUFFER_SIZE, buffer_size);
	if (rc)
	{
		printLog("screen_set_window_property_iv");
		return false;
	}

	rc = screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_ROTATION, &angle);
	if (rc)
	{
		printLog("screen_set_window_property_iv");
		return false;
	}

	rc = screen_create_window_buffers(screen_win, nbuffers);
	if (rc)
	{
		printLog("screen_create_window_buffers");
		return false;
	}

	egl_surf = eglCreateWindowSurface(egl_disp, egl_conf, screen_win, NULL);
	if (egl_surf == EGL_NO_SURFACE)
	{
		qnxscreen_eglErrorPrint("eglCreateWindowSurface");
		return false;
	}

	rc = eglMakeCurrent(egl_disp, egl_surf, egl_surf, egl_ctx);
	if (rc != EGL_TRUE)
	{
		qnxscreen_eglErrorPrint("eglMakeCurrent");
		return false;
	}

	rc = eglSwapInterval(egl_disp, interval);
	if (rc != EGL_TRUE)
	{
		qnxscreen_eglErrorPrint("eglSwapInterval");
		return false;
	}

	return true;
}

//---------------------------------------------------------------------------//

void QNXScreen::flip()
{
	int rc = eglSwapBuffers(egl_disp, egl_surf);
	if (rc != EGL_TRUE) {
		//ERROR
	}
}

//---------------------------------------------------------------------------//

void QNXScreen::fillWithColor(const rgba &color)
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(math::mat4f::fromOrtho( -1, 1, -1, 1, -1, 1).v);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(math::mat4f::fromIdentity().v);

	glColor(color);
	Guy::draw(math::bbox2f(math::vec2f(-1,-1),math::vec2f( 1, 1)));
}

//---------------------------------------------------------------------------//

bool QNXScreen::setMode(const Mode& mode, bool fullscreen)
{
	return false;
}

const std::vector<Screen::Mode> &QNXScreen::getAvailableModes(unsigned int &size)
{
	return m_videoModes;
}
//---------------------------------------------------------------------------//

const Screen::Mode& QNXScreen::getCurrentMode()
{
	return m_selectedMode;
}

//---------------------------------------------------------------------------//
void QNXScreen::unloadContent()
{
	TextureManager::instance().unloadTextures();
}

//---------------------------------------------------------------------------//

void QNXScreen::resetViewport()
{
	//	if ( m_ratio == 0 )
	//	{
	glViewport(0,0,m_selectedMode.w,m_selectedMode.h);
	//glDisable(GL_SCISSOR_TEST);
	return;
	//	}

	/*
	if (m_selectedMode.h == 0.0f) return;
	float screen_ratio = float(m_selectedMode.w)/float(m_selectedMode.h);

	if ( screen_ratio > m_ratio)
	{
		int weight = (int) ((m_ratio/screen_ratio) * (float) m_selectedMode.w);
		int x = (m_selectedMode.w - weight)/2;
		glViewport(x,0,weight,m_selectedMode.h);
		//glScissor (x,0,weight,m_selectedMode.h);
		//glEnable(GL_SCISSOR_TEST);
	}
	else if ( screen_ratio < m_ratio)
	{
		int height = (int) ((screen_ratio/m_ratio) * (float) m_selectedMode.h);
		int y = (m_selectedMode.h - height)/2;
		glViewport(0,y,m_selectedMode.w,height);
		//glScissor (0,y,m_selectedMode.w,height);
		//glEnable(GL_SCISSOR_TEST);
	}
	else
	{
		glViewport(0,0,m_selectedMode.w,m_selectedMode.h);
		//glDisable(GL_SCISSOR_TEST);
	}
	 */
}

} //namespace Guy
