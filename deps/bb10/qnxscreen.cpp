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

//---------------------------------------------------------------------------//

QNXScreen::QNXScreen()
{

}

//---------------------------------------------------------------------------//

QNXScreen::~QNXScreen()
{
	unloadContent();

	if (egl_disp != EGL_NO_DISPLAY) {
		eglMakeCurrent(egl_disp, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (egl_surf != EGL_NO_SURFACE) {
			eglDestroySurface(egl_disp, egl_surf);
			egl_surf = EGL_NO_SURFACE;
		}
		if (egl_ctx != EGL_NO_CONTEXT) {
			eglDestroyContext(egl_disp, egl_ctx);
			egl_ctx = EGL_NO_CONTEXT;
		}
		if (screen_win != NULL) {
			screen_destroy_window(screen_win);
			screen_win = NULL;
		}
		eglTerminate(egl_disp);
		egl_disp = EGL_NO_DISPLAY;
	}
	eglReleaseThread();
}

//---------------------------------------------------------------------------//

bool QNXScreen::preinit()
{
	if (egl_disp != EGL_NO_DISPLAY) {
		eglMakeCurrent(egl_disp, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (egl_surf != EGL_NO_SURFACE) {
			eglDestroySurface(egl_disp, egl_surf);
			egl_surf = EGL_NO_SURFACE;
		}
		if (egl_ctx != EGL_NO_CONTEXT) {
			eglDestroyContext(egl_disp, egl_ctx);
			egl_ctx = EGL_NO_CONTEXT;
		}
		if (screen_win != NULL) {
			screen_destroy_window(screen_win);
			screen_win = NULL;
		}
		eglTerminate(egl_disp);
		egl_disp = EGL_NO_DISPLAY;
	}
	eglReleaseThread();

	return true;
}

bool QNXScreen::init()
{
	screen_create_context(&screen_ctx, 0);

	if (BPS_SUCCESS != screen_request_events(screen_ctx))
	{
		fprintf(stderr, "screen_request_events failed\n");
		screen_destroy_context(screen_ctx);
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
