#pragma once

#include "graphics/screen.h"

class LinuxScreen : public Screen
{
public:
	 LinuxScreen();
	~LinuxScreen();

	bool preinit();
	bool init();
	void resetViewport();


	void unloadContent();
	void setCaption(const char* GameName);

	bool setMode(const Mode& mode, bool fullscreen = true);
	const std::vector<Mode>& getAvailableModes(unsigned int &size);
	const Mode& getCurrentMode();

	//Clear with alpha blending
	void fillWithColor(const rgba &color);

	void flip();

protected:
	void initGL();

	//STATIC
	static int  compareModes(const void *a, const void *b);

	//ATRIBS
	SDL_Surface* m_screen;
	Mode m_selectedMode;
	std::vector<Mode> m_videoModes;
	bool m_isFullscreen;
};
