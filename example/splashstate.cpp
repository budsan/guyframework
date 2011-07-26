#include "splashstate.h"
#include "menustate.h"

#include "audio/emyl.h"
#include "graphics/screen.h"
#include "graphics/texturemanager.h"
#include "graphics/color.h"

#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>


#define ACUMM_NEW_FRAME_MULT 0.15f
#define ACUMM_OLD_FRAME_MULT (1.0f-ACUMM_NEW_FRAME_MULT)

using namespace std;

SplashState:: SplashState() : myState(NULL), myNextState(NULL), myIntroSound(0)
{

}

SplashState::~SplashState()
{
	Unload();
}

void SplashState::Load()
{
	if (myState != NULL)
	{
		delete myState;
	}

	myState = new Starting(*this);

	//LOADING TEXTURES
	TextureManager &texman= TextureManager::Instance();
	texman.UseTexture("logo");
	texman.UseTexture("presents");
	texman.UseTexture("gameby");

	if (myIntroSound == 0)
	{
		emyl::manager* audiomng = emyl::manager::get_instance();
		myIntroSound = audiomng->get_buffer("data/sound/intro.ogg");

		mySoundHandler = new emyl::sound();
		mySoundHandler->set_buffer(myIntroSound);
		mySoundHandler->set_source();
	}

	//OPENGL INITS
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT, GL_FILL);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	myIsAccumFirstFrame = true;
}

void SplashState::Unload()
{
	TextureManager &texman= TextureManager::Instance();
	texman.DeleteTexture("logo");
	texman.DeleteTexture("presents");
	texman.DeleteTexture("gameby");

	if (myIntroSound) {
		emyl::manager* mng = emyl::manager::get_instance();
		delete mySoundHandler;
		mng->delete_buffer("data/sound/intro.ogg");
		myIntroSound = 0;
	}

	if (myState != NULL)
	{
		delete myState;
		myState = NULL;
	}

	//OPENGL RESTAURE
	glPopAttrib();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glClear(GL_ACCUM_BUFFER_BIT);


}

void SplashState::ChangeState(SplashState::InnerState *next)
{
	if (myNextState != NULL)
	{
		delete myNextState;
	}

	myNextState = next;
}

void SplashState::Update(float GameTime)
{
	if (myNextState != NULL)
	{
		if (myState != NULL) delete myState;
		myState = myNextState;
		myNextState = NULL;
	}

	myState->Update(GameTime);
}

void SplashState::Draw()
{
	myState->Draw();
}

//-STATE---------------------------------------------------------------------//

SplashState::InnerState::InnerState(SplashState &mySplash) : mySplash(mySplash), myTimeCount(0)
{
	myTexMan = TextureManager::pInstance();
	myAudio = emyl::manager::get_instance();
}

void SplashState::InnerState::Update(float GameTime)
{
	myUpdate(GameTime);
	myTimeCount += GameTime;
}

void SplashState::InnerState::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	myDraw();

	Screen::Instance().FillWithColor(myFadeColor);
	if (mySplash.myIsAccumFirstFrame) mySplash.myIsAccumFirstFrame = false;
	else
	{
		glAccum(GL_ACCUM, ACUMM_NEW_FRAME_MULT);
		glAccum(GL_RETURN, 1.f);
	}

	glClear(GL_ACCUM_BUFFER_BIT);
	glAccum(GL_ACCUM, ACUMM_OLD_FRAME_MULT);
}

//-STARTING-------------------------------------------------------------------//

SplashState::Starting::Starting(SplashState &mySplash) : InnerState(mySplash)
{
	myFadeColor = rgba(0,0,0,1);
}

void SplashState::Starting::myUpdate(float GameTime)
{
	// De 0,7segs a 1seg hacemos fade
	if(myTimeCount > 0.7f && myTimeCount < 0.999f)
	{
		myFadeColor = rgba(1.0f - (1.0f - myTimeCount)/(1.0f - 0.7f));
		myFadeColor.a = 1.0f; //A
	}

	// Pasado el segundo, cambiamos de estado y el fade a blanco
	if(myTimeCount >= 1.0f)
	{
		myFadeColor = rgba(1.0f);
		mySplash.ChangeState(new SplashState::Presents(mySplash));
		mySplash.mySoundHandler->play();
	}
}

void SplashState::Starting::myDraw()
{

}

//-PRESENTS------------------------------------------------------------------//

SplashState::Presents::Presents(SplashState &mySplash) : InnerState(mySplash)
{
	myFadeColor = rgba(1.0f);
}

void SplashState::Presents::myUpdate(float GameTime)
{
	// Fade out de blanco a transparente
	if(myTimeCount < 0.3f) {
		myFadeColor.a = (0.3f - myTimeCount)/(0.3f);
	}
	//Esperamos un 0.3s a 3,7s a que se muestre el PRESENTS
	else if(myTimeCount >= 0.3f && myTimeCount <= 3.5f) {
		myFadeColor.a = 0.0f;
	}
	//Fade in a blanco de 3 milisegundos
	else if(myTimeCount > 3.5f && myTimeCount < 3.799f) {
		myFadeColor.a = 1.0f - (3.8f - myTimeCount)/(3.8f - 3.5f);
	}
	//Y finalmente cambiamos el estado y el fade a blanco
	else if(myTimeCount >= 3.8f) {
		myFadeColor.a = 1.0f;
		mySplash.ChangeState(new SplashState::Gameby(mySplash));
	}
}

void SplashState::Presents::myDraw()
{
	Screen &gfx = Screen::Instance();
	gfx.FillWithColor(rgba(1,1,1,1));
	float ratio = gfx.getRatio();

	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	float fov = 60 -(myTimeCount*myTimeCount/16.0f)*10;
	gluPerspective(fov,ratio, 0.1, 150.0);

	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	gluLookAt(-1.50f, 1.50f, 4.00f,
		  -0.25f, -(myTimeCount*myTimeCount/80), 0.00f,
		   0.00f, 1.00f, 0.00f);

	myTexMan->UseTexture("logo");
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);

	 //VGAFIB LOGO
	 glColor4f(1, 1, 1, 1);
	 glTexCoord2f(0.0f, 0.0f); glVertex3f(-2, 0, 0);
	 glTexCoord2f(1.0f, 0.0f); glVertex3f( 2, 0, 0);
	 glTexCoord2f(1.0f, 1.0f); glVertex3f( 2, 1, 0);
	 glTexCoord2f(0.0f, 1.0f); glVertex3f(-2, 1, 0);

	 //VGAFIB LOGO SHADOW
	 glColor4f(1, 1, 1, 0.5f); glTexCoord2f(0, 0); glVertex3f(-2, 0.03f, 0);
	 glColor4f(1, 1, 1, 0.5f); glTexCoord2f(1, 0); glVertex3f( 2, 0.03f, 0);
	 glColor4f(1, 1, 1, 0); glTexCoord2f(1, 0.8f); glVertex3f( 2, 0.03f, 1);
	 glColor4f(1, 1, 1, 0); glTexCoord2f(0, 0.8f); glVertex3f(-2, 0.03f, 1);

	glEnd();

	//PRESENTS
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glOrtho(-ratio/2.0f, ratio/2.0f, -0.5f, 0.5f, -1, 1);
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	glTranslatef(0,-0.3175f,-0.5f);

	const Texture &presents = myTexMan->getTexture("presents");
	presents.setFiltering(GL_NEAREST);
	presents.setWrap(GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, rgba(1,1,1,1).raw());

	//VARIABLE DISTANCE BETWEEN LETTERS
	//EACH ARE SIZED (1/16)x(1/16), 0.0f <= SPLASH_TIMECOUNT <= 4.0f
	const float stride = (myTimeCount*myTimeCount)/(16.0f * 9.0f);
	float pos;

	//CENTER LETTERS, STABLISH INITIAL POSSITION.
	//-((INDEX * NUM_SPACES_BETWEEN_LETTERS) + PRESENTS_LENGHT)/2.0f
	pos = -(stride*7.0f + 0.5f) * 0.5f;

	//DRAW SHADE
	glBegin(GL_QUADS);
	for(int i = 0; i < 8; i++)
	{
		glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
		glTexCoord2f((i+0.0f)/8.f,0); glVertex3f(pos, 0, 0);
		glTexCoord2f((i+1.0f)/8.f,0); glVertex3f(pos+(1.f/16.f), 0, 0);
		glTexCoord2f((i+1.0f)/8.f,1); glVertex3f(pos+(1.f/16.f), 1.f/16.f, 0);
		glTexCoord2f((i+0.0f)/8.f,1); glVertex3f(pos, 1.f/16.f , 0);
		pos += (stride + 1.0f/16.0f); //LETTER LENGHT + INDEX
	}
	glEnd();

	//REINIT POS
	pos = -(stride*7.0f + 0.5f) * 0.5f;

	//DRAW PRESENTS
	glTranslatef(-0.005f, 0.005f, 0);
	glBegin(GL_QUADS);
	for(int i = 0; i < 8; i++)
	{
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glTexCoord2f((i+0.0f)/8.f,0); glVertex3f(pos, 0, 0);
		glTexCoord2f((i+1.0f)/8.f,0); glVertex3f(pos+(1.f/16.f), 0, 0);
		glTexCoord2f((i+1.0f)/8.f,1); glVertex3f(pos+(1.f/16.f), 1.f/16.f, 0);
		glTexCoord2f((i+0.0f)/8.f,1); glVertex3f(pos, 1.f/16.f , 0);
		pos += (stride + 1.0f/16.0f);
	}
	glEnd();
}

//-GAMEBY--------------------------------------------------------------------//

SplashState::Gameby::Gameby(SplashState &mySplash) : InnerState(mySplash)
{
    myFadeColor = rgba(1.0f);
}

void SplashState::Gameby::myUpdate(float GameTime)
{
	//Fade out de blanco a transparente de 0.6s
	if(myTimeCount <= 0.6f) {
		myFadeColor.a = (0.6f - myTimeCount)/(0.6f); //A
	}
	//mostramos GAMEBY de 0.3s a 4.4s
	else if(myTimeCount >= 0.3f && myTimeCount <= 4.4f) {
		myFadeColor.a = 0.0f; //A
	}
	//Fade in de transparente a negro de 0.6s
	else if(myTimeCount > 4.4f && myTimeCount < 5.0f) {
		myFadeColor   = rgba(0.0f);
		myFadeColor.a = 1.0f - (5.0f - myTimeCount)/(5.0f - 4.4f); //A
	}
	//Y finalmente hemos acabado y cambiamos de estado.
	else if(myTimeCount >= 5.0f) {
		myFadeColor.a = 1.0f; //A
		mySplash.ChangeState(new SplashState::Ending(mySplash));
	}
}

void SplashState::Gameby::myDraw()
{
	Screen &gfx = Screen::Instance();
	gfx.FillWithColor(rgba(1,1,1,1));
	float ratio = gfx.getRatio();

	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-ratio, ratio, -1.0f, 1.0f, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	myTexMan->UseTexture("gameby");

	glBegin(GL_QUADS);
	 glColor4f(1, 1, 1, 1);
	 glTexCoord2f(0, 0); glVertex3f(-1.2f,-0.6f, -0.5f);
	 glTexCoord2f(1, 0); glVertex3f( 1.2f,-0.6f, -0.5f);
	 glTexCoord2f(1, 1); glVertex3f( 1.2f, 0.6f, -0.5f);
	 glTexCoord2f(0, 1); glVertex3f(-1.2f, 0.6f, -0.5f);
	glEnd();
}

//-ENDING--------------------------------------------------------------------//

SplashState::Ending::Ending(SplashState &mySplash) : InnerState(mySplash)
{
    myFadeColor   = rgba(0.0f, 0.0f, 0.0f, 1.0f);
}

void SplashState::Ending::myUpdate(float GameTime)
{
	//Esperamos 1 segundo y nos vamos al menu
	if(myTimeCount >= 1.0f)
	{
		mySplash.getGame()->ChangeState(new MenuState());
	}
}

void SplashState::Ending::myDraw()
{

}
