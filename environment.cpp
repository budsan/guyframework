#include "environment.h"

#if defined(GUY_USE_LINUX)
#include "deps/linux/linuxenvironment.h"
#elif defined(GUY_USE_WINDOWS)
#include "deps/win/winenvironment.h"
#elif defined(GUY_USE_BB10)
#include "deps/bb10/qnxenvironment.h"
#endif

namespace Guy {

#if defined(GUY_USE_LINUX)
    static LinuxEnvironment sEnvironment;
#elif defined(GUY_USE_WINDOWS)
    static WinEnvironment sEnvironment;
#elif defined(GUY_USE_BB10)
	static QNXEnvironment sEnvironment;
#else
	#error "unknown platform"
#endif

Environment *Environment::s_instance = &sEnvironment;

Environment::Environment()
{
}

Environment::~Environment()
{
}

Environment& Environment::instance()
{
	assert(s_instance != NULL);
	return *s_instance;
}

Environment* Environment::ptrInstance()
{
	assert(s_instance != NULL);
	return s_instance;
}

bool Environment::init(Game *game)
{
	m_game = game;
	return true;
}

void Environment::destroy()
{

}

Game& Environment::getGame()
{
	return *m_game;
}

} // namespace Guy


