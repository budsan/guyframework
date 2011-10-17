#pragma once

#include "graphics.h"
#include "color.h"
#include <map>
#include <string>

class Screen
{
public:
	static Screen& Instance();
	static Screen* pInstance();
	~Screen(void);

	typedef struct
	{
		unsigned int w;
		unsigned int h;
		unsigned int bpp;
	} vmode;

	bool Init();
	void Flip();

	//Clear with alpha blending
	void FillWithColor(const rgba &color);
	
	bool setVideoMode();
	void UnloadContent();
	void setCaption(const char* GameName);

	//TOOLS
	float        getRatio();
	void         setRatio(float);
	const vmode* getVideoModeList(unsigned int &size);
	const vmode* getCurrentVideoMode();
	void resetViewport();

protected:

	Screen();
	void initGL();


	//STATIC
	static int  CompareModes(const void *a, const void *b);
	static void DeleteInstance();
	
	static Screen* myInstance;
	static bool myIsInstanced;

	//ATRIBS
	SDL_Surface* myScreen;
	vmode mySelectedMode;
	vmode *myVideoModes;
	unsigned int myVideoModesSize;
	float myRatio;
	bool myIsFullscreen;
};

