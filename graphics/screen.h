#pragma once

#include "graphics.h"
#include "color.h"
#include <map>
#include <string>

class Screen
{
public:
	static Screen& instance();
	static Screen* ptrInstance();
	~Screen(void);

	typedef struct
	{
		unsigned int w;
		unsigned int h;
		unsigned int bpp;
	} vmode;

	bool init();
	void flip();

	//Clear with alpha blending
	void fillWithColor(const rgba &color);
	
	bool setVideoMode();
	void unloadContent();
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
	static int  compareModes(const void *a, const void *b);
	static void deleteInstance();
	
	static Screen* m_instance;
	static bool m_isInstanced;

	//ATRIBS
	SDL_Surface* m_screen;
	vmode m_selectedMode;
	vmode *m_videoModes;
	unsigned int m_videoModesSize;
	float m_ratio;
	bool m_isFullscreen;
};

