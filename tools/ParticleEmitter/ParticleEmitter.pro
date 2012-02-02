#-------------------------------------------------
#
# Project created by QtCreator 2010-07-06T22:46:34
#
#-------------------------------------------------

QT += core gui opengl

TARGET = ParticleEmitter
TEMPLATE = app
SOURCES += main.cpp\
	particleeditor.cpp \
	editorwidget.cpp \
    ../../graphics/color.cpp \
    ../editorcamera2d.cpp \
    ../../graphics/texturemanager.cpp \
    ../../graphics/texture.cpp \
    ../../log.cpp \
    ../particleemitter.cpp \
    ../particle.cpp \
    particleparameters.cpp

HEADERS += particleeditor.h \
	editorwidget.h \
    ../../graphics/color.h \
    ../editorcamera2d.h \
    ../../graphics/texturemanager.h \
    ../../graphics/texture.h \
    ../../base/log.h \
    ../particleemitter.h \
    ../particle.h \
    particleparameters.h \
    ../../math/vec3.h \
    ../../math/vec2.h \
    ../../math/bbox.h \
    ../../math/mat4.h

FORMS += particleeditor.ui \
    particleparameters.ui

DEFINES += INCLUDED_FROM_QT

QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += ../../ ../../tools

