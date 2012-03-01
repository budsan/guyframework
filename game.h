#pragma once

#ifdef _WINDOWS
#include <windows.h>
#endif

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

	bool init();
	void run();

	void changeState(GameState* next);
	void exit();

protected:
	Screen*        m_screen; //graphics
	emyl::manager* m_audio; //sound
	Input*         m_input; //input
	Settings*      m_settings;

	//CONFIGURABLE SETTINGS
	void setFramesPerSecond(unsigned short frames); //Default 0
	void setStableDeltaTime(bool enable, bool autoframeskip = false); //Default false

	virtual void configure() = 0;

	virtual void   load();
	virtual void unload();

	virtual void update(float deltaTime);
	virtual void draw();

        virtual const char *getName() {return "Game";}
        virtual const char *getVersion() {return "Undefined";}

private:
	GameState* m_State;
	GameState* m_NextState;
	bool       m_Exit;

	unsigned short m_FramesPerSecond;
	float          m_SecondsPerFrame;

	//-GAMELOOP-BEHAVIOR-------------------------------------------------//

	class GameLoop
	{
	public:
		GameLoop(Game* parent);
		virtual void loopIteration() = 0;
	protected:
		Game* parent;
	};

	class VariableLoop : public GameLoop
	{
	public:
		VariableLoop(Game* parent);
		void loopIteration();
	private:
        float m_now;
	};

	class StableLoop : public GameLoop
	{
	public:
		StableLoop(Game* parent);
		void loopIteration();
	private:
        float m_now;
		float m_accumTime;
	};

	class StableSkipLoop : public GameLoop
	{
	public:
		StableSkipLoop(Game* parent);
		void loopIteration();
	private:
        float m_now;
		float m_accumTime;
	};

	template <class T>
	void changeGameLoop()
	{
		if (myGameLoop == nullptr) myGameLoop = new T(this);
		else
		{
			T* pointer = dynamic_cast<T*>(myGameLoop);
			if (pointer == nullptr) myGameLoop = new T(this);
		}
	}

	void loopVariable(float &Now);
	void loopStable(float &Now, float &AcumTime);
	void loopStableSkip(float &Now, float &AcumTime);

	GameLoop*      myGameLoop;
};

