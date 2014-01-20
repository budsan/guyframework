#include "deps/sdl/sdlscreen.h"
#include "graphics/texturemanager.h"

#include "math/algebra3.h"
#include "math/bbox.h"
#include "graphics/primitives.h"

#include "graphics/graphics.h"
#include <SDL2/SDL.h>

#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>

#include <algorithm>

#include "debug.h"

namespace Guy {

static const int GUY_DEFAULT_BPP = 32;

thread_local SDL_GLContext s_glcontext = nullptr;

//---------------------------------------------------------------------------//

SDLScreen::SDLScreen() : m_window(nullptr), m_windowName(nullptr)
{

}

//---------------------------------------------------------------------------//

SDLScreen::~SDLScreen()
{
	if (SDL_WasInit(SDL_INIT_VIDEO)) {
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
	}

	if (!SDL_WasInit(SDL_INIT_EVERYTHING)) {
		SDL_Quit();
	}

	unloadContent();
}

//---------------------------------------------------------------------------//

bool SDLScreen::preinit()
{
	if (!SDL_WasInit(SDL_INIT_VIDEO)) {
		if (SDL_InitSubSystem(SDL_INIT_VIDEO)) {
			return false;
		}
	}

	//GET AVAILABLE VIDEO MODES
	SDL_DisplayMode mode;
	int modes = SDL_GetNumDisplayModes(0);
	for (unsigned int i = 0; i < modes; ++i) {
		if (SDL_GetDisplayMode(0, i, &mode) == 0) {
			m_videoModes.push_back(Mode(mode.w, mode.h));
		}
	}
	std::sort( m_videoModes.begin(), m_videoModes.end());

	return true;
}

bool SDLScreen::init()
{
	if (m_window == NULL) {
		if(!setMode(Mode(), true)) return false;
	}

	SDL_ShowCursor(0);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		printLog("Error: %s\n", glewGetErrorString(err));
		return false;
	}

	return true;
}

//---------------------------------------------------------------------------//

void SDLScreen::flip()
{
	if (m_window != NULL) {
		SDL_GL_SwapWindow(m_window);
	}
}

//---------------------------------------------------------------------------//

void SDLScreen::fillWithColor(const rgba &color)
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

bool SDLScreen::setMode(const Mode& mode, bool fullscreen)
{
	Mode selected = mode;

	if (m_window != NULL)
	{
		if (m_selectedMode.w  == selected.w &&
		    m_selectedMode.h  == selected.h)
		{
			if (fullscreen != m_isFullscreen) {
				SDL_SetWindowFullscreen(m_window, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | (m_isFullscreen ? SDL_WINDOW_FULLSCREEN : 0));
			}
			return true;
		}
		else
		{
			SDL_DestroyWindow(m_window);
			m_window = NULL;
		}
	}

	SDL_DisplayMode sdl_mode;
	if (mode.w == 0 || mode.h == 0) {
		int res = SDL_GetCurrentDisplayMode(0, &sdl_mode);
		if (res != 0 ) return false;
	}
	else {
		SDL_DisplayMode closest;
		closest.w = mode.w;
		closest.h = mode.h;
		closest.format = SDL_PIXELFORMAT_RGBA8888;
		closest.refresh_rate = 0;
		closest.driverdata = NULL;

		SDL_DisplayMode *res = SDL_GetClosestDisplayMode(0, &closest, &sdl_mode);
		if (res == NULL) return false;
	}

	m_window = SDL_CreateWindow(
		m_windowName == nullptr ? "Game" : m_windowName,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		sdl_mode.w,
		sdl_mode.h,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | (fullscreen ? SDL_WINDOW_FULLSCREEN : 0)
		);

	if( m_window == NULL )
	{
		return false;
	}

	if (s_glcontext == nullptr)
	{
		s_glcontext = SDL_GL_CreateContext(m_window);
	}

	m_selectedMode.w = sdl_mode.w;
	m_selectedMode.h = sdl_mode.h;
	m_isFullscreen = fullscreen;

	initGL();

	return true;
}

const std::vector<Screen::Mode> &SDLScreen::availableModes(unsigned int &size)
{
	if (m_videoModes.empty())
	return m_videoModes;
}
//---------------------------------------------------------------------------//

const Screen::Mode& SDLScreen::currentMode()
{
	return m_selectedMode;
}

//---------------------------------------------------------------------------//
void SDLScreen::unloadContent()
{
	//Añade los unloads de todos los gestores aqui
	TextureManager::instance().unloadTextures();
}

//---------------------------------------------------------------------------//

void SDLScreen::setCaption(const char* name)
{
	m_windowName = name;

	if (m_window != NULL) {
		SDL_SetWindowTitle(m_window, m_windowName);
	}
}

//---------------------------------------------------------------------------//

void SDLScreen::initGL()
{
	glShadeModel(GL_SMOOTH);
	glClearDepth( 1.0f );

	//glDisable(GL_TEXTURE_2D);
	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_BLEND);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	glLineWidth(1.0);
	resetViewport();
}

//---------------------------------------------------------------------------//

void SDLScreen::resetViewport()
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

}
