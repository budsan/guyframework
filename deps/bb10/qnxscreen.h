#pragma once

#include "graphics/screen.h"
#include "graphics.h"

#include <bps/navigator.h>
#include <bps/bps.h>
#include <bps/event.h>
#include <bps/screen.h>

namespace Guy {

class QNXScreen : public Screen
{
public:
	QNXScreen();
	virtual ~QNXScreen();

	bool preinit();
	bool init();
	void resetViewport();

	void unloadContent();

	bool setMode(const Mode& mode, bool fullscreen = true);
	const std::vector<Mode>& getAvailableModes(unsigned int &size);
	const Mode& getCurrentMode();

	//Clear with alpha blending
	void fillWithColor(const rgba &color);

	void flip();

protected:

	Mode m_selectedMode;
	std::vector<Mode> m_videoModes;
	bool m_isFullscreen;

	//ATRIBS
	EGLDisplay egl_disp;
	EGLSurface egl_surf;

	EGLConfig egl_conf;
	EGLContext egl_ctx;

	screen_context_t screen_ctx;
	screen_window_t screen_win;
	screen_display_t screen_disp;
	int nbuffers;
	int initialized;
};

} // namespace Guy
