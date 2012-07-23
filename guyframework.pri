CONFIG -= qt

LIBS += -lvorbisfile -lvorbis -logg -lfreetype

win32 {
    DEFINES += GUY_USE_WINDOWS
    LIBS += -static -L$$PWD/extlibs/libs-mingw/ -lmingw32 -lSDL.dll -mwindows -lopengl32 -lglu32 -lopenal32
    INCLUDEPATH += $$PWD/extlibs/include
}

unix {
    DEFINES += GUY_USE_LINUX
    LIBS += -lSDL -lGL -lGLU -lopenal -lGLEW
    INCLUDEPATH += /usr/include/freetype2/
}

QMAKE_CXXFLAGS += -Wextra -Wconversion -Wuninitialized -Wmissing-include-dirs -Wshadow
#QMAKE_LDFLAGS += -pg

INCLUDEPATH += $$PWD/

SOURCES += \
    $$PWD/environment.cpp \
    $$PWD/game.cpp \
    $$PWD/persistencelayer.cpp \
    $$PWD/variable.cpp \
    $$PWD/content/content.cpp \
    $$PWD/graphics/screen.cpp \
    $$PWD/graphics/texture.cpp \
    $$PWD/graphics/sprite.cpp \
    $$PWD/graphics/spriteanim.cpp \
    $$PWD/graphics/texturemanager.cpp \
    $$PWD/graphics/shaderprogram.cpp \
    $$PWD/graphics/shader.cpp \
    $$PWD/graphics/framebuffer.cpp \
    $$PWD/graphics/camera2d.cpp \
    $$PWD/graphics/primitives.cpp \
    $$PWD/graphics/color.cpp \
    $$PWD/input/input.cpp \
    $$PWD/input/keyboard.cpp \
    $$PWD/input/gamepad.cpp \
    $$PWD/input/controller.cpp \
    $$PWD/tools/transition.cpp \
    $$PWD/math/hit.cpp \
    $$PWD/audio/emyl.cpp \
    $$PWD/graphics/tools/frameshud.cpp \
    $$PWD/graphics/tools/texthud.cpp \
    $$PWD/graphics/font.cpp \
    $$PWD/tools/particleemitter.cpp \
    $$PWD/tools/particle.cpp \
    $$PWD/graphics/spritestatic.cpp \
    $$PWD/deps/sdl/sdlscreen.cpp \
    $$PWD/deps/sdl/sdlinput.cpp \
    $$PWD/deps/sdl/sdlenvironment.cpp

win32 {
SOURCES += $$PWD/deps/win/winmain.cpp \
    $$PWD/deps/win/winenvironment.cpp \
    $$PWD/deps/win/winlog.cpp \
    $$PWD/deps/win/winpersistencelayer.cpp \
    $$PWD/deps/win/glew.c
}

unix {
SOURCES += $$PWD/deps/linux/linuxenvironment.cpp \
    $$PWD/deps/linux/linuxlog.cpp \
    $$PWD/deps/linux/linuxpersistencelayer.cpp
}
    
HEADERS += \
    $$PWD/environment.h \
    $$PWD/game.h \
    $$PWD/persistencelayer.h \
    $$PWD/log.h \
    $$PWD/content/content.h \
    $$PWD/graphics/graphics.h \
    $$PWD/graphics/screen.h \
    $$PWD/graphics/texture.h \
    $$PWD/graphics/sprite.h \
    $$PWD/graphics/spriteanim.h \
    $$PWD/graphics/color.h \
    $$PWD/graphics/texturemanager.h \
    $$PWD/graphics/shaderprogram.h \
    $$PWD/graphics/shader.h \
    $$PWD/graphics/framebuffer.h \
    $$PWD/graphics/camera2d.h \
    $$PWD/graphics/primitives.h \
    $$PWD/input/input.h \
    $$PWD/input/keyboard.h \
    $$PWD/input/gamepad.h \
    $$PWD/input/keyboardlistener.h \
    $$PWD/input/controller.h \
    $$PWD/input/gamepadlistener.h \
    $$PWD/input/focuslistener.h \
    $$PWD/tools/transition.h \
    $$PWD/math/bbox.h \
    $$PWD/math/poly2.h \
    $$PWD/math/ray.h \
    $$PWD/math/seg.h \
    $$PWD/math/tri.h \
    $$PWD/math/algebra3.h \
    $$PWD/math/algebra3_cpp11.h \
    $$PWD/audio/emyl.h \
    $$PWD/graphics/tools/frameshud.h \
    $$PWD/graphics/tools/texthud.h \
    $$PWD/graphics/font.h \
    $$PWD/tools/particleemitter.h \
    $$PWD/tools/particle.h \
    $$PWD/graphics/spritestatic.h \
    $$PWD/variable.h \
    $$PWD/graphics/camera.h \
    $$PWD/deps/sdl/sdlscreen.h \
    $$PWD/deps/sdl/sdlinput.h \
    $$PWD/deps/sdl/sdlenvironment.h \
    $$PWD/deps/stb_image/stb_image_write.h \
    $$PWD/deps/stb_image/stb_image.h

win32 {
HEADERS += $$PWD/deps/win/winenvironment.h \
    $$PWD/deps/win/winpersistencelayer.h \
    $$PWD/deps/win/graphics.h
}

unix {
HEADERS += $$PWD/deps/linux/linuxenvironment.h \
    $$PWD/deps/linux/linuxpersistencelayer.h \
    $$PWD/deps/linux/graphics.h
}

# Needed for GUY_USE_QT_DEPS define
# SOURCES += guyframework/deps/qt/textureqtloader.cpp
# HEADERS  += guyframework/deps/qt/textureqtloader.h
