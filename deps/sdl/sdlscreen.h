#pragma once

#include "graphics/screen.h"
#include <SDL2/SDL.h>

namespace Guy {

class SDLScreen : public Screen
{
public:
	SDLScreen();
	virtual ~SDLScreen();

	bool preinit();
	bool init();
	void resetViewport();

	void unloadContent();
	void setCaption(const char* GameName);

	bool setMode(const Mode& mode, bool fullscreen = true);
	const std::vector<Mode>& availableModes(unsigned int &size);
	const Mode& currentMode();

	//Clear with alpha blending
	void fillWithColor(const rgba &color);

	void flip();

protected:
	void initGL();

	//ATRIBS
	SDL_Window* m_window;
	const char* m_windowName;
	Mode m_selectedMode;
	std::vector<Mode> m_videoModes;
	bool m_isFullscreen;
};

} // namespace Guy
