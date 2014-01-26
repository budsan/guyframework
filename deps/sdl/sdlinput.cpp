#include "deps/sdl/sdlinput.h"

#include "environment.h"
#include "debug.h"

#include <SDL2/SDL.h>

namespace Guy {

SDLInput::SDLInput() :
    m_focusState(0)
{
	m_keyboard = new Keyboard();
	m_keyboard->setConnected(true);
	m_keyboard->setEnabled(true);
}

SDLInput::~SDLInput()
{

}

void SDLInput::pollEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) handleEvent(event);
}

void SDLInput::waitEvent()
{
	SDL_Event event;
	if (SDL_WaitEvent(&event)) handleEvent(event);
	else GUY_WARN("Error waiting event\n");
}

std::size_t SDLInput::keyboardCount()
{
	return 1;
}

Keyboard& SDLInput::keyboard(std::size_t i)
{
	GUY_ASSERT(i == 0);
	return *m_keyboard;
}

std::size_t SDLInput::gamePadCount()
{
	return 0;
}

GamePad& SDLInput::gamePad(std::size_t i)
{
	(void) i;
	return *(GamePad*)0;
}

int SDLInput::getFocusState()
{
	return m_focusState;
}

void SDLInput::handleEvent(const SDL_Event &event)
{
	switch (event.type)
	{
	case SDL_KEYDOWN:	
	{
		if (!event.key.repeat)
		{
			wchar_t unicode;
			Keyboard::Key key;
			Keyboard::Mod mod;
			SDLkeysymToGuyKey(event.key.keysym, unicode, key, mod);
			m_keyboard->fireKeyDownEvent(unicode, key, mod);
		}

		// ALT-F4 exit method :D
		if ( event.key.keysym.sym == SDLK_F4 &&
		     event.key.keysym.mod & KMOD_ALT)  {
			Environment::instance().exit();
		}
	}
		break;
	case SDL_KEYUP:
	{
		if (!event.key.repeat)
		{
			wchar_t unicode;
			Keyboard::Key key;
			Keyboard::Mod mod;
			SDLkeysymToGuyKey(event.key.keysym, unicode, key, mod);
			m_keyboard->fireKeyUpEvent(unicode, key, mod);
		}
	}
		break;
	case SDL_QUIT:
		Environment::instance().exit();
		break;
	case SDL_WINDOWEVENT:
	{
		switch(event.window.event)
		{
		case SDL_WINDOWEVENT_SHOWN:
		{
			m_focusState |= DrawFocusState;
			auto it = m_focusListeners.begin();
			for (;it != m_focusListeners.end(); ++it) {
				(*it)->onGainDrawFocus();
			}
			break;
		}
		case SDL_WINDOWEVENT_HIDDEN:
		{
			m_focusState &= ~DrawFocusState;
			auto it = m_focusListeners.begin();
			for (;it != m_focusListeners.end(); ++it) {
				(*it)->onLoseDrawFocus();
			}
			break;
		}
		case SDL_WINDOWEVENT_ENTER:
		{
			m_focusState |= MouseFocusState;
			auto it = m_focusListeners.begin();
			for (;it != m_focusListeners.end(); ++it) {
				(*it)->onGainMouseFocus();
			}
			break;
		}
		case SDL_WINDOWEVENT_LEAVE:
		{
			m_focusState &= ~MouseFocusState;
			auto it = m_focusListeners.begin();
			for (;it != m_focusListeners.end(); ++it) {
				(*it)->onLoseMouseFocus();
			}
			break;
		}
		case SDL_WINDOWEVENT_FOCUS_GAINED:
		{
			m_focusState |= InputFocusState;
			auto it = m_focusListeners.begin();
			for (;it != m_focusListeners.end(); ++it) {
				(*it)->onGainInputFocus();
			}
			break;
		}
		case SDL_WINDOWEVENT_FOCUS_LOST:
		{
			m_focusState &= ~InputFocusState;
			auto it = m_focusListeners.begin();
			for (;it != m_focusListeners.end(); ++it) {
				(*it)->onLoseInputFocus();
			}
			break;
		}
		case SDL_WINDOWEVENT_CLOSE:
			Environment::instance().exit();
			break;
		}
		break;
	}
	default:
		break;
	}
}

void SDLInput::SDLkeysymToGuyKey(SDL_Keysym keysym, wchar_t &unicode, Keyboard::Key &key, Keyboard::Mod &mod)
{
	unicode = keysym.scancode;
	switch (keysym.sym)
	{
	case SDLK_LSHIFT :       key = Keyboard::Key_LShift; break;
	case SDLK_RSHIFT :       key = Keyboard::Key_RShift; break;
	case SDLK_LCTRL :        key = Keyboard::Key_LControl; break;
	case SDLK_RCTRL :        key = Keyboard::Key_RControl; break;
	case SDLK_LALT :         key = Keyboard::Key_LAlt; break;
	case SDLK_RALT :         key = Keyboard::Key_RAlt; break;
	case SDLK_LGUI :         key = Keyboard::Key_LSystem; break;
	case SDLK_RGUI :         key = Keyboard::Key_RSystem; break;
	case SDLK_MENU :         key = Keyboard::Key_Menu; break;
	case SDLK_ESCAPE :       key = Keyboard::Key_Escape; break;
	case SDLK_SEMICOLON :    key = Keyboard::Key_SemiColon; break;
	case SDLK_SLASH :        key = Keyboard::Key_Slash; break;
	case SDLK_EQUALS :       key = Keyboard::Key_Equal; break;
	case SDLK_MINUS :        key = Keyboard::Key_Dash; break;
	case SDLK_LEFTBRACKET :  key = Keyboard::Key_LBracket; break;
	case SDLK_RIGHTBRACKET : key = Keyboard::Key_RBracket; break;
	case SDLK_COMMA :        key = Keyboard::Key_Comma; break;
	case SDLK_PERIOD :       key = Keyboard::Key_Period; break;
	case SDLK_QUOTE :        key = Keyboard::Key_Quote; break;
	case SDLK_BACKSLASH :    key = Keyboard::Key_BackSlash; break;
	//case XK_dead_grave :   key = Keyboard::Key_Tilde; break;
	case SDLK_SPACE :        key = Keyboard::Key_Space; break;
	case SDLK_RETURN :       key = Keyboard::Key_Return; break;
	case SDLK_BACKSPACE :    key = Keyboard::Key_BackSpace; break;
	case SDLK_TAB :          key = Keyboard::Key_Tab; break;
	case SDLK_PAGEUP :       key = Keyboard::Key_PageUp; break;
	case SDLK_PAGEDOWN :     key = Keyboard::Key_PageDown; break;
	case SDLK_END :          key = Keyboard::Key_End; break;
	case SDLK_HOME :         key = Keyboard::Key_Home; break;
	case SDLK_INSERT :       key = Keyboard::Key_Insert; break;
	case SDLK_DELETE :       key = Keyboard::Key_Delete; break;
	case SDLK_KP_PLUS :      key = Keyboard::Key_Add; break;
	case SDLK_KP_MINUS :     key = Keyboard::Key_Subtract; break;
	case SDLK_KP_MULTIPLY :  key = Keyboard::Key_Multiply; break;
	case SDLK_KP_DIVIDE :    key = Keyboard::Key_Divide; break;
	case SDLK_PAUSE :        key = Keyboard::Key_Pause; break;
	case SDLK_F1 :           key = Keyboard::Key_F1; break;
	case SDLK_F2 :           key = Keyboard::Key_F2; break;
	case SDLK_F3 :           key = Keyboard::Key_F3; break;
	case SDLK_F4 :           key = Keyboard::Key_F4; break;
	case SDLK_F5 :           key = Keyboard::Key_F5; break;
	case SDLK_F6 :           key = Keyboard::Key_F6; break;
	case SDLK_F7 :           key = Keyboard::Key_F7; break;
	case SDLK_F8 :           key = Keyboard::Key_F8; break;
	case SDLK_F9 :           key = Keyboard::Key_F9; break;
	case SDLK_F10 :          key = Keyboard::Key_F10; break;
	case SDLK_F11 :          key = Keyboard::Key_F11; break;
	case SDLK_F12 :          key = Keyboard::Key_F12; break;
	case SDLK_F13 :          key = Keyboard::Key_F13; break;
	case SDLK_F14 :          key = Keyboard::Key_F14; break;
	case SDLK_F15 :          key = Keyboard::Key_F15; break;
	case SDLK_LEFT :         key = Keyboard::Key_Left; break;
	case SDLK_RIGHT :        key = Keyboard::Key_Right; break;
	case SDLK_UP :           key = Keyboard::Key_Up; break;
	case SDLK_DOWN :         key = Keyboard::Key_Down; break;
	case SDLK_KP_0 :          key = Keyboard::Key_Numpad0; break;
	case SDLK_KP_1 :          key = Keyboard::Key_Numpad1; break;
	case SDLK_KP_2 :          key = Keyboard::Key_Numpad2; break;
	case SDLK_KP_3 :          key = Keyboard::Key_Numpad3; break;
	case SDLK_KP_4 :          key = Keyboard::Key_Numpad4; break;
	case SDLK_KP_5 :          key = Keyboard::Key_Numpad5; break;
	case SDLK_KP_6 :          key = Keyboard::Key_Numpad6; break;
	case SDLK_KP_7 :          key = Keyboard::Key_Numpad7; break;
	case SDLK_KP_8 :          key = Keyboard::Key_Numpad8; break;
	case SDLK_KP_9 :          key = Keyboard::Key_Numpad9; break;
	case SDLK_q :            key = Keyboard::Key_Q; break;
	case SDLK_w :            key = Keyboard::Key_W; break;
	case SDLK_e :            key = Keyboard::Key_E; break;
	case SDLK_r :            key = Keyboard::Key_R; break;
	case SDLK_t :            key = Keyboard::Key_T; break;
	case SDLK_y :            key = Keyboard::Key_Y; break;
	case SDLK_u :            key = Keyboard::Key_U; break;
	case SDLK_i :            key = Keyboard::Key_I; break;
	case SDLK_o :            key = Keyboard::Key_O; break;
	case SDLK_p :            key = Keyboard::Key_P; break;
	case SDLK_a :            key = Keyboard::Key_A; break;
	case SDLK_s :            key = Keyboard::Key_S; break;
	case SDLK_d :            key = Keyboard::Key_D; break;
	case SDLK_f :            key = Keyboard::Key_F; break;
	case SDLK_g :            key = Keyboard::Key_G; break;
	case SDLK_h :            key = Keyboard::Key_H; break;
	case SDLK_j :            key = Keyboard::Key_J; break;
	case SDLK_k :            key = Keyboard::Key_K; break;
	case SDLK_l :            key = Keyboard::Key_L; break;
	case SDLK_z :            key = Keyboard::Key_Z; break;
	case SDLK_x :            key = Keyboard::Key_X; break;
	case SDLK_c :            key = Keyboard::Key_C; break;
	case SDLK_v :            key = Keyboard::Key_V; break;
	case SDLK_b :            key = Keyboard::Key_B; break;
	case SDLK_n :            key = Keyboard::Key_N; break;
	case SDLK_m :            key = Keyboard::Key_M; break;
	case SDLK_0 :            key = Keyboard::Key_Num0; break;
	case SDLK_1 :            key = Keyboard::Key_Num1; break;
	case SDLK_2 :            key = Keyboard::Key_Num2; break;
	case SDLK_3 :            key = Keyboard::Key_Num3; break;
	case SDLK_4 :            key = Keyboard::Key_Num4; break;
	case SDLK_5 :            key = Keyboard::Key_Num5; break;
	case SDLK_6 :            key = Keyboard::Key_Num6; break;
	case SDLK_7 :            key = Keyboard::Key_Num7; break;
	case SDLK_8 :            key = Keyboard::Key_Num8; break;
	case SDLK_9 :            key = Keyboard::Key_Num9; break;
	default:                 key = Keyboard::Key_Unknown; break;
	}

	mod = (Keyboard::Mod) keysym.mod;
}

} // namespace Guy
