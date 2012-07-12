
DEFINES += __GAME_FRAMEWORK_USE_LINUX

LIBS += -lSDL -lGL -lGLU -lGLEW -lpng -lopenal -lvorbis -logg -lvorbisfile -lfreetype

QMAKE_CXXFLAGS += -Wextra -Wconversion -Wuninitialized -Wmissing-include-dirs -Wshadow -pg
QMAKE_LDFLAGS += -pg

INCLUDEPATH += $$PWD/ /usr/include/freetype2/

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
    $$PWD/tools/transition.cpp \
    $$PWD/math/hit.cpp \
    $$PWD/audio/emyl.cpp \
    $$PWD/graphics/tools/frameshud.cpp \
    $$PWD/graphics/tools/texthud.cpp \
    $$PWD/graphics/font.cpp \
    $$PWD/tools/particleemitter.cpp \
    $$PWD/tools/particle.cpp \
    $$PWD/graphics/spritestatic.cpp \
    $$PWD/deps/linux/linuxenvironment.cpp \
    $$PWD/deps/linux/linuxlog.cpp \
    $$PWD/deps/linux/linuxinput.cpp \
    $$PWD/deps/linux/linuxpersistencelayer.cpp \
    $$PWD/deps/linux/linuxscreen.cpp \
    gameframework/input/controller.cpp


    
HEADERS += \
    $$PWD/environment.h \
    $$PWD/game.h \
    $$PWD/persistencelayer.h \
    $$PWD/log.h \
    $$PWD/content/content.h \
    $$PWD/graphics/screen.h \
    $$PWD/graphics/texture.h \
    $$PWD/graphics/sprite.h \
    $$PWD/graphics/spriteanim.h \
    $$PWD/graphics/color.h \
    $$PWD/graphics/texturemanager.h \
    $$PWD/graphics/shaderprogram.h \
    $$PWD/graphics/shader.h \
    $$PWD/graphics/framebuffer.h \
    $$PWD/input/input.h \
    $$PWD/input/keyboard.h \
    $$PWD/input/gamepad.h \
    $$PWD/input/keyboardlistener.h \
    $$PWD/input/controller.h \
    $$PWD/input/gamepadlistener.h \
    $$PWD/tools/transition.h \
    $$PWD/math/bbox.h \
    $$PWD/math/poly2.h \
    $$PWD/math/ray.h \
    $$PWD/math/seg.h \
    $$PWD/math/tri.h \
    $$PWD/math/algebra3.h \
    $$PWD/math/algebra3_cpp11.h \
    $$PWD/graphics/camera2d.h \
    $$PWD/graphics/primitives.h \
    $$PWD/audio/emyl.h \
    $$PWD/graphics/tools/frameshud.h \
    $$PWD/graphics/tools/texthud.h \
    $$PWD/graphics/font.h \
    $$PWD/tools/particleemitter.h \
    $$PWD/tools/particle.h \
    $$PWD/graphics/spritestatic.h \
    $$PWD/variable.h \
    $$PWD/graphics/camera.h \
    $$PWD/deps/linux/linuxenvironment.h \
    $$PWD/deps/linux/linuxinput.h \
    $$PWD/deps/linux/linuxpersistencelayer.h \
    $$PWD/deps/linux/linuxscreen.h \
    gameframework/deps/linux/graphics.h \
    gameframework/graphics/graphics.h

