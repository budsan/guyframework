#pragma once

#include "base/defines.h"
#include "base/gamestate.h"

#include "graphics/screen.h"
#include "graphics/texturemanager.h"
#include "graphics/color.h"

#include "audio/emyl.h"

class SplashState : public GameState
{
public:

	 SplashState();
	~SplashState();

	void Update(float GameTime);
	void Draw();

	void Load();
	void Unload();
	
private:

	class InnerState
	{
	public:
		InnerState(SplashState &mySplash);

		void Update(float GameTime);
		void Draw();

	protected:
		virtual void myUpdate(float GameTime) = 0;
		virtual void myDraw() = 0;

		SplashState &mySplash;
		float myTimeCount;
		rgba myFadeColor;

		TextureManager* myTexMan;
		emyl::manager* myAudio;
	};

	class Starting : public InnerState
	{
	public:
		Starting(SplashState &mySplash);

	protected:
		void myUpdate(float GameTime);
		void myDraw();
	};

	class Presents : public InnerState
	{
	public:
		Presents(SplashState &mySplash);
	protected:
		void myUpdate(float GameTime);
		void myDraw();
	};

	class Gameby : public InnerState
	{
	public:
		Gameby(SplashState &mySplash);
	protected:
		void myUpdate(float GameTime);
		void myDraw();
	};

	class Ending : public InnerState
	{
	public:
		Ending(SplashState &mySplash);
	protected:
		void myUpdate(float GameTime);
		void myDraw();
	};

	friend class InnerState;
	void ChangeState(SplashState::InnerState *next);

	InnerState *myState;
	InnerState *myNextState;

	ALuint myIntroSound;
	emyl::sound* mySoundHandler;
	bool myIsAccumFirstFrame;
};
