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

#include "base/log.h"
#include "base/defines.h"
#include "base/settings.h"


//---------------------------------------------------------------------------//

Screen* Screen::myInstance = 0;
bool Screen::myIsInstanced = false;

Screen& Screen::Instance()
{
	return *pInstance();
}

Screen* Screen::pInstance()
{
	if (Screen::myInstance == 0)
	{
		myInstance = new Screen();
	}
	
	return myInstance;
}

//---------------------------------------------------------------------------//

Screen:: Screen()
{
	myRatio = 0;
	myVideoModesSize = 0;
	myVideoModes = NULL;
	myScreen = NULL;

	if (!myIsInstanced) {
		atexit(Screen::DeleteInstance);
		myIsInstanced = true;
	}
}

//---------------------------------------------------------------------------//

Screen::~Screen()
{
	myInstance = NULL;

	if (SDL_WasInit(SDL_INIT_VIDEO)) {
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
	}

	if (!SDL_WasInit(SDL_INIT_EVERYTHING)) {
		SDL_Quit();
	}

	UnloadContent();
}

//---------------------------------------------------------------------------//

bool Screen::Init()
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

void Screen::Flip()
{
	SDL_GL_SwapBuffers();
}

//---------------------------------------------------------------------------//

void Screen::FillWithColor(const rgba &color)
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
	size = myVideoModesSize;
	if(!size) return NULL;
	
	return myVideoModes;
}
//---------------------------------------------------------------------------//

const Screen::vmode* Screen::getCurrentVideoMode()
{
	return &mySelectedMode;
}

//---------------------------------------------------------------------------//

float Screen::getRatio()
{
	if (myRatio != 0.0f) return myRatio;
	if (mySelectedMode.h == 0.0f) return 0.0f;
	return float(mySelectedMode.w)/float(mySelectedMode.h);
}

//---------------------------------------------------------------------------//

void Screen::setRatio(float ratio)
{
	if (ratio == myRatio) return;
	myRatio = ratio;
	resetViewport();
}

//---------------------------------------------------------------------------//

bool Screen::setVideoMode()
{
	Settings *config = Settings::pInstance();

	if (myScreen != NULL)
	{
		if (mySelectedMode.w  == config->ScreenWidth &&
		    mySelectedMode.h  == config->ScreenHeight)
		{
			if (config->Fullscreen != myIsFullscreen)
				SDL_WM_ToggleFullScreen(myScreen);

			return true;
		}
		else UnloadContent();
	}

	mySelectedMode.w   = config->ScreenWidth;
	mySelectedMode.h   = config->ScreenHeight;
	mySelectedMode.bpp = config->ScreenBpp;

	unsigned int flags =
		SDL_OPENGL |
		SDL_GL_DOUBLEBUFFER |
		SDL_HWPALETTE |
		SDL_HWSURFACE |
		SDL_HWACCEL;

	if (config->Fullscreen) flags |= SDL_FULLSCREEN; 

	SDL_Surface* screen = NULL;

	//GET AVAILABLE VIDEO MODES
	SDL_Rect **modes_available = SDL_ListModes(NULL, flags|SDL_FULLSCREEN);
	const SDL_VideoInfo* videoinfo = SDL_GetVideoInfo();
	if (modes_available == (SDL_Rect **) 0) return NULL;
	unsigned int nModes;
	for (nModes = 0; modes_available[nModes]; nModes++) {}
	
	myVideoModesSize = nModes;
	myVideoModes = new vmode[nModes];
	for(nModes = 0; modes_available[nModes]; nModes++)
	{
		myVideoModes[nModes].w = modes_available[nModes]->w;
		myVideoModes[nModes].h = modes_available[nModes]->h;
		myVideoModes[nModes].bpp = config->ScreenBpp;
	}
	qsort( myVideoModes, nModes, sizeof(vmode), CompareModes);
	
	//select settings video-mode if exists
	if (mySelectedMode.w != 0 || mySelectedMode.h != 0)
	{
		screen = SDL_SetVideoMode (
			mySelectedMode.w,
			mySelectedMode.h, 
			mySelectedMode.bpp, 
			flags);
	}

	//select current desktop video-mode
	if (screen == NULL)
	{
		mySelectedMode.w = videoinfo->current_w;
		mySelectedMode.h = videoinfo->current_h;
		screen = SDL_SetVideoMode (
			mySelectedMode.w,
			mySelectedMode.h, 
			mySelectedMode.bpp, 
			flags);
	}

	//select one of compatible video-mode
	for(unsigned int i = 0; i < nModes && screen == NULL; i++)
	{
		mySelectedMode.w = myVideoModes[i].w;
		mySelectedMode.h = myVideoModes[i].h;
		
		screen = SDL_SetVideoMode (
			mySelectedMode.w,
			mySelectedMode.h, 
			mySelectedMode.bpp, 
			flags);
	}

	if (screen == NULL) return false;

	config->ScreenWidth  = mySelectedMode.w;
	config->ScreenHeight = mySelectedMode.h;
	config->ScreenBpp    = mySelectedMode.bpp;
	myIsFullscreen = config->Fullscreen;

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	initGL();

	myScreen = screen;
	SDL_WM_SetCaption(GAME_NAME, GAME_NAME);

	return true;
}

//---------------------------------------------------------------------------//
void Screen::UnloadContent()
{
	//Añade los unloads de todos los gestores aqui
	TextureManager::Instance().UnloadTextures();
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
	if ( myRatio == 0 )
	{
		glViewport(0,0,mySelectedMode.w,mySelectedMode.h);
		//glDisable(GL_SCISSOR_TEST);
		return;
	}
	
	if (mySelectedMode.h == 0.0f) return;
	float screen_ratio = float(mySelectedMode.w)/float(mySelectedMode.h);

	if ( screen_ratio > myRatio)
	{
		int weight = int((myRatio/screen_ratio)*(float)mySelectedMode.w);
		int x = (mySelectedMode.w - weight)/2;
		glViewport(x,0,weight,mySelectedMode.h);
		//glScissor (x,0,weight,mySelectedMode.h);
		//glEnable(GL_SCISSOR_TEST);
	}
	else if ( screen_ratio < myRatio)
	{
		int height = int((screen_ratio/myRatio)*(float)mySelectedMode.h);
		int y = (mySelectedMode.h - height)/2;
		glViewport(0,y,mySelectedMode.w,height);
		//glScissor (0,y,mySelectedMode.w,height);
		//glEnable(GL_SCISSOR_TEST);
	}
	else
	{
		glViewport(0,0,mySelectedMode.w,mySelectedMode.h);
		//glDisable(GL_SCISSOR_TEST);
	}
}

//---------------------------------------------------------------------------//

int Screen::CompareModes(const void *a, const void *b)
{
	vmode* A = (vmode*)a;
	vmode* B = (vmode*)b;

	return (A->w * A->h) < (B->w * B->h); //SORTED GREATER-->SMALLER
}

//---------------------------------------------------------------------------//

void Screen::DeleteInstance()
{
	if (myInstance) delete myInstance;
}

