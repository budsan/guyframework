#-------------------------------------------------
#
# Project created by QtCreator 2011-01-23T14:40:42
#
#-------------------------------------------------

QT       += core
QT       -= gui

TARGET = game
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += -lSDL -lGL -lGLU -lGLEW -lpng -lopenal -lvorbis -logg -lvorbisfile -lfreetype

SOURCES += \
    base/game.cpp \
    base/settings.cpp \
    base/gamestate.cpp \
    base/log.cpp \
    graphics/screen.cpp \
    graphics/texture.cpp \
    graphics/sprite.cpp \
    graphics/spriteanim.cpp \
    graphics/texturemanager.cpp \
    graphics/shaderprogram.cpp \
    graphics/shader.cpp \
    graphics/framebuffer.cpp \
    graphics/color.cpp \
    input/input.cpp \
    input/inputstate.cpp \
    tools/transition.cpp \
    math/hit.cpp \
    enginestate.cpp \
    main.cpp \
    graphics/camera2d.cpp \
    graphics/primitives.cpp \
    audio/emyl.cpp \
    graphics/tools/frameshud.cpp \
    graphics/font.cpp \
    mygame.cpp \
    splashstate.cpp \
    menustate.cpp \
    tools/particleemitter.cpp \
    tools/particle.cpp \
    graphics/spritestatic.cpp \
    logic/wall.cpp \
    logic/unitscout.cpp \
    logic/unitmelee.cpp \
    logic/unitdistance.cpp \
    logic/unitdefense.cpp \
    logic/unit.cpp \
    logic/team.cpp \
    logic/scene.cpp \
    logic/damagelabel.cpp \
    logic/player.cpp

    
HEADERS += \
    base/defines.h \
    base/game.h \
    base/gamestate.h \
    base/settings.h \
    base/log.h \
    graphics/screen.h \
    graphics/texture.h \
    graphics/sprite.h \
    graphics/spriteanim.h \
    graphics/color.h \
    graphics/graphics.h \
    graphics/texturemanager.h \
    graphics/shaderprogram.h \
    graphics/shader.h \
    graphics/framebuffer.h \
    input/input.h \
    input/inputstate.h \
    tools/transition.h \
    math/bbox.h \
    math/poly2.h \
    math/ray.h \
    math/seg.h \
    math/tri.h \
    math/vec2.h \
    math/vec3.h \
    enginestate.h \
    graphics/camera2d.h \
    graphics/primitives.h \
    audio/emyl.h \
    graphics/tools/frameshud.h \
    graphics/font.h \
    mygame.h \
    splashstate.h \
    menustate.h \
    tools/particleemitter.h \
    tools/particle.h \
    graphics/spritestatic.h \
    logic/wall.h \
    logic/unitscout.h \
    logic/unitmelee.h \
    logic/unitdistance.h \
    logic/unitdefense.h \
    logic/unit.h \
    logic/team.h \
    logic/scene.h \
    logic/damagelabel.h \
    logic/event.h \
    logic/player.h

QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += . /usr/include/freetype2/
