#include "deps/sdl/sdlscreen.h"
#include "graphics/texturemanager.h"

#include "math/algebra3.h"
#include "math/bbox.h"
#include "graphics/primitives.h"

#include "graphics/graphics.h"
#include <SDL/SDL.h>

#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>

#include <algorithm>

#include "log.h"

namespace Guy {

static const unsigned int GUY_SDL_SURFACE_FLAGS =
		SDL_OPENGL |
		SDL_GL_DOUBLEBUFFER |
		SDL_HWPALETTE |
		SDL_HWSURFACE |
		SDL_HWACCEL;

static const int GUY_DEFAULT_BPP = 32;

//---------------------------------------------------------------------------//

SDLScreen::SDLScreen()
{
	m_screen = NULL;
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
	SDL_Rect **modes_available = SDL_ListModes(NULL, GUY_SDL_SURFACE_FLAGS|SDL_FULLSCREEN);
	const SDL_VideoInfo* videoinfo = SDL_GetVideoInfo();
	if (modes_available == (SDL_Rect **) 0) return NULL;
	unsigned int nModes;
	for (nModes = 0; modes_available[nModes]; nModes++) {}

	m_videoModes.reserve(nModes);
	for(nModes = 0; modes_available[nModes]; nModes++) {
		m_videoModes.push_back(Mode(modes_available[nModes]->w, modes_available[nModes]->h));
	}
	std::sort( m_videoModes.begin(), m_videoModes.end());

	return true;
}

bool SDLScreen::init()
{
	if (m_screen == NULL) {
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
	SDL_GL_SwapBuffers();
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

	if (m_screen != NULL)
	{
		if (m_selectedMode.w  == selected.w &&
		    m_selectedMode.h  == selected.h)
		{
			if (fullscreen != m_isFullscreen)
				SDL_WM_ToggleFullScreen(m_screen);

			return true;
		}
		else unloadContent();
	}

	unsigned int flags = GUY_SDL_SURFACE_FLAGS;
	if (fullscreen) flags |= SDL_FULLSCREEN;

	SDL_Surface* screen = NULL;

	//select settings video-mode if exists
	if (selected.w == 0 || selected.h == 0)
	{
		selected = m_videoModes[0];
		screen = SDL_SetVideoMode (selected.w, selected.h, GUY_DEFAULT_BPP, flags);
	}

	//select current desktop video-mode
	if (screen == NULL)
	{
		screen = SDL_SetVideoMode (selected.w, selected.h, GUY_DEFAULT_BPP, flags);
	}

	if (screen == NULL) return false;

	m_selectedMode.w = selected.w;
	m_selectedMode.h = selected.h;
	m_isFullscreen = fullscreen;

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	initGL();

	m_screen = screen;
	return true;
}

const std::vector<Screen::Mode> &SDLScreen::availableModes(unsigned int &size)
{
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

void SDLScreen::setCaption(const char* GameName)
{
	SDL_WM_SetCaption(GameName, GameName);
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
