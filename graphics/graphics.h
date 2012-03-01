#pragma once

#ifndef INCLUDED_FROM_QT

#ifdef _WINDOWS
#include <windows.h>
#endif

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#else

#include <QtOpenGL/qgl.h>
#include <GL/glu.h>

#endif
