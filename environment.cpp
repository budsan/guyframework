#include "environment.h"

#if defined(__GAME_FRAMEWORK_USE_LINUX)
#include "deps/linux/linuxenvironment.h"
#elif defined(__GAME_FRAMEWORK_USE_BB10)
#include "deps/bb10/qnxenvironment.h"
#endif

namespace Guy {

#if defined(__GAME_FRAMEWORK_USE_LINUX)
	static LinuxEnvironment sEnvironment;
#elif defined(__GAME_FRAMEWORK_USE_BB10)
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


