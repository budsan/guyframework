#pragma once

#ifdef _WINDOWS
#include <windows.h>
#endif

#include "defines.h"
#include "settings.h"
#include "input/input.h"
#include "graphics/screen.h"
#include "audio/emyl.h"

class GameState;
class Game
{
public:
	 Game();
	~Game();

	bool Init();
	void Run();

	void ChangeState(GameState* next);
	void Exit();

protected:
	Screen*        myScreen; //graphics
	emyl::manager* myAudio; //sound
	Input*         myInput; //input
	Settings*      mySettings;

	//CONFIGURABLE SETTINGS
	void setFramesPerSecond(unsigned short frames); //Default 0
	void setStableGameTime(bool enable, bool autoframeskip = false); //Default false

	virtual void Configure() = 0;

	virtual void   Load();
	virtual void Unload();

	virtual void Update(float GameTime);
	virtual void Draw();

        virtual const char *getName() {return "Game";}
        virtual const char *getVersion() {return "Undefined";}

private:
	GameState* myState;
	GameState* myNextState;
	bool       myExit;

	unsigned short myFramesPerSecond;
        float          mySecondsPerFrame;

	//-GAMELOOP-BEHAVIOR-------------------------------------------------//

	class GameLoop
	{
	public:
		GameLoop(Game* parent);
		virtual void LoopIteration() = 0;
	protected:
		Game* parent;
	};

	class VariableLoop : public GameLoop
	{
	public:
		VariableLoop(Game* parent);
		void LoopIteration();
	private:
		float myNow;
	};

	class StableLoop : public GameLoop
	{
	public:
		StableLoop(Game* parent);
		void LoopIteration();
	private:
		float myNow;
		float myAccumTime;
	};

	class StableSkipLoop : public GameLoop
	{
	public:
		StableSkipLoop(Game* parent);
		void LoopIteration();
	private:
		float myNow;
		float myAccumTime;
	};

	template <class T>
	void ChangeGameLoop()
	{
		if (myGameLoop == NULL) myGameLoop = new T(this);
		else
		{
			T* pointer = dynamic_cast<T*>(myGameLoop);
			if (pointer == NULL) myGameLoop = new T(this);
		}
	}

	void LoopVariable(float &Now);
	void LoopStable(float &Now, float &AcumTime);
	void LoopStableSkip(float &Now, float &AcumTime);

	GameLoop*      myGameLoop;
};

