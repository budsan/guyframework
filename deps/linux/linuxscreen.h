#pragma once

#include "graphics/screen.h"

class LinuxScreen : public Screen
{
public:
	 LinuxScreen();
	~LinuxScreen();

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
	void initGL();

	//STATIC
	static int  compareModes(const void *a, const void *b);

	//ATRIBS
	SDL_Surface* m_screen;
	vmode m_selectedMode;
	vmode *m_videoModes;
	unsigned int m_videoModesSize;
	float m_ratio;
	bool m_isFullscreen;
};
