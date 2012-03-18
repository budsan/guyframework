#include "screen.h"
#include "texturemanager.h"

#ifdef _WINDOWS
#include <windows.h>
#endif

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <png.h>

#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "log.h"
#include "settings.h"


//---------------------------------------------------------------------------//

Screen* Screen::m_instance = 0;
bool Screen::m_isInstanced = false;

Screen& Screen::instance()
{
	return *ptrInstance();
}

Screen* Screen::ptrInstance()
{
	if (Screen::m_instance == 0)
	{
		m_instance = new Screen();
	}
	
	return m_instance;
}

//---------------------------------------------------------------------------//

Screen:: Screen()
{
	m_ratio = 0;
	m_videoModesSize = 0;
	m_videoModes = NULL;
	m_screen = NULL;

	if (!m_isInstanced) {
		atexit(Screen::deleteInstance);
		m_isInstanced = true;
	}
}

//---------------------------------------------------------------------------//

Screen::~Screen()
{
	m_instance = NULL;

	if (SDL_WasInit(SDL_INIT_VIDEO)) {
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
	}

	if (!SDL_WasInit(SDL_INIT_EVERYTHING)) {
		SDL_Quit();
	}

	unloadContent();
}

//---------------------------------------------------------------------------//

bool Screen::init()
{
	if (!SDL_WasInit(SDL_INIT_VIDEO)) {
		if (SDL_InitSubSystem(SDL_INIT_VIDEO)) {
			return false;
		}
	}

	if(!setVideoMode()) return false;
	SDL_ShowCursor(0);
//	const char *titol = title.c_str();
//	SDL_WM_SetCaption(titol, NULL);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		LOG << "Error: " << glewGetErrorString(err) << std::endl;
		return false;
	}

	return true;
}

//---------------------------------------------------------------------------//

void Screen::flip()
{
	SDL_GL_SwapBuffers();
}

//---------------------------------------------------------------------------//

void Screen::fillWithColor(const rgba &color)
{
	glPushAttrib(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT|GL_ENABLE_BIT);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho( -1, 1, -1, 1, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBegin(GL_QUADS);
	 glColor(color);
	 glVertex3f(-1,  1, 0);
	 glVertex3f( 1,  1, 0);
	 glVertex3f( 1, -1, 0);
	 glVertex3f(-1, -1, 0);
	glEnd();

	glPopAttrib();
}

//---------------------------------------------------------------------------//

const Screen::vmode* Screen::getVideoModeList(unsigned int &size)
{
	size = m_videoModesSize;
	if(!size) return NULL;
	
	return m_videoModes;
}
//---------------------------------------------------------------------------//

const Screen::vmode* Screen::getCurrentVideoMode()
{
	return &m_selectedMode;
}

//---------------------------------------------------------------------------//

float Screen::getRatio()
{
	if (m_ratio != 0.0f) return m_ratio;
	if (m_selectedMode.h == 0.0f) return 0.0f;
	return float(m_selectedMode.w)/float(m_selectedMode.h);
}

//---------------------------------------------------------------------------//

void Screen::setRatio(float ratio)
{
	if (ratio == m_ratio) return;
	m_ratio = ratio;
	resetViewport();
}

//---------------------------------------------------------------------------//

bool Screen::setVideoMode()
{
	Settings *config = Settings::pInstance();

	unsigned int ScreenWidth  = (unsigned int) config->get("ScreenWidth")->toInt();
	unsigned int ScreenHeight = (unsigned int) config->get("ScreenHeight")->toInt();
	unsigned int ScreenBpp    = (unsigned int) config->get("ScreenBpp")->toInt();
	bool Fullscreen  = config->get("Fullscreen")->toBool();

	if (m_screen != NULL)
	{
		if (m_selectedMode.w  == ScreenWidth &&
		    m_selectedMode.h  == ScreenHeight)
		{
			if (Fullscreen != m_isFullscreen)
				SDL_WM_ToggleFullScreen(m_screen);

			return true;
		}
		else unloadContent();
	}

	m_selectedMode.w   = ScreenWidth;
	m_selectedMode.h   = ScreenHeight;
	m_selectedMode.bpp = ScreenBpp;

	unsigned int flags =
		SDL_OPENGL |
		SDL_GL_DOUBLEBUFFER |
		SDL_HWPALETTE |
		SDL_HWSURFACE |
		SDL_HWACCEL;

	if (Fullscreen) flags |= SDL_FULLSCREEN;

	SDL_Surface* screen = NULL;

	//GET AVAILABLE VIDEO MODES
	SDL_Rect **modes_available = SDL_ListModes(NULL, flags|SDL_FULLSCREEN);
	const SDL_VideoInfo* videoinfo = SDL_GetVideoInfo();
	if (modes_available == (SDL_Rect **) 0) return NULL;
	unsigned int nModes;
	for (nModes = 0; modes_available[nModes]; nModes++) {}
	
	m_videoModesSize = nModes;
	m_videoModes = new vmode[nModes];
	for(nModes = 0; modes_available[nModes]; nModes++)
	{
		m_videoModes[nModes].w = modes_available[nModes]->w;
		m_videoModes[nModes].h = modes_available[nModes]->h;
		m_videoModes[nModes].bpp = ScreenBpp;
	}
	qsort( m_videoModes, nModes, sizeof(vmode), compareModes);
	
	//select settings video-mode if exists
	if (m_selectedMode.w != 0 || m_selectedMode.h != 0)
	{
		screen = SDL_SetVideoMode (
			m_selectedMode.w,
			m_selectedMode.h,
			m_selectedMode.bpp,
			flags);
	}

	//select current desktop video-mode
	if (screen == NULL)
	{
		m_selectedMode.w = videoinfo->current_w;
		m_selectedMode.h = videoinfo->current_h;
		screen = SDL_SetVideoMode (
			m_selectedMode.w,
			m_selectedMode.h,
			m_selectedMode.bpp,
			flags);
	}

	//select one of compatible video-mode
	for(unsigned int i = 0; i < nModes && screen == NULL; i++)
	{
		m_selectedMode.w = m_videoModes[i].w;
		m_selectedMode.h = m_videoModes[i].h;
		
		screen = SDL_SetVideoMode (
			m_selectedMode.w,
			m_selectedMode.h,
			m_selectedMode.bpp,
			flags);
	}

	if (screen == NULL) return false;

	ScreenWidth  = m_selectedMode.w;
	ScreenHeight = m_selectedMode.h;
	ScreenBpp    = m_selectedMode.bpp;
	m_isFullscreen = Fullscreen;

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	initGL();

	m_screen = screen;


	return true;
}

//---------------------------------------------------------------------------//
void Screen::unloadContent()
{
	//Añade los unloads de todos los gestores aqui
	TextureManager::instance().unloadTextures();
}

//---------------------------------------------------------------------------//

void Screen::setCaption(const char* GameName)
{
	SDL_WM_SetCaption(GameName, GameName);
}

//---------------------------------------------------------------------------//

void Screen::initGL()
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

void Screen::resetViewport()
{
	if ( m_ratio == 0 )
	{
		glViewport(0,0,m_selectedMode.w,m_selectedMode.h);
		//glDisable(GL_SCISSOR_TEST);
		return;
	}
	
	if (m_selectedMode.h == 0.0f) return;
	float screen_ratio = float(m_selectedMode.w)/float(m_selectedMode.h);

	if ( screen_ratio > m_ratio)
	{
		int weight = int((m_ratio/screen_ratio)*(float)m_selectedMode.w);
		int x = (m_selectedMode.w - weight)/2;
		glViewport(x,0,weight,m_selectedMode.h);
		//glScissor (x,0,weight,m_selectedMode.h);
		//glEnable(GL_SCISSOR_TEST);
	}
	else if ( screen_ratio < m_ratio)
	{
		int height = int((screen_ratio/m_ratio)*(float)m_selectedMode.h);
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
}

//---------------------------------------------------------------------------//

int Screen::compareModes(const void *a, const void *b)
{
	vmode* A = (vmode*)a;
	vmode* B = (vmode*)b;

	return (A->w * A->h) < (B->w * B->h); //SORTED GREATER-->SMALLER
}

//---------------------------------------------------------------------------//

void Screen::deleteInstance()
{
	if (m_instance) delete m_instance;
}

